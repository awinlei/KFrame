﻿#ifndef __KF_TEAM_SHARD_MODULE_H__
#define __KF_TEAM_SHARD_MODULE_H__

/************************************************************************
//    @Module			:    队伍模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2018-5-16
************************************************************************/

#include "KFTeamShardInterface.h"
#include "KFKernel/KFKernelInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFDisplay/KFDisplayInterface.h"
#include "KFRouteClient/KFRouteClientInterface.h"
#include "KFConfig/KFTeamConfig.hpp"

namespace KFrame
{
    //////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////
    class KFTeamShardModule : public KFTeamShardInterface
    {
    public:
        KFTeamShardModule() = default;
        ~KFTeamShardModule() = default;

        // 初始化
        virtual void BeforeRun();

        // 关闭
        virtual void BeforeShut();

    protected:
        // 连接成功
        void OnRouteConnectCluster( uint64 server_id );

        // 更换队长
        void ChangeTeamCaptain( EntityPtr team_data, uint64 captain_id );

        // 设置队长信息
        void SetTeamCaptain( EntityPtr team_data, DataPtr member_data, bool update );

        // 发送消息到队伍
        void SendMessageToTeam( EntityPtr team_data, uint32 msg_id, ::google::protobuf::Message* message );
        void SendMessageToMember( DataPtr member_data, uint32 msg_id, ::google::protobuf::Message* message );

        // 通知加入队伍
        void SendJoinTeamToMember( EntityPtr team_data, DataPtr member_data );

        // 同步更新属性到客户端
        void SendTeamUpdateDataToMember( EntityPtr team_data, KFMsg::PBObject& proto_object );

        // 同步添加属性到客户端
        void SendTeamAddDataToMember( EntityPtr team_data, KFMsg::PBObject& proto_object );

        // 同步删除属性到客户端
        void SendTeamRemoveDataToMember( EntityPtr team_data, KFMsg::PBObject& proto_object );

        // 删除队员
        void RemoveTeamMember( EntityPtr team_data, uint64 member_id );
    protected:
        // 创建队伍请求
        __KF_MESSAGE_FUNCTION__( HandleTeamCreateToTeamReq, KFMsg::S2STeamCreateToTeamReq );

        // 加入队伍失败
        __KF_MESSAGE_FUNCTION__( HandleTeamJoinFailedToTeamReq, KFMsg::S2STeamJoinFailedToTeamReq );

        // 上线查询队伍
        __KF_MESSAGE_FUNCTION__( HandleTeamOnlineQueryToTeamReq, KFMsg::S2STeamOnlineQueryToTeamReq );

        // 更新队伍属性
        __KF_MESSAGE_FUNCTION__( HandleTeamIntValueToTeamReq, KFMsg::S2STeamIntValueToTeamReq );
        __KF_MESSAGE_FUNCTION__( HandleTeamStrValueToTeamReq, KFMsg::S2STeamStrValueToTeamReq );

        // 更新队伍成员属性
        __KF_MESSAGE_FUNCTION__( HandleTeamMemberIntValueToTeamReq, KFMsg::S2STeamMemberIntValueToTeamReq );
        __KF_MESSAGE_FUNCTION__( HandleTeamMemberStrValueToTeamReq, KFMsg::S2STeamMemberStrValueToTeamReq );

        // 离开队伍
        __KF_MESSAGE_FUNCTION__( HandleTeamLeaveToTeamReq, KFMsg::S2STeamLeaveToTeamReq );
        // 踢出队伍
        __KF_MESSAGE_FUNCTION__( HandleTeamKickToTeamReq, KFMsg::S2STeamKickToTeamReq );

        // 同意加入队伍
        __KF_MESSAGE_FUNCTION__( HandleTeamAgreeToTeamReq, KFMsg::S2STeamAgreeToTeamReq );
    private:
        // 队伍上下文组件
        std::shared_ptr<KFComponent> _component = nullptr;
    };
}

#endif