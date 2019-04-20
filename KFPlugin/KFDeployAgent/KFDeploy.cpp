﻿#include "KFDeploy.hpp"

namespace KFrame
{
    KFDeployData::KFDeployData()
    {
        _process_id = 0;
        _startup_time = 0;
        _zone_id = 0;
        _is_startup = false;
        _is_shutdown = false;
        _is_debug = false;
    }

    std::string KFDeployData::GetAppPath()
    {
        auto apppath = _invalid_str;
        if ( _app_path.empty() )
        {
            apppath = _deploy_path + "/";
        }
        else
        {
            apppath = _deploy_path + "/" + _app_path + "/";
        }

        return apppath;
    }

    std::string KFDeployData::GetStartupFile( bool isdebug )
    {
        auto file = _app_file;
        if ( isdebug )
        {
            file += "d";
        }

#if __KF_SYSTEM__ == __KF_WIN__
        file += ".exe";
#endif
        auto apppath = GetAppPath();
        return apppath + file;
    }

    bool KFDeployData::IsAppServer( const std::string& appname, const std::string& apptype, const std::string& appid, uint32 zoneid )
    {
        // 指定appid
        if ( appid != _globbing_str )
        {
            return ( appid == _app_id );
        }

        // appname
        if ( appname != _globbing_str )
        {
            if ( appname != _app_name )
            {
                return false;
            }
        }

        // apptype
        if ( apptype != _globbing_str )
        {
            if ( apptype != _app_type )
            {
                return false;
            }
        }

        // zoneid
        if ( zoneid != _invalid_int )
        {
            if ( zoneid != _zone_id )
            {
                return false;
            }
        }

        return true;
    }

    void KFDeployData::CopyFrom( MapString& values )
    {
        _deploy_path = values[ __KF_STRING__( deploypath ) ];
        _app_path = values[ __KF_STRING__( apppath ) ];
        _app_file = values[ __KF_STRING__( appfile ) ];

        _app_id = values[ __KF_STRING__( appid ) ];
        KFAppId kfappid( _app_id );
        _zone_id = kfappid.GetZoneId();

        _app_name = values[ __KF_STRING__( appname ) ];
        _app_type = values[ __KF_STRING__( apptype ) ];

        _is_startup = KFUtility::ToValue< uint32 >( values[ __KF_STRING__( startup ) ] );
        _is_debug = KFUtility::ToValue< uint32 >( values[ __KF_STRING__( debug ) ] );
        _is_shutdown = KFUtility::ToValue< uint32 >( values[ __KF_STRING__( shutdown ) ] );
        _process_id = KFUtility::ToValue< uint32 >( values[ __KF_STRING__( process ) ] );
        _startup_time = KFUtility::ToValue< uint64 >( values[ __KF_STRING__( time ) ] );
        _agent_id = values[ __KF_STRING__( agentid ) ];
        _local_ip = values[ __KF_STRING__( localip ) ];
        _log_type = values[ __KF_STRING__( logtype ) ];
        _service_type = values[ __KF_STRING__( service ) ];
        _net_type = values[ __KF_STRING__( net ) ];
        _params = values[ __KF_STRING__( param ) ];
    }

    void KFDeployData::SaveTo( MapString& values )
    {
        values[ __KF_STRING__( deploypath ) ] = _deploy_path;
        values[ __KF_STRING__( apppath ) ] = _app_path;
        values[ __KF_STRING__( appfile ) ] = _app_file;

        values[ __KF_STRING__( appid ) ] = _app_id;
        values[ __KF_STRING__( appname ) ] = _app_name;
        values[ __KF_STRING__( apptype ) ] = _app_type;
        values[ __KF_STRING__( shutdown ) ] = __TO_STRING__( _is_shutdown ? 1 : 0 );
        values[ __KF_STRING__( startup ) ] = __TO_STRING__( _is_startup ? 1 : 0 );
        values[ __KF_STRING__( debug ) ] = __TO_STRING__( _is_startup ? 1 : 0 );
        values[ __KF_STRING__( process ) ] = __TO_STRING__( _process_id );
        values[ __KF_STRING__( time ) ] = __TO_STRING__( _startup_time );
        values[ __KF_STRING__( agentid ) ] = _agent_id;
        values[ __KF_STRING__( localip ) ] = _local_ip;
        values[ __KF_STRING__( logtype ) ] = _log_type;
        values[ __KF_STRING__( service ) ] = _service_type;
        values[ __KF_STRING__( net ) ] = _net_type;
        values[ __KF_STRING__( param ) ] = _params;
    }

}