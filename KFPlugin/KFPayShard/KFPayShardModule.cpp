﻿#include "KFPayShardModule.hpp"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
#define __PAY_REDIS_DRIVER__ _kf_redis->Create( __STRING__( pay ) )

    void KFPayShardModule::BeforeRun()
    {
        // pay
        __REGISTER_HTTP__( __STRING__( applyorder ), false, &KFPayShardModule::HandleApplyOrder );
        __REGISTER_HTTP__( __STRING__( removeorder ), false, &KFPayShardModule::HandleRemoveOrder );
        __REGISTER_HTTP__( __STRING__( querypay ), false, &KFPayShardModule::HandleQueryPay );
        __REGISTER_HTTP__( __STRING__( finishpay ), false, &KFPayShardModule::HandleFinishPay );
        __REGISTER_HTTP__( __STRING__( internalpay ), false, &KFPayShardModule::HandleInternalPay );

        /////////////////////////////////////////////////////////////////////////////////////////
    }

    void KFPayShardModule::BeforeShut()
    {
        // pay
        __UN_HTTP__( __STRING__( applyorder ) );
        __UN_HTTP__( __STRING__( removeorder ) );
        __UN_HTTP__( __STRING__( querypay ) );
        __UN_HTTP__( __STRING__( finishpay ) );
        __UN_HTTP__( __STRING__( internalpay ) );
        ///////////////////////////////////////////////////////////////////////////
    }
    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
    __KF_HTTP_FUNCTION__( KFPayShardModule::HandleApplyOrder )
    {
        __JSON_PARSE_STRING__( request, data );

        auto order = __JSON_GET_STRING__( request, __STRING__( payorder ) );
        auto pay_id = __JSON_GET_STRING__( request, __STRING__( payid ) );
        auto player_id = __JSON_GET_UINT64__( request, __STRING__( playerid ) );
        if ( player_id == _invalid_int || pay_id.empty() || order.empty() )
        {
            return _kf_http_server->SendCode( KFMsg::PayDataError );
        }

        // 订单保存时间( 1个月 )
        static auto _order_expire_time = 3600 * 24 * 30;
        auto order_key_name = __FORMAT__( "{}:{}", __STRING__( payorder ), order );

        // 保存到数据库
        auto redis_driver = __PAY_REDIS_DRIVER__;

        __JSON_OBJECT_DOCUMENT__( response );
        __JSON_SET_VALUE__( response, __STRING__( playerid ), player_id );

        // 保存订单信息
        StringMap values;
        __JSON_TO_MAP__( request, values );
        redis_driver->Append( values, "hmset {}", order_key_name );
        redis_driver->Append( "expire {} {}", order_key_name, _order_expire_time );
        auto result = redis_driver->Pipeline();
        if ( result->IsOk() )
        {
            __JSON_SET_VALUE__( response, __STRING__( payid ), payid );
            __JSON_SET_VALUE__( response, __STRING__( payorder ), order );
            __JSON_SET_VALUE__( response, __STRING__( retcode ), KFMsg::Ok );
        }
        else
        {
            __JSON_SET_VALUE__( response, __STRING__( retcode ), KFMsg::AuthDatabaseBusy );
        }

        return _kf_http_server->SendResponse( response );
    }

    __KF_HTTP_FUNCTION__( KFPayShardModule::HandleRemoveOrder )
    {
        __JSON_PARSE_STRING__( request, data );

        // 删除订单
        auto order = __JSON_GET_STRING__( request, __STRING__( payorder ) );

        auto redis_driver = __AUTH_REDIS_DRIVER__;
        redis_driver->Execute( "del {}:{}", __STRING__( payorder ), order );

        return _kf_http_server->SendCode( KFMsg::Ok );
    }

    __KF_HTTP_FUNCTION__( KFPayShardModule::HandleQueryPay )
    {
        __JSON_PARSE_STRING__( request, data );
        auto player_id = __JSON_GET_UINT64__( request, __STRING__( playerid ) );

        __JSON_OBJECT_DOCUMENT__( response );
        __JSON_SET_VALUE__( response, __STRING__( playerid ), player_id );

        // 查询玩家充值列表
        auto redis_driver = __AUTH_REDIS_DRIVER__;
        auto kfresult = redis_driver->QueryList( "smembers {}:{}", __STRING__( paydata ), player_id );
        if ( !kfresult->_value.empty() )
        {
            __JSON_ARRAY__( payarray );
            for ( auto& order : kfresult->_value )
            {
                auto kfpaydata = redis_driver->QueryMap( "hgetall {}:{}", __STRING__( pay ), order );
                if ( !kfpaydata->_value.empty() )
                {
                    auto flag = __TO_UINT32__( kfpaydata->_value[ __STRING__( flag ) ] );
                    if ( flag == _invalid_int )
                    {
                        __JSON_OBJECT__( payjson );
                        __JSON_SET_VALUE__( payjson, __STRING__( payorder ), kfpaydata->_value[ __STRING__( payorder ) ] );
                        __JSON_SET_VALUE__( payjson, __STRING__( payid ), kfpaydata->_value[ __STRING__( payid ) ] );

                        __JSON_ADD_VALUE__( payarray, payjson );

                        // 更新状态
                        auto updataresult = redis_driver->Execute( "hset {}:{} {} {}", __STRING__( pay ), order, __STRING__( flag ), 1 );
                        if ( !updataresult->IsOk() )
                        {
                            __LOG_ERROR__( "update player=[{}] pay=[{}] flag failed", player_id, order );
                        }
                    }
                }
            }

            // 如果不为空, 返回给玩家
            auto size = __JSON_ARRAY_SIZE__( payarray );
            if ( size > 0u )
            {
                __JSON_SET_VALUE__( response, __STRING__( paydata ), payarray );
            }
        }

        return _kf_http_server->SendResponse( response );
    }

    __KF_HTTP_FUNCTION__( KFPayShardModule::HandleFinishPay )
    {
        __JSON_PARSE_STRING__( request, data );
        auto order = __JSON_GET_STRING__( request, __STRING__( payorder ) );
        auto player_id = __JSON_GET_UINT64__( request, __STRING__( playerid ) );

        // 删除充值信息
        auto redis_driver = __AUTH_REDIS_DRIVER__;
        redis_driver->Append( "del {}:{}", __STRING__( pay ), order );
        redis_driver->Append( "srem {}:{} {}", __STRING__( paydata ), player_id, order );
        auto kfresult = redis_driver->Pipeline();
        if ( !kfresult->IsOk() )
        {
            __LOG_WARN__( "player=[{}] finish pay=[{}] failed", player_id, order );
        }

        return _kf_http_server->SendCode( KFMsg::Ok );
    }

    __KF_HTTP_FUNCTION__( KFPayShardModule::HandleInternalPay )
    {
        __LOG_INFO__( "channel=[{}] pay=[{}]", KFMsg::Internal, data );
        return _kf_channel->AuthPay( KFMsg::Internal, data );
    }
    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
}