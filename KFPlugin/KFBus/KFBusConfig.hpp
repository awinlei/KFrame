﻿#ifndef __KF_BUS_CONFIG_H__
#define __KF_BUS_CONFIG_H__

#include "KFConfig.h"

namespace KFrame
{
    class KFConnection
    {
    public:
        KFConnection()
        {
        }

    public:
        std::string _app_name;
        std::string _app_type;
        std::string _app_id;

        std::string _connect_name;
        std::string _connect_type;
        std::string _connect_id;

        // 连接间隔
        uint32 _interval = _invalid_int;

        // 数量比
        uint32 _multi = _invalid_int;
    };

    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    class KFBusConfig : public KFConfig, public KFInstance< KFBusConfig >
    {
    public:
        KFBusConfig()
        {
            _file_name = "bus";
        }

        bool LoadConfig( const std::string& filename, const std::string& filepath, uint32 loadmask );

        // 查找连接
        const KFConnection* FindMasterConnection( const std::string& appname, const std::string& apptype, const std::string& appid );

        // 判断是否是连接
        bool IsValidConnection( const std::string& connectname, const std::string& connecttype, const std::string& connectid );
    public:
        // 连接信息
        std::vector< KFConnection > _bus_connection;
    };
}

#endif