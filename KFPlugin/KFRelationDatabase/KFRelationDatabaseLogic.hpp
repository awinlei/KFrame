﻿#ifndef __KF_RELATION_DATABASE_LOGIC_H__
#define __KF_RELATION_DATABASE_LOGIC_H__

#include "KFrame.h"

namespace KFrame
{
    typedef std::map<uint64, StringMap> RelationListType;
    class KFRelationDatabaseLogic
    {
    public:
        KFRelationDatabaseLogic() = default;
        virtual ~KFRelationDatabaseLogic() = default;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 查询好友列表
        virtual void QueryRelationList( const std::string& listname, const std::string& relationname, uint64 player_id, RelationListType& relationlist ) = 0;

        // 查询申请列表
        virtual void QueryInviteList( const std::string& listname, const std::string& relationname, uint64 player_id, RelationListType& relationlist ) = 0;

        // 查询关系是否存在
        virtual bool RelationExist( const std::string& listname, uint64 player_id, uint64 targetid ) = 0;

        // 查询关系数量
        virtual uint32 RelationCount( const std::string& listname, uint64 player_id ) = 0;

        // 添加关系
        virtual void AddRelation( const std::string& listname, const std::string& relationname, uint64 player_id, uint64 targetid, bool bothway ) = 0;

        // 删除关系
        virtual void RemoveRelation( const std::string& listname, const std::string& relationname, uint64 player_id, uint64 targetid, bool bothway ) = 0;

        // 添加邀请数据
        virtual void AddInvite( const std::string& listname, const std::string& relationname, uint64 player_id, uint64 targetid, const std::string& message, uint64 keeptime ) = 0;

        // 是否拒绝
        virtual bool IsRefuse( const std::string& refusename, uint64 player_id ) = 0;

        // 设置拒绝
        virtual void SetRefuse( const std::string& refusename, uint64 player_id, uint32 refusevalue ) = 0;
    };
}



#endif