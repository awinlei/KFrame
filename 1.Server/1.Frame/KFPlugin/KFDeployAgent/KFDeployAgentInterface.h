﻿#ifndef __KF_DEPLOY_AGENT_INTERFACE_H__
#define __KF_DEPLOY_AGENT_INTERFACE_H__

#include "KFrame.h"

namespace KFrame
{
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    class KFDeployAgentInterface : public KFModule
    {
    public:
    };


    ///////////////////////////////////////////////////////////////////////
    __KF_INTERFACE__( _kf_deploy_agent, KFDeployAgentInterface );
    ///////////////////////////////////////////////////////////////////////

}



#endif