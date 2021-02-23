﻿#include "KFInternal.hpp"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
    std::string KFInternal::RequestLogin( KFJson& json, const KFChannelSetting* setting )
    {
        auto account = __JSON_GET_STRING__( json, __STRING__( account ) );

        // 测试服直接登录
        __JSON_OBJECT_DOCUMENT__( response );
        __JSON_SET_VALUE__( response, __STRING__( account ), account );
        __JSON_SET_VALUE__( response, __STRING__( channel ), setting->_id );
        return _kf_http_server->SendResponse( response );
    }

    std::string KFInternal::RequestPay( const std::string& data, const KFChannelSetting* setting )
    {
        __JSON_PARSE_STRING__( request, data );

        auto order = __JSON_GET_STRING__( request, __STRING__( payorder ) );

        StringMap values;
        __JSON_TO_MAP__( request, values );

        auto ok = SavePayData( order, values );
        return _kf_http_server->SendCode( ok ? KFMsg::Ok : KFMsg::Error );
    }
}