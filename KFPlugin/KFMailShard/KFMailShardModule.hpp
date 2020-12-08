﻿#ifndef __KF_MAIL_SHARD_MODULE_H__
#define __KF_MAIL_SHARD_MODULE_H__

/************************************************************************
//    @Module			:    数据库模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-11-30
************************************************************************/

#include "KFMailShardInterface.h"
#include "KFMailDatabaseRedis.hpp"
#include "KFMailDatabaseMongo.hpp"
#include "KFRedis/KFRedisInterface.h"
#include "KFMongo/KFMongoInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFDelayed/KFDelayedInterface.h"
#include "KFHttpServer/KFHttpServerInterface.h"
#include "KFRouteClient/KFRouteClientInterface.h"
#include "KFBasicDatabase/KFBasicDatabaseInterface.h"

namespace KFrame
{
    class KFMailShardModule : public KFMailShardInterface
    {
    public:
        KFMailShardModule() = default;
        ~KFMailShardModule() = default;

        // 初始化
        virtual void BeforeRun();
        virtual void PrepareRun();

        // 关闭
        virtual void BeforeShut ();
        ////////////////////////////////////////////////////////////////////////////////
    protected:
        // 计划清理过期的全局邮件
        __KF_DELAYED_FUNCTION__( OnDelayedClearWholeOverdueMail );

        // 增加邮件
        __KF_HTTP_FUNCTION__( HandleGMAddMailReq );

        // 删除邮件
        __KF_HTTP_FUNCTION__( HandleGMDeleteMailReq );

    protected:
        // 查询邮件
        __KF_MESSAGE_FUNCTION__ ( HandleQueryMailReq, KFMsg::S2SQueryMailReq );

        // 添加邮件
        __KF_MESSAGE_FUNCTION__( HandleAddMailReq, KFMsg::S2SAddMailReq );

        // 设置邮件状态
        __KF_MESSAGE_FUNCTION__( HandleUpdateMailStatusReq, KFMsg::S2SUpdateMailStatusReq );

        // 新玩家登陆邮件处理
        __KF_MESSAGE_FUNCTION__( HandleNewPlayerMailReq, KFMsg::S2SNewPlayerMailReq );

        ///////////////////////////////////////////////////////////////////////////////
    protected:
        // 添加邮件
        uint64 AddMail( uint32 flag, uint64 objectid, StringMap& maildata );

    private:
        // 邮件数据库逻辑
        KFMailDatabaseLogic* _mail_database_logic = nullptr;
    };
}

#endif