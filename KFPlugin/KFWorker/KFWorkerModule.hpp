﻿#ifndef __KF_WORKER_MODULE_H__
#define __KF_WORKER_MODULE_H__

/************************************************************************
//    @Module			:    数据库工作线程
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-11-21
************************************************************************/

#include "KFrame.h"
#include "KFActor.hpp"
#include "KFWorkerInterface.h"
#include "KFWorkerMessage.hpp"
#include "KFRedis/KFRedisInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFTcpServer/KFTcpServerInterface.h"

namespace KFrame
{
    /////////////////////////////////////////////////////////////////////////
    class KFWorkerModule : public KFWorkerInterface
    {
    public:
        KFWorkerModule();
        ~KFWorkerModule();

        virtual void BeforeRun();
        virtual void Run();

        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // 执行函数
        bool CallFunction( const Route& route, uint32 msgid, const char* data, uint32 length );

        // 取消注册
        virtual void UnRegisterFunction( uint32 msgid );

        //////////////////////////////////////////////////////////////////////////////////////////
        // 发送消息
        virtual void SendToClient( const Route& route, uint32 msgid, ::google::protobuf::Message* message );
        virtual void SendToClient( const Route& route, uint64 serverid, uint32 msgid, google::protobuf::Message* message );

    protected:
        // 转发消息到工作线程
        __KF_TRANSPOND_MESSAGE_FUNCTION__( SendMessageToWorker );

    protected:

        // 添加函数
        virtual void AddFunction( uint32 msgid, KFMessageFunction& function );

        // 查找工作线程
        KFActor* CreateWorkActor();
        KFActor* FindWorkActor();

    private:
        // 处理器函数
        KFMapFunction< uint32, KFMessageFunction > _kf_message_function;

        // 工作者索引
        uint32 _actor_index;

        // 工作者线程
        std::vector< KFActor* > _kf_actor;
    };
}



#endif