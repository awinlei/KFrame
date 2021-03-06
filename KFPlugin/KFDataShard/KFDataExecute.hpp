﻿#ifndef __KF_DATA_EXECUTE_H__
#define __KF_DATA_EXECUTE_H__

#include "KFDataSaveConfig.hpp"

namespace KFrame
{
    class KFDataKeeper
    {
    public:
        // 小区id
        uint32 _zone_id = 0u;

        // 玩家id
        uint64 _player_id = 0u;

        // 玩家数据
        std::string _player_data;

        // 保存标记
        uint32 _save_flag = 0u;

        // 第几次保存
        uint32 _save_count = 0;

        // 下次保存时间
        uint64 _next_save_time = 0u;

    public:
        // 设置保存时间
        void SetSaveTime( uint64 now_time );
    };

    class KFDataExecute
    {
    public:
        KFDataExecute() = default;
        virtual ~KFDataExecute() = default;

        // 初始化
        void InitExecute( std::shared_ptr<const KFDataSaveSetting> setting );

        // 逻辑执行
        void RunDataKeeper();

        // 保存玩家数据
        void SavePlayerData( uint32 zone_id, uint64 player_id, const KFMsg::PBObject* player_object, uint32 save_flag );

        // 读取玩家数据
        KFResult<std::string>::UniqueType LoadPlayerData( uint32 zone_id, uint64 player_id );

    protected:
        // 初始化
        virtual void InitExecute() = 0;

        // 保存数据
        virtual bool SaveData( uint32 zone_id, uint64 player_id, const std::string& player_data, uint32 save_flag ) = 0;

        // 读取数据
        virtual KFResult<std::string>::UniqueType LoadData( uint32 zone_id, uint64 player_id ) = 0;

    public:
        // 数据配置
        std::shared_ptr<const KFDataSaveSetting> _data_save_setting = nullptr;
    protected:
        // 保存失败的数据
        KFHashMap<uint64, KFDataKeeper> _data_keeper;
    };

    ///////////////////////////////////////////////////////////////////////////////////
    class KFRedisDataExecute : public KFDataExecute
    {
    protected:
        // 初始化
        virtual void InitExecute() {};

        // 保存玩家数据
        virtual bool SaveData( uint32 zone_id, uint64 player_id, const std::string& player_data, uint32 save_flag );

        // 读取玩家数据
        virtual KFResult<std::string>::UniqueType LoadData( uint32 zone_id, uint64 player_id );
    };
    ///////////////////////////////////////////////////////////////////////////////////
    class KFMongoDriver;
    class KFMongoDataExecute : public KFDataExecute
    {
    protected:
        // 初始化
        virtual void InitExecute() {}

        // 保存玩家数据
        virtual bool SaveData( uint32 zone_id, uint64 player_id, const std::string& player_data, uint32 save_flag );

        // 读取玩家数据
        virtual KFResult<std::string>::UniqueType LoadData( uint32 zone_id, uint64 player_id );
    };
    ///////////////////////////////////////////////////////////////////////////////////
    class KFMySQLDataExecute : public KFDataExecute
    {
    protected:
        // 初始化
        virtual void InitExecute() {};

        // 保存玩家数据
        virtual bool SaveData( uint32 zone_id, uint64 player_id, const std::string& player_data, uint32 save_flag );

        // 读取玩家数据
        virtual KFResult<std::string>::UniqueType LoadData( uint32 zone_id, uint64 player_id );
    };
    ///////////////////////////////////////////////////////////////////////////////////
}



#endif