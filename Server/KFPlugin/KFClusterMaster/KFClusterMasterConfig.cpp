﻿#include "KFClusterMasterConfig.h"

namespace KFrame
{
    const KFClusterSetting* KFClusterMasterConfig::FindClusterSetting( const std::string& name )
    {
        auto iter = _cluster_setting_list.find( name );
        if ( iter == _cluster_setting_list.end() )
        {
            return nullptr;
        }

        return &iter->second;
    }

    bool KFClusterMasterConfig::LoadConfig()
    {
        _cluster_setting_list.clear();
        //////////////////////////////////////////////////////////////////
        KFXml kfxml( _file );
        auto config = kfxml.RootNode();
        auto clusters = config.FindNode( "Clusters" );
        auto cluster = clusters.FindNode( "Cluster" );
        while ( cluster.IsValid() )
        {
            KFClusterSetting kfsetting;

            kfsetting._name = cluster.GetString( "Name" );
            kfsetting._type = cluster.GetString( "Type" );
            kfsetting._key = cluster.GetString( "Key" );
            _cluster_setting_list[ kfsetting._name ] = kfsetting;

            cluster.NextNode();
        }
        //////////////////////////////////////////////////////////////////

        return true;
    }
}