﻿#include "KFDirClientModule.hpp"
namespace KFrame
{

    void KFDirClientModule::BeforeRun()
    {

    }

    void KFDirClientModule::BeforeShut()
    {
    }

    void KFDirClientModule::UpdateGateToDir( uint64 app_id, const std::string& ip, uint32 port, uint32 count, uint32 expire_time )
    {
        static auto _dir_url = _kf_ip_address->GetDirUrl() + __STRING__( zoneupdate );

        auto setting = KFZoneConfig::Instance()->FindSetting( KFGlobal::Instance()->_app_id->GetZoneId() );
        if ( setting == nullptr )
        {
            return __LOG_ERROR__( "can't find zone=[{}] setting", KFGlobal::Instance()->_app_id->GetZoneId() );
        }

        __JSON_OBJECT_DOCUMENT__( kfjson );
        __JSON_SET_VALUE__( kfjson, __STRING__( appid ), app_id );
        __JSON_SET_VALUE__( kfjson, __STRING__( loginzoneid ), setting->_login_id );
        __JSON_SET_VALUE__( kfjson, __STRING__( ip ), ip );
        __JSON_SET_VALUE__( kfjson, __STRING__( port ), port );
        __JSON_SET_VALUE__( kfjson, __STRING__( count ), count );
        __JSON_SET_VALUE__( kfjson, __STRING__( time ), expire_time );
        _kf_http_client->MTGet< KFDirClientModule >( _dir_url, kfjson );
    }

    bool KFDirClientModule::ZoneRegisterToDir()
    {
        auto setting = KFZoneConfig::Instance()->FindSetting( KFGlobal::Instance()->_app_id->GetZoneId() );
        if ( setting == nullptr )
        {
            __LOG_ERROR__( "can't find zone=[{}] setting", KFGlobal::Instance()->_app_id->GetZoneId() );
            return false;
        }

        static auto _zone_recommend_constant = KFGlobal::Instance()->FindConstant( __STRING__( zonerecommend ) );
        if ( _zone_recommend_constant->_uint32_value == 1u )
        {
            // 推荐小区
            static auto _recommend_url = _kf_ip_address->GetDirUrl() + __STRING__( zonerecommend );
            __JSON_OBJECT_DOCUMENT__( kfjson );
            __JSON_SET_VALUE__( kfjson, __STRING__( zoneid ), setting->_id );
            __JSON_SET_VALUE__( kfjson, __STRING__( flag ), setting->_flag );
            __JSON_SET_VALUE__( kfjson, __STRING__( recommend ), setting->_recommend );
            _kf_http_client->MTGet<KFDirClientModule>( _recommend_url, kfjson );
        }

        // 注册小区信息
        {
            auto _register_url = _kf_ip_address->GetDirUrl() + __STRING__( zoneregister );

            // 注册小区信息
            auto zone_name = __FORMAT__( setting->_name, setting->_id );

            __JSON_OBJECT_DOCUMENT__( kfjson );
            __JSON_SET_VALUE__( kfjson, __STRING__( name ), zone_name );
            __JSON_SET_VALUE__( kfjson, __STRING__( zoneid ), setting->_id );
            __JSON_SET_VALUE__( kfjson, __STRING__( flag ), setting->_flag );
            __JSON_SET_VALUE__( kfjson, __STRING__( loginzoneid ), setting->_login_id );

            auto recv_data = _kf_http_client->STGet( _register_url, kfjson );
            __JSON_PARSE_STRING__( kfresult, recv_data );
            auto ret_code = _kf_http_client->GetCode( kfresult );
            return ( ret_code == KFEnum::Ok );
        }
    }

    bool KFDirClientModule::WorldRegisterToDir( uint64 world_id, const std::string& url )
    {
        static auto _dir_url = _kf_ip_address->GetDirUrl() + __STRING__( worldregister );

        __JSON_OBJECT_DOCUMENT__( sendjson );
        __JSON_SET_VALUE__( sendjson, __STRING__( world ), world_id );
        __JSON_SET_VALUE__( sendjson, __STRING__( url ), url );
        auto recv_data = _kf_http_client->STGet( _dir_url, sendjson );

        __JSON_PARSE_STRING__( recvjson, recv_data );
        auto ret_code = _kf_http_client->GetCode( recvjson );
        return ( ret_code == KFEnum::Ok );
    }

    void KFDirClientModule::ZoneBalanceToDir( uint32 zone_id, uint32 count )
    {
        static auto _update_url = _kf_ip_address->GetDirUrl() + __STRING__( zonebalance );

        __JSON_OBJECT_DOCUMENT__( sendjson );
        __JSON_SET_VALUE__( sendjson, __STRING__( count ), count );
        __JSON_SET_VALUE__( sendjson, __STRING__( zoneid ), zone_id );
        _kf_http_client->MTGet< KFDirClientModule >( _update_url, sendjson );
    }

}