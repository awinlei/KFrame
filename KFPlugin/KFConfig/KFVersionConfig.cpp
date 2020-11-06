﻿#include "KFVersionConfig.hpp"


namespace KFrame
{
    void KFVersionConfig::ReadSetting( KFXmlNode& xmlnode, KFVersionSetting* kfsetting )
    {
        kfsetting->_old_version = kfsetting->_new_version;
        kfsetting->_new_version = xmlnode.ReadString( "version" );
    }
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    const std::string& KFVersionConfig::CheckNeedReload( const std::string& filename )
    {
        auto kfsetting = FindSetting( filename );
        if ( kfsetting == nullptr || !kfsetting->IsNeedReload() )
        {
            return _invalid_string;
        }

        return kfsetting->_new_version;
    }
}