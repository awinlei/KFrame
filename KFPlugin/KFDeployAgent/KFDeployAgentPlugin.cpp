﻿#include "KFDeployAgentPlugin.hpp"
#include "KFDeployAgentModule.hpp"
#include "KFLoader/KFLoaderInterface.h"
//////////////////////////////////////////////////////////////////////////

namespace KFrame
{
    void KFDeployAgentPlugin::Install()
    {
        __REGISTER_MODULE__( KFDeployAgent );
    }

    void KFDeployAgentPlugin::UnInstall()
    {
        __UN_MODULE__( KFDeployAgent );
    }

    void KFDeployAgentPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_mysql, KFMySQLInterface );
        __FIND_MODULE__( _kf_timer, KFTimerInterface );
        __FIND_MODULE__( _kf_loader, KFLoaderInterface );
        __FIND_MODULE__( _kf_message, KFMessageInterface );
        __FIND_MODULE__( _kf_ip_address, KFIpAddressInterface );
        __FIND_MODULE__( _kf_tcp_server, KFTcpServerInterface );
        __FIND_MODULE__( _kf_tcp_client, KFTcpClientInterface );
        __FIND_MODULE__( _kf_http_client, KFHttpClientInterface );
    }
}