﻿#ifndef __KF_ACHIEVE_CONFIG_H__
#define __KF_ACHIEVE_CONFIG_H__

#include "KFCore/KFElement.h"
#include "KFConfig.h"
#include "KFXmlReader/KFElementConfig.h"

namespace KFrame
{
    class KFAchieveSetting : public KFIntSetting
    {
    public:
        // 完成条件
        uint32 _complete_condition_type = 0u;
        UInt32Vector _complete_condition;

        // 奖励
        KFElements _rewards;
    };
    /////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    class KFAchieveConfig : public KFConfigT< KFAchieveSetting >, public KFInstance< KFAchieveConfig >
    {
    public:
        KFAchieveConfig()
        {
            _file_name = "achieve";
        }

        virtual void LoadAllComplete();

    protected:
        // 读取配置
        virtual void ReadSetting( KFNode& xmlnode, KFAchieveSetting* kfsetting );
    };
}

#endif