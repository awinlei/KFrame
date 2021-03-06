﻿#include "KFSignPlugin.hpp"
#include "KFSignModule.hpp"
#include "KFLoader/KFLoaderInterface.h"
//////////////////////////////////////////////////////////////////////////

namespace KFrame
{
    void KFSignPlugin::Install()
    {
        __REGISTER_MODULE__( KFSign );
    }

    void KFSignPlugin::UnInstall()
    {
        __UN_MODULE__( KFSign );
    }

    void KFSignPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_game, KFGameInterface );
        __FIND_MODULE__( _kf_reset, KFResetInterface );
        __FIND_MODULE__( _kf_loader, KFLoaderInterface );
        __FIND_MODULE__( _kf_player, KFPlayerInterface );
        __FIND_MODULE__( _kf_display, KFDisplayInterface );
        __FIND_MODULE__( _kf_message, KFMessageInterface );
    }

    void KFSignPlugin::AddConfig()
    {
        __KF_ADD_CONFIG__( KFSignConfig );
    }
}