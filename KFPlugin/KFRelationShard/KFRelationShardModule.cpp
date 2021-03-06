﻿#include "KFRelationShardModule.hpp"

namespace KFrame
{
    void KFRelationShardModule::BeforeRun()
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        __REGISTER_MESSAGE__( KFMessageEnum::Player, KFMsg::S2S_QUERY_RELATION_TO_RELATION_REQ, &KFRelationShardModule::HandleQueryRelationToRelationReq );
        __REGISTER_MESSAGE__( KFMessageEnum::Player, KFMsg::S2S_QUERY_RELATION_INVITE_TO_RELATION_REQ, &KFRelationShardModule::HandleQueryRelationInviteToRelationReq );
        __REGISTER_MESSAGE__( KFMessageEnum::Player, KFMsg::S2S_APPLY_ADD_RELATION_TO_RELATION_REQ, &KFRelationShardModule::HandleApplyAddRelationToRelationReq );
        __REGISTER_MESSAGE__( KFMessageEnum::Player, KFMsg::S2S_DEL_RELATION_INVITE_TO_RELATION_REQ, &KFRelationShardModule::HandleDelRelationInviteToRelationReq );
        __REGISTER_MESSAGE__( KFMessageEnum::Player, KFMsg::S2S_ADD_RELATION_TO_RELATION_REQ, &KFRelationShardModule::HandleAddRelationToRelationReq );
        __REGISTER_MESSAGE__( KFMessageEnum::Player, KFMsg::S2S_DEL_RELATION_TO_RELATION_REQ, &KFRelationShardModule::HandleDelRelationToRelationReq );
        __REGISTER_MESSAGE__( KFMessageEnum::Player, KFMsg::S2S_REFUSE_RELATION_TO_RELATION_REQ, &KFRelationShardModule::HandleRefuseRelationToRelationReq );
        __REGISTER_MESSAGE__( KFMessageEnum::Player, KFMsg::S2S_UPDATE_FRIENDLINESS_TO_RELATION_REQ, &KFRelationShardModule::HandleUpdateFriendLinessToRelationReq );
    }

    void KFRelationShardModule::BeforeShut()
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        __UN_MESSAGE__( KFMsg::S2S_QUERY_RELATION_TO_RELATION_REQ );
        __UN_MESSAGE__( KFMsg::S2S_QUERY_RELATION_INVITE_TO_RELATION_REQ );
        __UN_MESSAGE__( KFMsg::S2S_APPLY_ADD_RELATION_TO_RELATION_REQ );
        __UN_MESSAGE__( KFMsg::S2S_DEL_RELATION_INVITE_TO_RELATION_REQ );
        __UN_MESSAGE__( KFMsg::S2S_ADD_RELATION_TO_RELATION_REQ );
        __UN_MESSAGE__( KFMsg::S2S_DEL_RELATION_TO_RELATION_REQ );
        __UN_MESSAGE__( KFMsg::S2S_REFUSE_RELATION_TO_RELATION_REQ );
        __UN_MESSAGE__( KFMsg::S2S_UPDATE_FRIENDLINESS_TO_RELATION_REQ );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFRelationShardModule::MapStringToPBRelation( uint64 player_id, KFMsg::PBRelation* relation_proto, StringMap& basic_data, StringMap& relation_data )
    {
        relation_proto->set_playerid( player_id );
        auto& basic_data_proto = *relation_proto->mutable_basicdata();
        __MAP_TO_PROTO__( basic_data, basic_data_proto );

        auto& relation_data_proto = *relation_proto->mutable_relationdata();
        __MAP_TO_PROTO__( relation_data, relation_data_proto );
    }

    std::string KFRelationShardModule::FormatRelationKey( uint64 first_id, uint64 second_id, std::shared_ptr<const KFRelationSetting> setting )
    {
        auto id1 = first_id;
        auto id2 = second_id;
        if ( setting->_both_way )
        {
            id1 = __MIN__( first_id, second_id );
            id2 = __MAX__( first_id, second_id );
        }

        return __FORMAT__( "{}:{}:{}", setting->_id, id1, id2 );
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    __KF_MESSAGE_FUNCTION__( KFRelationShardModule::HandleQueryRelationToRelationReq, KFMsg::S2SQueryRelationToRelationReq )
    {
        auto setting = KFRelationConfig::Instance()->FindSetting( kfmsg->relationname() );
        if ( setting == nullptr )
        {
            return __LOG_ERROR__( "relatiion=[{}] no setting", kfmsg->relationname() );
        }

        RelationListType relation_list;
        _kf_relation_database->QueryRelationList( setting->_data_list_name, setting->_id, kfmsg->playerid(), relation_list );
        if ( relation_list.empty() )
        {
            return;
        }

        KFMsg::S2SQueryRelationToGameAck ack;
        ack.set_playerid( kfmsg->playerid() );
        ack.set_relationname( kfmsg->relationname() );
        for ( auto& iter : relation_list )
        {
            auto player_id = iter.first;

            StringMap basic_data;
            _kf_basic_database->QueryBasicAttribute( player_id, basic_data );
            if ( basic_data.empty() )
            {
                continue;
            }

            MapStringToPBRelation( player_id, ack.add_pbrelation(), basic_data, iter.second );
        }

        _kf_route->SendToRoute( route, KFMsg::S2S_QUERY_RELATION_TO_GAME_ACK, &ack );
    }

    __KF_MESSAGE_FUNCTION__( KFRelationShardModule::HandleQueryRelationInviteToRelationReq, KFMsg::S2SQueryRelationInviteToRelationReq )
    {
        auto setting = KFRelationConfig::Instance()->FindSetting( kfmsg->relationname() );
        if ( setting == nullptr )
        {
            return __LOG_ERROR__( "relatiion=[{}] no setting", kfmsg->relationname() );
        }

        RelationListType invite_list;
        _kf_relation_database->QueryInviteList( setting->_invite_list_name, setting->_invite_data_name, kfmsg->playerid(), invite_list );
        if ( invite_list.empty() )
        {
            return;
        }

        KFMsg::S2SQueryRelationInviteToGameAck ack;
        ack.set_playerid( kfmsg->playerid() );
        ack.set_relationname( setting->_invite_data_name );
        for ( auto& iter : invite_list )
        {
            auto player_id = iter.first;

            StringMap basic_data;
            _kf_basic_database->QueryBasicAttribute( player_id, basic_data );
            if ( basic_data.empty() )
            {
                continue;
            }

            MapStringToPBRelation( player_id, ack.add_pbinvite(), basic_data, iter.second );
        }
        _kf_route->SendToRoute( route, KFMsg::S2S_QUERY_RELATION_INVITE_TO_GAME_ACK, &ack );
    }

    __KF_MESSAGE_FUNCTION__( KFRelationShardModule::HandleApplyAddRelationToRelationReq, KFMsg::S2SApplyAddRelationToRelationReq )
    {
        auto self_id = __ROUTE_SEND_ID__;
        auto setting = KFRelationConfig::Instance()->FindSetting( kfmsg->relationname() );
        if ( setting == nullptr )
        {
            return __LOG_ERROR__( "relation=[{}] no setting", kfmsg->relationname() );
        }

        auto name = _kf_basic_database->QueryBasicStrValue( kfmsg->playerid(), __STRING__( name ) );
        if ( name.empty() )
        {
            return _kf_display->SendToPlayer( route, KFMsg::RoleNotExist, kfmsg->playername() );
        }

        // 是否已经设置拒绝
        auto refuse_invite = _kf_relation_database->IsRefuse( setting->_refuse_name, kfmsg->playerid() );
        if ( refuse_invite )
        {
            return _kf_display->SendToPlayer( route, KFMsg::RelationRefuseInvite, kfmsg->playername() );
        }

        // 查找对方申请列表是否有自己
        bool is_relation_exist = _kf_relation_database->RelationExist( setting->_invite_list_name, kfmsg->playerid(), self_id );
        if ( is_relation_exist )
        {
            return _kf_display->SendToPlayer( route, KFMsg::RelationInviteAlready, kfmsg->playername() );
        }

        // 查找对方申请列表数量
        auto invite_count = _kf_relation_database->RelationCount( setting->_invite_list_name, kfmsg->playerid() );
        if ( invite_count >= setting->_invite_list_count )
        {
            return _kf_display->SendToPlayer( route, KFMsg::RelationInviteLimit, kfmsg->playername() );
        }

        _kf_relation_database->AddInvite( setting->_invite_list_name, setting->_invite_data_name, kfmsg->playerid(), self_id, kfmsg->message(), setting->_invite_keep_time );
        _kf_display->SendToPlayer( route, KFMsg::RelationInviteOk, kfmsg->playername() );

        // 判断对方是否在线, 如果在线直接发送消息
        auto server_id = _kf_basic_database->QueryBasicServerId( kfmsg->playerid() );
        if ( server_id == _invalid_int )
        {
            return;
        }

        StringMap self_basic_data;
        _kf_basic_database->QueryBasicAttribute( self_id, self_basic_data );
        if ( self_basic_data.empty() )
        {
            return;
        }

        StringMap invite_data;
        invite_data[ __STRING__( message ) ] = kfmsg->message();
        invite_data[ __STRING__( time ) ] = __TO_STRING__( KFGlobal::Instance()->_real_time );

        KFMsg::S2SApplyAddRelationToGameAck ack;
        ack.set_playerid( kfmsg->playerid() );
        ack.set_relationname( setting->_invite_data_name );
        MapStringToPBRelation( self_id, ack.mutable_pbinvite(), self_basic_data, invite_data );
        _kf_route->RepeatToEntity( self_id, server_id, kfmsg->playerid(), KFMsg::S2S_APPLY_ADD_RELATION_TO_GAME_ACK, &ack );
    }

    __KF_MESSAGE_FUNCTION__( KFRelationShardModule::HandleDelRelationInviteToRelationReq, KFMsg::S2SDelRelationInviteToRelationReq )
    {
        auto setting = KFRelationConfig::Instance()->FindSetting( kfmsg->relationname() );
        if ( setting == nullptr )
        {
            return __LOG_ERROR__( "relation=[{}] no setting", kfmsg->relationname() );
        }

        _kf_relation_database->RemoveRelation( setting->_invite_list_name, setting->_invite_data_name, kfmsg->selfplayerid(), kfmsg->targetplayerid(), false );
    }

    __KF_MESSAGE_FUNCTION__( KFRelationShardModule::HandleAddRelationToRelationReq, KFMsg::S2SAddRelationToRelationReq )
    {
        auto self_id = __ROUTE_SEND_ID__;
        auto setting = KFRelationConfig::Instance()->FindSetting( kfmsg->relationname() );
        if ( setting == nullptr )
        {
            return __LOG_ERROR__( "relation=[{}] no setting", kfmsg->relationname() );
        }

        // 判断自己的关系数量
        auto self_count = _kf_relation_database->RelationCount( setting->_data_list_name, self_id );
        if ( self_count >= setting->_max_count )
        {
            return _kf_display->SendToPlayer( route, KFMsg::RelationSelfLimit, setting->_max_count );
        }

        // 判断对方关系的数量
        if ( setting->_both_way )
        {
            auto target_count = _kf_relation_database->RelationCount( setting->_data_list_name, kfmsg->playerid() );
            if ( target_count >= setting->_max_count )
            {
                return _kf_display->SendToPlayer( route, KFMsg::RelationTargetLimit, kfmsg->playername() );
            }
        }

        // 添加关系
        AddRelation( self_id, kfmsg->playerid(), setting );
        if ( setting->_both_way )
        {
            AddRelation( kfmsg->playerid(), self_id, setting );
        }
    }

    void KFRelationShardModule::AddRelation( uint64 player_id, uint64 targetid, std::shared_ptr<const KFRelationSetting> setting )
    {
        _kf_relation_database->AddRelation( setting->_data_list_name, setting->_id, player_id, targetid, setting->_both_way );
        if ( !setting->_invite_data_name.empty() )
        {
            _kf_relation_database->RemoveRelation( setting->_invite_list_name, setting->_invite_data_name, player_id, targetid, false );
        }

        // 发送消息给游戏玩家
        auto server_id = _kf_basic_database->QueryBasicServerId( player_id );
        if ( server_id == _invalid_int )
        {
            return;
        }

        // 获得对方数据
        StringMap query_target_data;
        _kf_basic_database->QueryBasicAttribute( targetid, query_target_data );
        if ( query_target_data.empty() )
        {
            return;
        }

        StringMap relation_data;
        relation_data[ __STRING__( time ) ] = __TO_STRING__( KFGlobal::Instance()->_real_time );

        KFMsg::S2SAddRelationToGameAck ack;
        ack.set_playerid( player_id );
        ack.set_relationname( setting->_id );
        MapStringToPBRelation( targetid, ack.mutable_pbrelation(), query_target_data, relation_data );
        _kf_route->RepeatToEntity( player_id, server_id, player_id, KFMsg::S2S_ADD_RELATION_TO_GAME_ACK, &ack );
    }

    __KF_MESSAGE_FUNCTION__( KFRelationShardModule::HandleDelRelationToRelationReq, KFMsg::S2SDelRelationToRelationReq )
    {
        auto self_id = __ROUTE_SEND_ID__;
        auto setting = KFRelationConfig::Instance()->FindSetting( kfmsg->relationname() );
        if ( setting == nullptr )
        {
            return __LOG_ERROR__( "relation=[{}] no setting", kfmsg->relationname() );
        }

        // 删除关系
        DelRelation( self_id, kfmsg->playerid(), setting );
        if ( setting->_both_way )
        {
            DelRelation( kfmsg->playerid(), self_id, setting );
        }
    }

    void KFRelationShardModule::DelRelation( uint64 player_id, uint64 targetid, std::shared_ptr<const KFRelationSetting> setting )
    {
        _kf_relation_database->RemoveRelation( setting->_data_list_name, setting->_id, player_id, targetid, setting->_both_way );

        auto server_id = _kf_basic_database->QueryBasicServerId( player_id );
        if ( server_id == _invalid_int )
        {
            return;
        }

        KFMsg::S2SDelRelationToGameAck ack;
        ack.set_playerid( player_id );
        ack.set_relationid( targetid );
        ack.set_relationname( setting->_id );
        _kf_route->SendToEntity( player_id, server_id, player_id, KFMsg::S2S_DEL_RELATION_TO_GAME_ACK, &ack );
    }

    __KF_MESSAGE_FUNCTION__( KFRelationShardModule::HandleRefuseRelationToRelationReq, KFMsg::S2SRefuseRelationToRelationReq )
    {
        _kf_relation_database->SetRefuse( kfmsg->refusename(), kfmsg->playerid(), kfmsg->refusevalue() );
    }

    __KF_MESSAGE_FUNCTION__( KFRelationShardModule::HandleUpdateFriendLinessToRelationReq, KFMsg::S2SUpdateFriendLinessToRelationReq )
    {
        UpdateFriendliness( kfmsg->selfplayerid(), kfmsg->targetplayerid(), kfmsg->type(), kfmsg->friendliness() );
    }

    void KFRelationShardModule::UpdateFriendliness( uint64 selfplayerid, uint64 targetplayerid, uint32 type, uint32 addvalue )
    {
        //// 计算总好友度
        //auto friendkey = FormatRelationKey( __STRING__( friend ), selfplayerid, targetplayerid );

        //auto querycurfriendliness = _relation_redis_driver->QueryUInt64( "hget {} {}", friendkey, __STRING__( friendliness ) );
        //auto nowfriendliness = querycurfriendliness->_value;
        //if ( nowfriendliness >= _max_friend_liness->_uint32_value )
        //{
        //    return;
        //}

        //// 计算当天剩余好感度
        //auto friendlinesslimitkey = FormatFriendLimitKey( selfplayerid, targetplayerid, type );
        //auto queryfriendliness = _relation_redis_driver->QueryUInt64( "hget {} {}", __STRING__( friendlinesslimit ), friendlinesslimitkey );
        //auto maxfriendliness = _kf_option->GetUInt32( __STRING__( freindlinessdailymax ), type );
        //if ( queryfriendliness->_value >= maxfriendliness )
        //{
        //    return;
        //}

        //// 计算剩余总的好感度
        //auto leftfriendliness = maxfriendliness - addvalue;
        //auto addfriendliness = __MIN__( leftfriendliness, addvalue );
        //auto restfriendliness = _max_friend_liness->_uint32_value - nowfriendliness;
        //addfriendliness = __MIN__( addfriendliness, restfriendliness );

        //if ( addfriendliness > _invalid_int )
        //{
        //    _relation_redis_driver->Append( "hincrby {} {} {}", friendkey, __STRING__( friendliness ), addfriendliness );
        //    _relation_redis_driver->Append( "hincrby {} {} {}", __STRING__( friendlinesslimit ), friendlinesslimitkey, addfriendliness );
        //    _relation_redis_driver->Pipeline();

        //    // 发送消息给玩家的好友
        //    SendAddFriendlinessToPlayer( selfplayerid, targetplayerid, addfriendliness );
        //    SendAddFriendlinessToPlayer( targetplayerid, selfplayerid, addfriendliness );
        //}
    }

    void KFRelationShardModule::SendAddFriendlinessToPlayer( uint64 self_id, uint64 targetid, uint32 friendliness )
    {
        //auto queryserverid = _public_redis->QueryUInt64( "hget {}:{} {}", __STRING__( public ), self_id, __STRING__( server_id ) );
        //if ( queryserverid->_value == _invalid_int )
        //{
        //    return;
        //}

        //KFMsg::S2SUpdateFriendLinessToGameAck ack;
        //ack.set_playerid( self_id );
        //ack.set_targetplayerid( targetid );
        //ack.set_friendliness( friendliness );
        //_kf_route->SendToPlayer( self_id, queryserverid->_value, self_id, KFMsg::S2S_UPDATE_FRIENDLINESS_TO_GAME_ACK, &ack, true );
    }

    //__KF_MESSAGE_FUNCTION__( KFRelationShardModule::HandleAddRecentPlayerDataReq )
    //{
    //    __PROTO_PARSE__( KFMsg::S2SAddRecentPlayerDataReq );

    //    static auto _recent_keep_time = _kf_option->FindOption( __STRING__( recentkeeptime ) );
    //    static auto _recent_max_count = _kf_option->FindOption( __STRING__( recentmaxcount ) );

    //    auto strbattlerecored = __FORMAT__( "{}:{}:{}", __STRING__( battlerecored ), kfmsg->playerid(), kfmsg->roomid() );

    //    // 先保存战斗数据, 设置有效时间默认7天
    //    auto& strdata = KFProto::Serialize( &kfmsg->pbdata(), KFCompressEnum::Convert );
    //    _relation_redis_driver->Append( "set {} {}", strbattlerecored, strdata );
    //    _relation_redis_driver->Append( "expire {} {}", strbattlerecored, _recent_keep_time->_uint32_value );
    //    _relation_redis_driver->Pipeline();

    //    // 添加到玩家列表
    //    for ( auto i = 0; i < kfmsg->members_size(); ++i )
    //    {
    //        auto strrecentplayer = __FORMAT__( "{}:{}", __STRING__( recentplayer ), kfmsg->members( i ) );
    //        _relation_redis_driver->Execute( "zadd {} {} {}", strrecentplayer, kfmsg->roomid(), kfmsg->playerid() );

    //        // 判断数量, 超过限制数量, 删除最久远的记录
    //        // 战场id是按时间生成的, 所以最新的都是最大的
    //        auto querycount = _relation_redis_driver->QueryUInt64( "zcard {}", strrecentplayer );
    //        if ( querycount->_value > _recent_max_count->_uint32_value )
    //        {
    //            _relation_redis_driver->Execute( "zremrangebyrank {} 0 0", strrecentplayer );
    //        }
    //    }
    //}

    //__KF_MESSAGE_FUNCTION__( KFRelationShardModule::HandleQueryRecentListReq )
    //{
    //    __PROTO_PARSE__( KFMsg::S2SQueryRecentListReq );

    //    KFMsg::S2SQueryRecentListAck ack;
    //    ack.set_playerid( kfmsg->playerid() );

    //    // 查询列表
    //    auto queryrecentlist = _relation_redis_driver->QueryMap( "zrange {}:{} 0 -1 withscores", __STRING__( recentplayer ), kfmsg->playerid() );
    //    for ( auto& iter : queryrecentlist->_value )
    //    {
    //        // 查询记录
    //        auto player_id = __TO_UINT64__( iter.first );
    //        auto roomid = __TO_UINT64__( iter.second );

    //        auto querydata = _relation_redis_driver->QueryString( "get {}:{}:{}", __STRING__( battlerecored ), player_id, roomid );
    //        if ( !querydata->IsOk() )
    //        {
    //            continue;
    //        }

    //        // 找不到了 删除列表
    //        if ( querydata->_value.empty() )
    //        {
    //            _relation_redis_driver->Execute( "zrem {}:{} {}", __STRING__( recentplayer ), kfmsg->playerid(), player_id );
    //            continue;
    //        }

    //        // 查询玩家的基础信息
    //        auto queryplayerdata = _public_redis_driver->QueryMap( "hgetall {}:{}", __STRING__( public ), player_id );
    //        if ( queryplayerdata->_value.empty() )
    //        {
    //            continue;
    //        }

    //        auto pbrelation = ack.add_pbrelation();
    //        KFProto::Parse( pbrelation->mutable_relationdata(), querydata->_value, KFCompressEnum::Convert );
    //        MapStringToPBPlayer( queryplayerdata->_value, player_id, pbrelation );
    //    }

    //    _kf_cluster_shard->SendToClient( kfid, KFMsg::S2S_QUERY_RECENT_LIST_ACK, &ack );
    //}
}