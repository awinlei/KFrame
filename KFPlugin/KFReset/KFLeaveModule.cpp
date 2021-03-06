﻿#include "KFLeaveModule.hpp"

namespace KFrame
{
    void KFLeaveModule::BeforeRun()
    {
        __REGISTER_PLAYER_LEAVE__( &KFLeaveModule::LeaveGameWorld );
    }

    void KFLeaveModule::BeforeShut()
    {
        __UN_PLAYER_LEAVE__();
    }

    __KF_PLAYER_LEAVE_FUNCTION__( KFLeaveModule::LeaveGameWorld )
    {
        // 更新总时间
        auto login_time = player->Get( __STRING__( logintime ) );
        player->Operate( __STRING__( totaltime ), KFEnum::Add, KFGlobal::Instance()->_real_time - login_time );
        ///////////////////////////////////////////////////////////////////////////////////////////
        auto player_id = player->GetKeyID();
        auto note_record = player->Find( __STRING__( note ) );
        for ( auto& iter : KFLeaveConfig::Instance()->_setting_list._objects )
        {
            auto setting = iter.second;
            if ( setting->_id != _invalid_int )
            {
                auto note_value = note_record->Get( setting->_id, __STRING__( value ) );
                if ( note_value != _invalid_int )
                {
                    continue;
                }

                // 设置属性
                player->UpdateRecord( note_record, setting->_id, __STRING__( value ), KFEnum::Set, 1 );
            }

            // 调用脚本
            _kf_lua->Call( setting->_lua_file, setting->_lua_function, player_id );
        }
    }
}