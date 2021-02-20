﻿#include "KFInt32.hpp"

namespace KFrame
{
    KFInt32::KFInt32()
    {
        _data_type = KFDataDefine::DataTypeInt32;
    }

    int32 KFInt32::GetInt32()
    {
        return _data;
    }

    int32 KFInt32::SetInt32( int32 value )
    {
        _data = value;
        if ( _data > ( int32 )_data_setting->_int_max_value )
        {
            _data = ( int32 )_data_setting->_int_max_value;
        }
        else if ( _data < ( int32 )_data_setting->_int_min_value )
        {
            _data = ( int32 )_data_setting->_int_min_value;
        }

        return _data;
    }

    bool KFInt32::IsFull()
    {
        return _data >= ( int32 )_data_setting->_int_max_value;
    }

    void KFInt32::Reset()
    {
        _data = _invalid_int;
        KFData::Reset();
    }

    bool KFInt32::IsValid()
    {
        return _data != _invalid_int;
    }

    void KFInt32::InitData()
    {
        _data = _data_setting->_int_init_value;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    void KFInt32::CopyFrom( DataPtr& other )
    {
        Set( other->Get<int32>() );
    }

    void KFInt32::SaveTo( DataPtr& other )
    {
        other->Set( Get<int32>() );
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

    std::string KFInt32::ToString()
    {
        return KFUtility::ToString( _data );
    }

    void KFInt32::FromString( const std::string& value )
    {
        SetInt32( __TO_INT32__( value ) );
    }
}
