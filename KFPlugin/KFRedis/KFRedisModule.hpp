﻿#ifndef __KF_REDIS_MODULE_H__
#define __KF_REDIS_MODULE_H__

/************************************************************************
//    @Module			:    redis数据库
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-1-11
************************************************************************/

#include "KFRedisInterface.h"
#include "KFRedisLogic.hpp"

namespace KFrame
{
    class KFRedisModule : public KFRedisInterface
    {
    public:
        KFRedisModule() = default;
        ~KFRedisModule() = default;

        // 关闭
        virtual void ShutDown();

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // 创建
        virtual KFRedisDriver* Create( const std::string& module, uint32 logicid = 0 );

    protected:
        // 查找redis配置
        const KFRedisConnectOption* FindRedisConnectOption( const std::string& module, uint32 logicid );

        // 查找
        KFRedisLogic* FindRedisLogic( uint32 id );

        // 插入redis
        void InsertRedisLogic( uint32 id, KFRedisLogic* kfredislogic );

    private:
        // 线程锁
        KFMutex _mt_mutex;

        // 多线程列表
        typedef std::pair< uint32, uint32 > RedisKey;
        KFMap< RedisKey,  KFRedisLogic > _redis_logic;
    };
}



#endif