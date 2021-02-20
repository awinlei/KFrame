﻿#include "KFBasicDatabaseRedis.hpp"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
#define __BASIC_REDIS_DRIVER__ _kf_redis->Create( __STRING__( basic ) )

    void KFBasicDatabaseRedis::UpdateBasicIntValue( uint64 playerid, uint64 serverid, const StringUInt64& values )
    {
        StringMap invalues;
        for ( auto& iter : values )
        {
            invalues[ iter.first ] = __TO_STRING__( iter.second );
        }

        UpdateBasicStrValue( playerid, serverid, invalues );
    }

    void KFBasicDatabaseRedis::UpdateBasicStrValue( uint64 playerid, uint64 serverid, const StringMap& values )
    {
        auto redisdriver = __BASIC_REDIS_DRIVER__;
        redisdriver->HMSet( __DATABASE_KEY_2__( __STRING__( basic ), playerid ), values );

        static auto _get_status_value = []( const StringMap & values )
        {
            auto iter = values.find( __STRING__( status ) );
            if ( iter == values.end() )
            {
                return 0u;
            }

            return __TO_UINT32__( iter->second );
        };

        auto status = _get_status_value( values );
        switch ( status )
        {
        case KFMsg::OnlineStatus:
            redisdriver->SAdd( __DATABASE_KEY_2__( __STRING__( onlinelist ), serverid ), playerid );
            break;
        case KFMsg::OfflineStatus:
            redisdriver->SRem( __DATABASE_KEY_2__( __STRING__( onlinelist ), serverid ), playerid );
            break;
        default:
            break;
        }
    }

    void KFBasicDatabaseRedis::ClearBasicServerId( uint64 serverid )
    {
        auto redisdriver = __BASIC_REDIS_DRIVER__;
        auto kfplayerlist = redisdriver->SMembers( __DATABASE_KEY_2__( __STRING__( onlinelist ), serverid ) );
        if ( kfplayerlist->_value.empty() )
        {
            return;
        }

        StringMap values;
        values[ __STRING__( serverid ) ] = "0";
        values[ __STRING__( status ) ] = __TO_STRING__( KFMsg::OfflineStatus );

        for ( auto& id : kfplayerlist->_value )
        {
            auto strkey = __DATABASE_KEY_2__( __STRING__( basic ), id );
            auto queryserverid = redisdriver->HGetUInt64( strkey, __STRING__( serverid ) );
            if ( queryserverid->_value == serverid )
            {
                redisdriver->HMSet( strkey, values );
            }
        }

        redisdriver->Del( __DATABASE_KEY_2__( __STRING__( onlinelist ), serverid ) );
    }

    uint64 KFBasicDatabaseRedis::QueryBasicServerId( uint64 playerid )
    {
        return QueryBasicIntValue( playerid, __STRING__( serverid ) );
    }

    uint32 KFBasicDatabaseRedis::QueryBasicAttribute( uint64 playerid, StringMap& values )
    {
        auto redisdriver = __BASIC_REDIS_DRIVER__;
        auto kfquery = redisdriver->HGetAll( __DATABASE_KEY_2__( __STRING__( basic ), playerid ) );
        if ( !kfquery->IsOk() )
        {
            return KFMsg::PublicDatabaseBusy;
        }

        if ( kfquery->_value.empty() )
        {
            return KFMsg::NameCanNotFind;
        }

        values.swap( kfquery->_value );
        return KFMsg::Ok;
    }

    std::string KFBasicDatabaseRedis::FormatNameKey( const std::string& name, uint32 zoneid )
    {
        return __DATABASE_KEY_4__( __STRING__( player ), __STRING__( name ), zoneid, name );
    }

    uint32 KFBasicDatabaseRedis::SetPlayerName( uint32 zoneid, uint64 playerid, const std::string& oldname, const std::string& newname )
    {
        auto redisdriver = __BASIC_REDIS_DRIVER__;

        auto newnamekey = FormatNameKey( newname, zoneid );
        auto kfplayerid = redisdriver->GetUInt64( newnamekey );
        if ( !kfplayerid->IsOk() )
        {
            return KFMsg::NameDatabaseBusy;
        }

        // 如果不存在, 设置新名字
        if ( kfplayerid->_value == _invalid_int )
        {
            // 保存名字
            auto kfresult = redisdriver->Set( newnamekey, playerid );
            if ( !kfresult->IsOk() )
            {
                return KFMsg::NameDatabaseBusy;
            }

            // 删除旧的名字关联
            if ( !oldname.empty() )
            {
                auto oldnamekey = FormatNameKey( oldname, zoneid );
                redisdriver->Del( oldnamekey );
            }
        }
        else if ( kfplayerid->_value != playerid )
        {
            // 存在, 并且不是设定者
            return KFMsg::NameAlreadyExist;
        }

        return KFMsg::NameSetOk;
    }

    uint64 KFBasicDatabaseRedis::QueryBasicPlayerid( const std::string& playername, uint32 zoneid )
    {
        auto redisdriver = __BASIC_REDIS_DRIVER__;

        auto namekey = FormatNameKey( playername, zoneid );
        auto kfplayerid = redisdriver->GetUInt64( namekey );
        return kfplayerid->_value;
    }

    uint64 KFBasicDatabaseRedis::QueryBasicIntValue( uint64 playerid, const std::string& data_name )
    {
        auto redisdriver = __BASIC_REDIS_DRIVER__;
        auto kfserverid = redisdriver->HGetUInt64( __DATABASE_KEY_2__( __STRING__( basic ), playerid ), data_name );
        return kfserverid->_value;
    }

    std::string KFBasicDatabaseRedis::QueryBasicStrValue( uint64 playerid, const std::string& data_name )
    {
        auto redisdriver = __BASIC_REDIS_DRIVER__;
        auto kfserverid = redisdriver->HGet( __DATABASE_KEY_2__( __STRING__( basic ), playerid ), data_name );
        return kfserverid->_value;
    }
}