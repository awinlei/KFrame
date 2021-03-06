﻿#ifndef __KF_GENERATE_EVENT_H__
#define __KF_GENERATE_EVENT_H__

#include "KFGenerateDefine.h"

namespace KFrame
{
    typedef std::function<void( EventData* )> KFEventFunction;
    class KFGenerateEvent : public KFSingleton<KFGenerateEvent>
    {
    public:
        // 抛出一个事件
        void AddEvent( uint32 type, uint32 intparam, const std::string& strparam );

        // 执行时间
        void ExecuteEvent();

        template<class T>
        void RegisterEventFunction( uint32 type, T* object, void ( T::*handle )( EventData* ) )
        {
            auto kffunction = _event_function.Create( type );
            kffunction->SetFunction( std::bind( handle, object, std::placeholders::_1 ) );
        }

        template<typename... P>
        void ShowEventMessage( const std::string& myfmt, P&& ... args )
        {
            auto content = __FORMAT__( myfmt, std::forward<P>( args )... );
            AddEvent( EventType::ShowMessage, 0, content );
        }

    public:
        // 事件锁
        KFMutex _mutex;

        // 事件列表
        std::list< EventData* > _event_list;

        // 事件回调列表
        KFMapFunction<uint32, KFEventFunction> _event_function;
    };
}
///////////////////////////////////////////////////////////////////////////////////////////////
static auto _event = KFrame::KFGenerateEvent::Instance();
///////////////////////////////////////////////////////////////////////////////////////////////

#endif
