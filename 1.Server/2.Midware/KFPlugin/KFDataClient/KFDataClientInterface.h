﻿#ifndef __KF_DATA_CLIENT_INTERFACE_H__
#define __KF_DATA_CLIENT_INTERFACE_H__

#include "KFrame.h"

namespace KFrame
{
    typedef std::function< void( uint32, uint64, KFMsg::PBObject* ) > KFQueryPlayerFunction;
    typedef std::function< void( uint32, uint64, const std::string&, uint64 ) > KFSetPlayerNameFunction;
    typedef std::function< void( uint32, const KFMsg::PBLoginData*, KFMsg::PBObject* ) > KFLoadPlayerFunction;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class KFDataClientInterface : public KFModule
    {
    public:
        // 绑定加载玩家数据回调函数
        template< class T >
        void BindLoadPlayerFunction( T* object, void ( T::*handle )( uint32, const KFMsg::PBLoginData*, KFMsg::PBObject* ) )
        {
            KFLoadPlayerFunction function = std::bind( handle, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
            SetLoadPlayerFunction( function );
        }

        // 取消绑定
        template< class T >
        void UnBindLoadPlayerFunction( T* object )
        {
            KFLoadPlayerFunction function = nullptr;
            SetLoadPlayerFunction( function );
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 查询玩家数据回调函数
        template< class T >
        void BindQueryPlayerFunction( T* object, void ( T::*handle )( uint32, uint64, KFMsg::PBObject* ) )
        {
            KFQueryPlayerFunction function = std::bind( handle, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
            SetQueryPlayerFunction( function );
        }

        // 取消绑定
        template< class T >
        void UnBindQueryPlayerFunction( T* object )
        {
            KFQueryPlayerFunction function = nullptr;
            SetQueryPlayerFunction( function );
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template< class T >
        void BindSetNameFunction( T* object, void ( T::*handle )( uint32, uint64, const std::string&, uint64 ) )
        {
            KFSetPlayerNameFunction function = std::bind( handle, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 );
            SetPlayerNameFunction( function );
        }

        // 取消绑定
        template< class T >
        void UnBindSetNameFunction( T* object )
        {
            KFSetPlayerNameFunction function = nullptr;
            SetPlayerNameFunction( function );
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 加载玩家数据
        virtual bool LoadPlayerData( const KFMsg::PBLoginData* pblogin ) = 0;

        // 保存玩家数据
        virtual bool SavePlayerData( uint64 playerid, const KFMsg::PBObject* pbplayerdata ) = 0;

        // 查询玩家数据
        virtual bool QueryPlayerData( uint64 sendid, uint64 playerid ) = 0;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 设置名字
        virtual bool SetPlayerName( uint64 playerid, const std::string& oldname, const std::string& newname, uint64 itemguid ) = 0;

    protected:
        // 设置回调函数
        virtual void SetLoadPlayerFunction( KFLoadPlayerFunction& function ) = 0;
        virtual void SetQueryPlayerFunction( KFQueryPlayerFunction& function ) = 0;
        virtual void SetPlayerNameFunction( KFSetPlayerNameFunction& function ) = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    __KF_INTERFACE__( _kf_data_client, KFDataClientInterface );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}



#endif