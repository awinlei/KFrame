﻿#include "KFEnterModule.hpp"

namespace KFrame
{
    void KFEnterModule::BeforeRun()
    {
        __REGISTER_PLAYER_ENTER__( &KFEnterModule::EnterGameWorld );
    }

    void KFEnterModule::BeforeShut()
    {
        __UN_PLAYER_ENTER__();
    }

    __KF_PLAYER_ENTER_FUNCTION__( KFEnterModule::EnterGameWorld )
    {
        // 上线时间
        player->Set( __STRING__( logintime ), KFGlobal::Instance()->_real_time );
        ////////////////////////////////////////////////////////////////////////////////
        auto playerid = player->GetKeyID();
        auto kfnoterecord = player->Find( __STRING__( note ) );
        for ( auto& iter : KFEnterConfig::Instance()->_settings._objects )
        {
            auto setting = iter.second;
            if ( setting->_id != _invalid_int )
            {
                auto notevalue = kfnoterecord->Get( setting->_id, __STRING__( value ) );
                if ( notevalue != _invalid_int )
                {
                    continue;
                }

                // 设置属性
                player->UpdateRecord( kfnoterecord, setting->_id, __STRING__( value ), KFEnum::Set, 1 );
            }

            // 调用脚本
            _kf_lua->Call( setting->_lua_file, setting->_lua_function, playerid );
        }
    }
}