﻿#ifndef __KF_BUS_MODULE_H__
#define __KF_BUS_MODULE_H__

/************************************************************************
//    @Module			:    Bus总线连接模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2018-7-4
************************************************************************/

#include "KFBusInterface.h"
#include "KFBusConfig.hpp"
#include "KFTimer/KFTimerInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFIpAddress/KFIpAddressInterface.h"
#include "KFTcpClient/KFTcpClientInterface.h"

namespace KFrame
{
    class KFBusModule : public KFBusInterface
    {
    public:
        KFBusModule() = default;
        ~KFBusModule() = default;

        virtual void BeforeRun();
        virtual void PrepareRun();

        // 关闭
        virtual void ShutDown();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
    protected:
        // 通知注册
        __KF_MESSAGE_FUNCTION__( HanldeTellRegisterToServer, KFMsg::TellRegisterToServer );

        // 取消注册
        __KF_MESSAGE_FUNCTION__( HanldeTellUnRegisterFromServer, KFMsg::TellUnRegisterFromServer );

        // 连接master服务器
        __KF_TIMER_FUNCTION__( OnTimerConnectionMaster );

        // 连接master失败
        __KF_NET_EVENT_FUNCTION__( OnClientConnectMasterFailed );

        // 断开master连接
        __KF_NET_EVENT_FUNCTION__( OnClientDisconnectMaster );

    protected:
        // 判断是否需要连接
        bool CheckNeedConnection( const std::string& connect_name, const std::string& connect_type, uint64 connect_id );

        // 查找连接
        std::shared_ptr<const KFBusSetting> FindMasterConnection( const std::string& app_name, const std::string& app_type, const std::string& app_id );

    private:
        // 连接master失败次数
        uint32 _connect_master_failed_count = 0u;
    };
}

#endif