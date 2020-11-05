﻿#ifndef __KF_ENTER_MODULE_H__
#define __KF_ENTER_MODULE_H__

/************************************************************************
//    @Module			:    角色进入游戏模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-11-10
************************************************************************/

#include "KFEnterInterface.h"
#include "KFLua/KFLuaInterface.h"
#include "KFPlayer/KFPlayerInterface.h"
#include "KFXmlReader/KFEnterConfig.hpp"

namespace KFrame
{
    class KFEnterModule : public KFEnterInterface
    {
    public:
        KFEnterModule() = default;
        ~KFEnterModule() = default;


        virtual void BeforeRun();
        virtual void BeforeShut();

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
    protected:
        // 进入游戏世界
        __KF_PLAYER_ENTER_FUNCTION__( EnterGameWorld );
    };
}



#endif