﻿#include "KFBasicDatabaseModule.hpp"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
    void KFBasicDatabaseModule::BeforeRun()
    {
        auto database_type = KFGlobal::Instance()->GetUInt32( __STRING__( basicdatabase ) );
        switch ( database_type )
        {
        case KFDatabaseEnum::Mongo:
            _basic_database_logic = __NEW_OBJECT__( KFBasicDatabaseMongo );
            break;
        default:
            _basic_database_logic = __NEW_OBJECT__( KFBasicDatabaseRedis );
            break;
        }
    }

    void KFBasicDatabaseModule::BeforeShut()
    {
        __DELETE_OBJECT__( _basic_database_logic );
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFBasicDatabaseModule::UpdateBasicIntValue( uint64 player_id, uint64 server_id, const StringUInt64& values )
    {
        _basic_database_logic->UpdateBasicIntValue( player_id, server_id, values );
    }

    void KFBasicDatabaseModule::UpdateBasicStrValue( uint64 player_id, uint64 server_id, const StringMap& values )
    {
        _basic_database_logic->UpdateBasicStrValue( player_id, server_id, values );
    }

    uint64 KFBasicDatabaseModule::QueryBasicPlayerId( const std::string& player_name, uint32 zone_id )
    {
        return _basic_database_logic->QueryBasicPlayerId( player_name, zone_id );
    }

    void KFBasicDatabaseModule::ClearBasicServerId( uint64 server_id )
    {
        _basic_database_logic->ClearBasicServerId( server_id );
    }

    uint64 KFBasicDatabaseModule::QueryBasicServerId( uint64 player_id )
    {
        return _basic_database_logic->QueryBasicServerId( player_id );
    }

    uint32 KFBasicDatabaseModule::QueryBasicAttribute( uint64 player_id, StringMap& values )
    {
        return _basic_database_logic->QueryBasicAttribute( player_id, values );
    }

    uint32 KFBasicDatabaseModule::QueryBasicAttribute( const std::string& player_name, uint32 zone_id, StringMap& values )
    {
        auto player_id = QueryBasicPlayerId( player_name, zone_id );
        if ( player_id == 0u )
        {
            return KFMsg::QueryBasicNotExist;
        }

        return QueryBasicAttribute( player_id, values );
    }

    uint32 KFBasicDatabaseModule::SetPlayerName( uint32 zone_id, uint64 player_id, const std::string& old_name, const std::string& new_name )
    {
        return _basic_database_logic->SetPlayerName( zone_id, player_id, old_name, new_name );
    }

    uint64 KFBasicDatabaseModule::QueryBasicIntValue( uint64 player_id, const std::string& data_name )
    {
        return _basic_database_logic->QueryBasicIntValue( player_id, data_name );
    }

    std::string KFBasicDatabaseModule::QueryBasicStrValue( uint64 player_id, const std::string& data_name )
    {
        return _basic_database_logic->QueryBasicStrValue( player_id, data_name );
    }
}