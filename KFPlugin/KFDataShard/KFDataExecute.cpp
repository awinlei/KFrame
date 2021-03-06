﻿#include "KFDataExecute.hpp"
#include "KFRedis/KFRedisInterface.h"
#include "KFMongo/KFMongoInterface.h"
#include "KFMySQL/KFMySQLInterface.h"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
    // 每次间隔的时间
#define __PER_SAVE_TIME__ 10000
#define __REDIS_DATA_DRIVER__( zone_id ) _kf_redis->Create( __STRING__( data ), zone_id )
#define __MYSQL_DATA_DRIVER__( zone_id ) _kf_mysql->Create( __STRING__( data ), zone_id )
#define __MONGO_DATA_DRIVER__( zone_id ) _kf_mongo->Create( __STRING__( data ), zone_id )
    /////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////

    void KFDataKeeper::SetSaveTime( uint64 now_time )
    {
        ++_save_count;
        _next_save_time = now_time + ( _save_count * __PER_SAVE_TIME__ );
    }
    /////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
    void KFDataExecute::InitExecute( std::shared_ptr<const KFDataSaveSetting> setting )
    {
        _data_save_setting = setting;
        InitExecute();
    }

    void KFDataExecute::RunDataKeeper()
    {
        UInt64Set removes;
        auto now_time = KFGlobal::Instance()->_real_time;
        for ( auto& iter : _data_keeper._objects )
        {
            auto keeper = iter.second;
            if ( keeper->_next_save_time <= now_time )
            {
                keeper->SetSaveTime( now_time );
                auto ok = SaveData( keeper->_zone_id, keeper->_player_id, keeper->_player_data, keeper->_save_flag );
                if ( ok )
                {
                    removes.insert( keeper->_player_id );
                }
            }
        }

        // 删除
        for ( auto player_id : removes )
        {
            _data_keeper.Remove( player_id );
        }
    }

    void KFDataExecute::SavePlayerData( uint32 zone_id, uint64 player_id, const KFMsg::PBObject* player_object, uint32 save_flag )
    {
        // 没有保存标记
        if ( !KFUtility::HaveBitMask( _data_save_setting->_save_flag, save_flag ) )
        {
            return;
        }

        auto player_data = KFProto::Serialize( player_object, _data_save_setting->_compress_type, _data_save_setting->_compress_level, true );
        if ( player_data == _invalid_string )
        {
            return __LOG_ERROR__( "player[{}:{}] serialize failed", zone_id, player_id );
        }

        // 不在keeper中
        auto keeper = _data_keeper.Find( player_id );
        if ( keeper == nullptr )
        {
            auto ok = SaveData( zone_id, player_id, player_data, save_flag );
            if ( ok )
            {
                return;
            }
        }

        // 保存失败 先缓存下来
        keeper = _data_keeper.Create( player_id );
        keeper->_zone_id = zone_id;
        keeper->_player_id = player_id;
        keeper->_save_flag = save_flag;
        keeper->_player_data = player_data;
        if ( keeper->_next_save_time == 0 )
        {
            keeper->SetSaveTime( KFGlobal::Instance()->_real_time );
        }
    }

    KFResult<std::string>::UniqueType KFDataExecute::LoadPlayerData( uint32 zone_id, uint64 player_id )
    {
        auto keeper = _data_keeper.Find( player_id );
        if ( keeper != nullptr )
        {
            __NEW_RESULT__( std::string );
            result->_value = keeper->_player_data;
            return result;
        }

        return LoadData( zone_id, player_id );
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool KFRedisDataExecute::SaveData( uint32 zone_id, uint64 player_id, const std::string& player_data, uint32 save_flag )
    {
        auto redis_driver = __REDIS_DATA_DRIVER__( zone_id );
        if ( redis_driver == nullptr )
        {
            __LOG_ERROR__( "player[{}:{}] can't find redis", zone_id, player_id );
            return false;
        }

        auto player_key_name = __DATABASE_KEY_2__( __STRING__( player ), player_id );
        auto result = redis_driver->HSet( player_key_name, __STRING__( data ), player_data );
        if ( !result->IsOk() )
        {
            __LOG_ERROR__( "redis player[{}:{}] save failed", zone_id, player_id );
            return false;
        }

        if ( _data_save_setting->_cache_time != 0u && save_flag == KFSaveEnum::OfflineSave )
        {
            // 设置缓存时间
            redis_driver->Expire( player_key_name, _data_save_setting->_cache_time );
        }

        __LOG_INFO__( "redis player [{}:{}] size=[{}] save ok", zone_id, player_id, player_data.size() );
        return true;
    }

    KFResult<std::string>::UniqueType KFRedisDataExecute::LoadData( uint32 zone_id, uint64 player_id )
    {
        auto redis_driver = __REDIS_DATA_DRIVER__( zone_id );
        if ( redis_driver == nullptr )
        {
            __LOG_ERROR__( "player[{}:{}] can't find redis", zone_id, player_id );
            return nullptr;
        }

        return redis_driver->HGet( __DATABASE_KEY_2__( __STRING__( player ), player_id ), __STRING__( data ) );
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool KFMongoDataExecute::SaveData( uint32 zone_id, uint64 player_id, const std::string& player_data, uint32 save_flag )
    {
        auto mongo_driver = __MONGO_DATA_DRIVER__( zone_id );
        if ( mongo_driver == nullptr )
        {
            __LOG_ERROR__( "player[{}:{}] can't find mongo", zone_id, player_id );
            return false;
        }

        mongo_driver->CreateIndex( __STRING__( player ), MongoKeyword::_expire );
        auto ok = mongo_driver->Insert( __STRING__( player ), player_id, __STRING__( data ), player_data, true );
        if ( ok )
        {
            if ( _data_save_setting->_cache_time != 0u && save_flag == KFSaveEnum::OfflineSave )
            {
                mongo_driver->Expire( __STRING__( player ), player_id, _data_save_setting->_cache_time );
            }
        }

        return ok;
    }

    KFResult<std::string>::UniqueType KFMongoDataExecute::LoadData( uint32 zone_id, uint64 player_id )
    {
        auto mongo_driver = __MONGO_DATA_DRIVER__( zone_id );
        if ( mongo_driver == nullptr )
        {
            __LOG_ERROR__( "player[{}:{}] can't find mongo", zone_id, player_id );
            return nullptr;
        }

        return mongo_driver->QueryString( __STRING__( player ), player_id, __STRING__( data ) );
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool KFMySQLDataExecute::SaveData( uint32 zone_id, uint64 player_id, const std::string& player_data, uint32 save_flag )
    {
        auto mysql_driver = __MYSQL_DATA_DRIVER__( zone_id );
        if ( mysql_driver == nullptr )
        {
            __LOG_ERROR__( "player[{}:{}] can't find mysql", zone_id, player_id );
            return false;
        }

        StringMap values;
        values[ __STRING__( data ) ] = player_data;
        values[ __STRING__( id ) ] = __TO_STRING__( player_id );

        auto ok = mysql_driver->Insert( __STRING__( player ), values );
        if ( !ok )
        {
            __LOG_ERROR__( "mysql player[{}:{}] save failed", zone_id, player_id );
            return false;
        }

        __LOG_INFO__( "mysql player [{}:{}] size=[{}] save ok", zone_id, player_id, player_data.size() );
        return true;
    }

    KFResult<std::string>::UniqueType KFMySQLDataExecute::LoadData( uint32 zone_id, uint64 player_id )
    {
        auto mysql_driver = __MYSQL_DATA_DRIVER__( zone_id );
        if ( mysql_driver == nullptr )
        {
            __LOG_ERROR__( "player[{}:{}] can't find mysql", zone_id, player_id );
            return nullptr;
        }

        return mysql_driver->QueryString( "select `{}` from `{}` where `{}`={};", __STRING__( data ), __STRING__( player ), __STRING__( id ), player_id );
    }
}