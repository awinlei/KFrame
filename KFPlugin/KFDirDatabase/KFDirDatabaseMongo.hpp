﻿#ifndef __KF_DIR_DATABASE_MONGO_H__
#define __KF_DIR_DATABASE_MONGO_H__

#include "KFDirDatabaseLogic.hpp"
#include "KFMongo/KFMongoInterface.h"

namespace KFrame
{
    class KFDirDatabaseMongo : public KFDirDatabaseLogic
    {
    public:
        KFDirDatabaseMongo() = default;
        ~KFDirDatabaseMongo() = default;

        // 注册小区信息
        virtual bool ZoneRegister( uint32 zone_id, KFJson& zone_data );

        // 更新小区gate在线人数
        virtual bool ZoneUpdate( uint64 app_id, uint32 zone_id, uint32 count, const std::string& ip, uint32 port, uint32 expire_time );

        // 查询小区列表
        virtual StringMapList QueryZoneList( const std::string& flag );

        // 查询小区ip
        virtual StringMap QueryZoneIp( uint32 zone_id );

        // 小区人数负载均衡
        virtual bool ZoneBalance( uint32 zone_id, uint32 count );

        // 分配玩家小区
        virtual StringMap AllocPlayerZone( const std::string& flag, uint32 zone_id );

        // 设置推荐小区
        virtual bool SetZoneRecommend( const std::string& flag, uint32 zone_id, bool is_recommend );

        // 查询小区状态
        virtual uint32 QueryZoneStatus( uint32 zone_id );

        // 更新小区状态
        virtual bool UpdateZoneStatus( uint32 zone_id, uint32 status );

        // world http地址注册
        virtual bool SetWorldUrl( uint64 world_id, const std::string& url );

        // 获得world http地址
        virtual std::string GetWorldUrl( uint64 world_id );

        // 更新masterip
        virtual bool UpdateMasterIp( const std::string& app_name, uint64 app_id, uint32 zone_id, const std::string& ip, uint32 port, uint32 expire_time );

        // 查询masterip
        virtual StringMap QueryMasterIp( const std::string& app_name, uint32 zone_id );

        // 查询masterlist
        virtual StringMapList QueryMasterList( const std::string& app_name, uint32 zone_id );
    public:
        // 分配一个小区
        virtual uint32 BalanceAllocZone( const std::string& flag );

        // 查询推荐小区
        virtual uint32 GetZoneRecommend( const std::string& flag );

        // 查询小区信息
        virtual StringMap QueryZoneData( uint32 zone_id );
    };
}



#endif