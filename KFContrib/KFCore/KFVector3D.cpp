﻿#include "KFVector3D.hpp"

namespace KFrame
{
    KFVector3D::KFVector3D()
    {
        _data_type = KFDataDefine::DataTypeVector3D;
    }

    const Math3D::Vector3D& KFVector3D::GetVector3D()
    {
        return _data;
    }

    const Math3D::Vector3D& KFVector3D::SetVector3D( Math3D::Vector3D& value )
    {
        _data = value;
        return _data;
    }

    void KFVector3D::Reset()
    {
        _data.Set( 0.0f, 0.0f, 0.0f );
        KFData::Reset();
    }

    bool KFVector3D::IsValid()
    {
        return ( _data.GetX() != 0.0f || _data.GetY() != 0.0f || _data.GetZ() != 0.0f );
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFVector3D::CopyFrom( DataPtr& other )
    {
        _data = other->Get<Math3D::Vector3D>();
    }

    void KFVector3D::SaveTo( DataPtr& other )
    {
        other->Set( _data );
    }

    std::string KFVector3D::ToString()
    {
        return __FORMAT__( "({:.2f},{:.2f},{:.2f})", _data.GetX(), _data.GetY(), _data.GetY() );
    }

    void KFVector3D::FromString( const std::string& value )
    {
        auto temp = value;
        KFUtility::SplitString( temp, "(" );

        auto x = KFUtility::SplitValue<double>( temp, "," );
        auto y = KFUtility::SplitValue<double>( temp, "," );
        auto z = KFUtility::SplitValue<double>( temp, "," );

        _data.Set( x, y, z );
    }
}