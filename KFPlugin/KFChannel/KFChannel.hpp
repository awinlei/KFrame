﻿#ifndef __KF_CHANNEL_H__
#define __KF_CHANNEL_H__

#include "KFConfig/KFChannelConfig.hpp"
#include "KFHttpServer/KFHttpServerInterface.h"

namespace KFrame
{
    class KFChannel
    {
    public:
        KFChannel() = default;
        virtual ~KFChannel() = default;

        // 登录请求
        virtual std::string RequestLogin( KFJson& json, std::shared_ptr<const KFChannelSetting> setting ) = 0;

        // 充值请求
        virtual std::string RequestPay( const std::string& data, std::shared_ptr<const KFChannelSetting> setting ) = 0;

    protected:
        // 保存订单信息
        bool SavePayData( const std::string& order, StringMap& values );
    };
}


#endif
