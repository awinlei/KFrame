﻿#include "KFConfigConfig.hpp"


namespace KFrame
{
    static std::map< std::string, uint32 > _mask_list =
    {
        {"Reload", KFConfigEnum::CanReload},
        {"ClearData", KFConfigEnum::NeedClearData},
        {"ClearFile", KFConfigEnum::ClearFileData},
        {"ChildFile", KFConfigEnum::ClearChildFile},
    };

    void KFConfigConfig::ReadSetting( KFXmlNode& xmlnode, KFConfigSetting* kfsetting )
    {
        auto filename = xmlnode.ReadString( "FileName" );
        auto configfile = KFUtility::FormatConfigFile( filename, KFGlobal::Instance()->_channel, KFGlobal::Instance()->_service );

        KFConfigData data;
        data._file_path = configfile;
        data._file_name = xmlnode.ReadString( "Name" );
        for ( auto& iter : _mask_list )
        {
            if ( xmlnode.ReadString( iter.first.c_str(), true ) == "1" )
            {
                KFUtility::AddBitMask< uint32 >( data._load_mask, iter.second );
            }
        }

        kfsetting->_config_data_list.push_back( data );
    }

    const KFConfigData* KFConfigSetting::IsFile( const std::string& file ) const
    {
        for ( auto& kfdata : _config_data_list )
        {
            auto pos = kfdata._file_path.find( file );
            if ( pos != std::string::npos )
            {
                return &kfdata;
            }
        }

        return nullptr;
    }

}