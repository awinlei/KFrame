﻿#include "KFrame.h"
#include "KFUuid.hpp"
#include "KFUuidGenerate.hpp"
#include "KFUuidData.hpp"

namespace KFrame
{
    KFUuid::KFUuid()
    {
        _kf_mutex = __NEW_OBJECT__( KFMutex );
    }

    KFUuid::~KFUuid()
    {
        __DELETE_OBJECT__( _kf_mutex );
        for ( auto& iter : _uuid_data_list )
        {
            delete iter.second;
        }
        _uuid_data_list.clear();

        for ( auto& iter : _uuid_setting_list )
        {
            delete iter.second;
        }
        _uuid_setting_list.clear();
    }

    void KFUuid::AddData( const std::string& name, uint64 projecttime, uint32 timebits, uint32 zonebits, uint32 workerbits, uint32 seqbits )
    {
        auto iter = _uuid_setting_list.find( name );
        if ( iter == _uuid_setting_list.end() )
        {
            auto kfsetting = __NEW_OBJECT__( KFUuidData );
            iter = _uuid_setting_list.insert( std::make_pair( name, kfsetting ) ).first;
        }

        iter->second->InitData( projecttime, timebits, zonebits, workerbits, seqbits );
    }

    const KFUuidData* KFUuid::FindUuidData( const std::string& name )
    {
        auto iter = _uuid_setting_list.find( name );
        if ( iter == _uuid_setting_list.end() )
        {
            iter = _uuid_setting_list.find( __STRING__( normal ) );
        }

        return iter->second;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    KFUuidGenerate* KFUuid::CreateUuidGenerate( const std::string& name )
    {
        auto iter = _uuid_data_list.find( name );
        if ( iter != _uuid_data_list.end() )
        {
            return iter->second;
        }

        auto kfdata = FindUuidData( name );
        auto kfuuidgenerate = __NEW_OBJECT__( KFUuidGenerate, kfdata );
        _uuid_data_list[ name ] = kfuuidgenerate;
        return kfuuidgenerate;
    }

    uint64 KFUuid::STMake( const std::string& name, uint32 zoneid, uint32 workerid, uint64 nowtime )
    {
        auto kfuuiddata = CreateUuidGenerate( name );
        return kfuuiddata->Make( zoneid, workerid, nowtime );
    }

    uint64 KFUuid::MTMake( const std::string& name, uint32 zoneid, uint32 workerid, uint64 nowtime )
    {
        KFLocker locker( *_kf_mutex );
        return STMake( name, zoneid, workerid, nowtime );
    }

    uint32 KFUuid::STZoneId( const std::string& name, uint64 uuid )
    {
        auto kfuuiddata = CreateUuidGenerate( name );
        return kfuuiddata->ZoneId( uuid );
    }

    uint32 KFUuid::MTZoneId( const std::string& name, uint64 uuid )
    {
        KFLocker locker( *_kf_mutex );
        return STZoneId( name, uuid );
    }

    std::tuple<uint64, uint32, uint32, uint32> KFUuid::STParse( const std::string& name, uint64 uuid )
    {
        auto kfuuiddata = CreateUuidGenerate( name );
        return kfuuiddata->Parse( uuid );
    }

    std::tuple<uint64, uint32, uint32, uint32> KFUuid::MTParse( const std::string& name, uint64 uuid )
    {
        KFLocker locker( *_kf_mutex );
        return STParse( name, uuid );
    }
}

