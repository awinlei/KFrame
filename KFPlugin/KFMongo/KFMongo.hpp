﻿#ifndef __KF_MONGO_H__
#define __KF_MONGO_H__

#include "KFMongoConfig.hpp"
#include "Poco/MongoDB/Connection.h"

namespace KFrame
{
    using namespace Poco::MongoDB;
    class KFMongo
    {
    public:
        KFMongo() = default;
        virtual ~KFMongo() = default;

        // 初始化
        virtual void InitMongo( const KFMongoSetting* kfsetting );

        // 关闭
        virtual void ShutDown();

        // 是否连接
        bool IsConnected();

        ///////////////////////////////////////////////////////////////////////////////
        bool SendRequest( RequestMessage& request );
        bool SendRequest( RequestMessage& request, ResponseMessage& response );
        ///////////////////////////////////////////////////////////////////////////////

    protected:
        // 连接
        bool ConnectMongo();

        // 判断是否掉线
        bool CheckDisconnected( int32 code );

    protected:
        // 连接字串
        std::string _connect_data;

        // 是否已经连接上
        bool _is_connected = false;

        // 连接
        Connection _connection;

        // 连接工厂
        Connection::SocketFactory _factory;
    };
}


#endif