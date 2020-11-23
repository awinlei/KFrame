﻿#ifndef __KF_CLIENT_MODULE_H__
#define __KF_CLIENT_MODULE_H__

/************************************************************************
//    @Module			:    tcp客户端
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-1-8
************************************************************************/

#include "KFrame.h"
#include "KFTcpClientInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFNetwork/KFNetClientEngine.hpp"

namespace KFrame
{
    class KFTcpClientModule : public KFTcpClientInterface
    {
    public:
        KFTcpClientModule() = default;
        ~KFTcpClientModule() = default;

        // 初始化
        virtual void InitModule();

        // 逻辑
        virtual void BeforeRun();
        virtual void Run();

        // 关闭
        virtual void BeforeShut();
        virtual void ShutDown();
        /////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////
        // 添加客户端连接
        virtual void StartClient( const KFNetData* netdata );
        virtual void StartClient( const std::string& name, const std::string& type, uint64 id, const std::string& ip, uint32 port );

        // 断开连接
        virtual void CloseClient( uint64 serverid, const char* function, uint32 line );
        /////////////////////////////////////////////////////////////////////////

        // 发送消息
        virtual void SendNetMessage( uint32 msgid, google::protobuf::Message* message, uint32 delay = 0 );
        virtual bool SendNetMessage( uint64 serverid, uint32 msgid, google::protobuf::Message* message, uint32 delay = 0 );
        virtual bool SendNetMessage( uint64 serverid, uint64 recvid, uint32 msgid, google::protobuf::Message* message, uint32 delay = 0 );

        virtual void SendNetMessage( uint32 msgid, const char* data, uint32 length );
        virtual bool SendNetMessage( uint64 serverid, uint32 msgid, const char* data, uint32 length );
        virtual bool SendNetMessage( uint64 serverid, uint64 recvid, uint32 msgid, const char* data, uint32 length );

        virtual void SendMessageToName( const std::string& servername, uint32 msgid, google::protobuf::Message* message );
        virtual void SendMessageToName( const std::string& servername, uint32 msgid, const char* data, uint32 length );;

        virtual void SendMessageToType( const std::string& servertype, uint32 msgid, google::protobuf::Message* message );
        virtual void SendMessageToType( const std::string& servertype, uint32 msgid, const char* data, uint32 length );

        virtual void SendMessageToServer( const std::string& servername, const std::string& servertype, uint32 msgid, google::protobuf::Message* message );
        virtual void SendMessageToServer( const std::string& servername, const std::string& servertype, uint32 msgid, const char* data, uint32 length );

        ////////////////////////////////////////////////////////////////////////////////////////////////////////

    protected:
        // 客户端连接
        __KF_NET_EVENT_FUNCTION__( OnClientConnected );

        // 客户端断开
        __KF_NET_EVENT_FUNCTION__( OnClientDisconnect );

        // 客户端关闭
        __KF_NET_EVENT_FUNCTION__( OnClientShutdown );

        // 客户端连接失败
        __KF_NET_EVENT_FUNCTION__( OnClientFailed );

        // 注册回馈
        __KF_MESSAGE_FUNCTION__( HandleRegisterAck );
    private:
        // 连接回调
        void AddConnectionFunction( KFModule* module, KFNetEventFunction& function );
        void RemoveConnectionFunction( KFModule* module );
        void CallClientConnectionFunction( const KFNetData* netdata );

        // 断线函数
        virtual void AddLostFunction( KFModule* module, KFNetEventFunction& function );
        void RemoveLostFunction( KFModule* module );
        void CallClientLostFunction( const KFNetData* netdata );

        // 添加关闭函数
        virtual void AddShutdownFunction( KFModule* module, KFNetEventFunction& function );
        virtual void RemoveShutdownFunction( KFModule* module );
        void CallClientShutdownFunction( const KFNetData* netdata );

        // 添加失败函数
        virtual void AddFailedFunction( KFModule* module, KFNetEventFunction& function );
        virtual void RemoveFailedFunction( KFModule* module );
        void CallClientFailedFunction( const KFNetData* netdata );

        // 转发函数
        virtual void AddTranspondFunction( KFModule* module, KFTranspondFunction& function );
        virtual void RemoveTranspondFunction( KFModule* module );
        ////////////////////////////////////////////////////////////////
        // 处理客户端消息
        void HandleNetMessage( const Route& route, uint32 msgid, const char* data, uint32 length );

        // 是否连接自己
        bool IsSelfConnection( const std::string& name, const std::string& type, uint64 id );
    public:
        // 客户端引擎
        KFNetClientEngine* _client_engine = nullptr;

        // 转发函数
        KFTranspondFunction _kf_transpond_function = nullptr;

        // 注册成功回调函数
        KFFunctionMap< KFModule*, KFNetEventFunction > _kf_connection_function;

        // 客户端掉线
        KFFunctionMap< KFModule*, KFNetEventFunction > _kf_lost_function;

        // 客户端关闭
        KFFunctionMap< KFModule*, KFNetEventFunction > _kf_shutdown_function;

        // 客户端失败
        KFFunctionMap< KFModule*, KFNetEventFunction > _kf_failed_function;
    };
}



#endif