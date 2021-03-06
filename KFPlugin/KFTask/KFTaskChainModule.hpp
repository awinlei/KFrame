﻿#ifndef __KF_TASK_CHAIN_MOUDLE_H__
#define __KF_TASK_CHAIN_MOUDLE_H__

/************************************************************************
//    @Module			:    任务链系统
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2019-10-31
************************************************************************/

#include "KFTaskInterface.h"
#include "KFTaskChainInterface.h"
#include "KFKernel/KFKernelInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFPlayer/KFPlayerInterface.h"
#include "KFDisplay/KFDisplayInterface.h"
#include "KFCondition/KFConditionInterface.h"
#include "KFConfig/KFTaskChainConfig.hpp"
#include "KFConfig/KFWeightConfig.hpp"

namespace KFrame
{
    class KFTaskChainModule : public KFTaskChainInterface
    {
    public:
        KFTaskChainModule() = default;
        ~KFTaskChainModule() = default;

        // 逻辑
        virtual void BeforeRun();

        // 关闭
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
        // 判断是否是任务链任务
        bool IsTaskChain( EntityPtr player, DataPtr task_data );

        // 清除任务链
        void CleanTaskChain( EntityPtr player, uint32 chain_id );

        // 开启任务链
        bool OpenTaskChain( EntityPtr player, uint32 chain_id, uint32 chain_index, uint32 valid_time );
    protected:
        // 添加任务链
        __KF_ADD_ELEMENT_FUNCTION__( AddTaskChainElement );

        // 删除任务
        __KF_REMOVE_DATA_FUNCTION__( OnRemoveTaskTaskChainModule );

        // 开启任务链数据
        bool OpenTaskChainLogicData( EntityPtr player, const TaskChainData* task_chain_data, uint32 chain_id, uint32 chain_index, uint32 valid_time );

        // 开启附加任务链
        void OpenExtendChain( EntityPtr player, uint32 chain_id, const TaskChainData* task_chain_data );

        // 完成任务链任务
        void FinishTaskChain( EntityPtr player, DataPtr task_data );
    protected:
        // 玩家组件上下文
        std::shared_ptr<KFComponent> _component = nullptr;
    };
}

#endif