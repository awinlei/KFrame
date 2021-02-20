﻿#include "KFDefine.h"
#include "KFNetSession.hpp"
#include "KFNetEvent.hpp"
#include "KFNetConnector.hpp"
#include "uv.h"

namespace KFrame
{
    KFNetSession::KFNetSession()
    {
        _is_connected = false;
        _is_shutdown = false;
        _is_sending = false;
        _event_type = 0u;

        _uv_write = __NEW_OBJECT__( uv_write_t );
    }

    KFNetSession::~KFNetSession()
    {
        __DELETE_OBJECT__( _uv_write );
    }

    void KFNetSession::InitSession( uint64 id, uint32 queuecount, uint32 headlength )
    {
        _session_id = id;
        _uv_write->data = this;
        _message_head_length = headlength;

        auto extendcount = IsServerSession() ? queuecount : 0u;

        _send_queue.InitQueue( queuecount, extendcount );
        _recv_queue.InitQueue( queuecount, extendcount );
    }

    bool KFNetSession::IsConnected() const
    {
        return _is_connected;
    }

    bool KFNetSession::IsNeedToSend()
    {
        // 已经在发送 断开连接 已经关闭
        if ( _is_sending || !_is_connected || _is_shutdown  )
        {
            return false;
        }

        return true;
    }

    bool KFNetSession::IsServerSession()
    {
        return _session_id == _object_id;
    }

    void KFNetSession::StartSendMessage()
    {
        if ( !_is_connected || _is_shutdown || _is_sending )
        {
            return;
        }

        SendQueueMessage();
    }

    void KFNetSession::SendBuffer( char* buffer, uint32 length )
    {
        _is_sending = true;

        uv_buf_t buff;
        buff.base = buffer;
        buff.len = length;

        _uv_write->data = this;
        uv_write( _uv_write, _uv_stream, &buff, 1, OnSendCallBack );
    }

    void KFNetSession::OnSendCallBack( uv_write_t* uvwrite, int32 status )
    {
        auto netsession = reinterpret_cast< KFNetSession* >( uvwrite->data );
        if ( status != 0 )
        {
            netsession->OnDisconnect( status, __FUNC_LINE__ );
        }
        else
        {
            netsession->OnSendOK();
            netsession->StartSendMessage();
        }
    }

    void KFNetSession::OnSendOK()
    {
        _send_length = 0;
        _is_sending = false;
    }

    uint32 KFNetSession::SendQueueMessage()
    {
        if ( _send_length == 0 )
        {
            do
            {
                // 消息队列为空
                auto message = _send_queue.Front();
                if ( message == nullptr )
                {
                    break;
                }

                // 超过最大长度
                if ( !CheckBufferLength( _send_length, message->_head._length ) )
                {
                    break;
                }

                memcpy( _req_send_buffer + _send_length, &message->_head, _message_head_length );
                _send_length += _message_head_length;

                if ( message->_head._length > 0 )	// 不是空消息
                {
                    memcpy( _req_send_buffer + _send_length, message->_data, message->_head._length );
                    _send_length += message->_head._length;
                }

                // 释放内存
                _send_queue.PopRemove();
            } while ( true );
        }

        if ( _send_length != 0 )
        {
            SendBuffer( _req_send_buffer, _send_length );
        }

        return _send_length;
    }

    bool KFNetSession::CheckBufferLength( uint32 totallength, uint32 messagelength )
    {
        if ( totallength + _message_head_length + messagelength <= KFNetDefine::MaxReqBuffLength )
        {
            return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFNetSession::OnRecvData( const char* buffer, uint32 length )
    {
        // 接受总长度
        _recv_length += length;

        // 处理消息
        ParseBuffToMessage();
    }

    void KFNetSession::ParseBuffToMessage()
    {
        // 长度不足一个消息头
        uint32 nowposition = 0;
        auto nethead = CheckRecvBuffValid( nowposition );
        if ( nethead == nullptr )
        {
            return;
        }

        while ( _recv_length >= ( nowposition + _message_head_length + nethead->_length ) )
        {
            auto recvmessage = KFNetMessage::Create( nethead->_length );
            memcpy( &recvmessage->_head, nethead, _message_head_length );

            nowposition += _message_head_length;
            if ( nethead->_length > 0 )
            {
                recvmessage->CopyData( _req_recv_buffer + nowposition, nethead->_length );
                nowposition += nethead->_length;
            }

            AddRecvMessage( recvmessage );

            // 检查消息的有效性
            nethead = CheckRecvBuffValid( nowposition );
            if ( nethead == nullptr )
            {
                break;
            }
        }

        // 设置长度
        _recv_length -= __MIN__( _recv_length, nowposition );
        if ( _recv_length > 0 && nowposition > 0 )
        {
            memmove( _req_recv_buffer, _req_recv_buffer + nowposition, _recv_length );
        }
    }

    KFNetHead* KFNetSession::CheckRecvBuffValid( uint32 position )
    {
        if ( _recv_length < ( position + _message_head_length ) )
        {
            return nullptr;
        }

        auto nethead = reinterpret_cast< KFNetHead* >( _req_recv_buffer + position );

        // 收到的消息长度有错误
        if ( nethead->_length > KFNetDefine::MaxMessageLength )
        {
            _recv_length = 0;
            return nullptr;
        }

        return nethead;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFNetSession::CloseSession()
    {
        _is_connected = false;
        if ( _uv_stream != nullptr )
        {
            uv_read_stop( _uv_stream );
        }
    }

    void KFNetSession::OnConnect( uv_stream_t* uvstream )
    {
        _is_connected = true;
        _uv_stream = uvstream;

        // 开始接受消息
        StartRecvData();
    }

    void KFNetSession::StartRecvData()
    {
        if ( !_is_connected || _is_shutdown )
        {
            return;
        }

        _uv_stream->data = this;
        uv_read_start( _uv_stream, AllocRecvBuffer, OnRecvCallBack );
    }

    void KFNetSession::AllocRecvBuffer( uv_handle_t* handle, size_t size, uv_buf_t* pbuffer )
    {
        auto netsession = reinterpret_cast< KFNetSession* >( handle->data );

        pbuffer->base = netsession->_req_recv_buffer + netsession->_recv_length;
        pbuffer->len = KFNetDefine::MaxRecvBuffLength - netsession->_recv_length;
    }

    void KFNetSession::OnRecvCallBack( uv_stream_t* uvstream, ssize_t length, const uv_buf_t* pbuffer )
    {
        auto netsession = reinterpret_cast< KFNetSession* >( uvstream->data );
        if ( length < 0 )
        {
            return netsession->OnDisconnect( static_cast< int32 >( length ), __FUNC_LINE__ );
        }

        // 接受数据
        netsession->OnRecvData( pbuffer->base, static_cast< uint32 >( length ) );
        netsession->StartRecvData();
    }

    void KFNetSession::OnDisconnect( int32 code, const char* function, uint32 line )
    {
        _is_connected = false;
        if ( IsServerSession() )
        {
            __LOG_ERROR_FUNCTION__( function, line, "session[{}:{}] disconnect[{}:{}]", _session_id, KFAppId::ToString( _session_id ), code, uv_err_name( code ) );
        }
        else
        {
            __LOG_INFO_FUNCTION__( function, line, "session[{}:{}] disconnect[{}:{}]", _session_id, _object_id, code, uv_err_name( code ) );
        }
    }

    bool KFNetSession::AddSendMessage( KFNetMessage* message )
    {
        auto ok = _send_queue.PushObject( message, _object_id, __FUNC_LINE__ );
        if ( !ok )
        {
            if ( !_is_send_queue_full )
            {
                _is_send_queue_full = true;

                if ( IsServerSession() )
                {
                    __LOG_ERROR__( "session[{}:{}] send msgid[{}] failed", _session_id, KFAppId::ToString( _session_id ), message->_head._msgid );
                }
                else
                {
                    __LOG_ERROR__( "session[{}:{}] send msgid[{}] failed", _session_id, _object_id, message->_head._msgid );
                }
            }
        }
        else
        {
            _is_send_queue_full = false;
        }

        return ok;
    }

    bool KFNetSession::AddRecvMessage( KFNetMessage* message )
    {
        auto ok = _recv_queue.PushObject( message, _object_id, __FUNC_LINE__ );
        if ( !ok )
        {
            if ( !_is_recv_queue_full )
            {
                _is_recv_queue_full = true;

                if ( IsServerSession() )
                {
                    __LOG_ERROR__( "session[{}:{}] recv msgid[{}] failed", _session_id, KFAppId::ToString( _session_id ), message->_head._msgid );
                }
                else
                {
                    __LOG_ERROR__( "session[{}:{}] recv msgid[{}] failed", _session_id, _object_id, message->_head._msgid );
                }
            }
        }
        else
        {
            _is_recv_queue_full = false;
        }

        return ok;
    }

    bool KFNetSession::SetMainLoopEvent( uint32 eventtype )
    {
        // 已经在关闭, 不再加入队列
        if ( _is_shutdown ||
                _event_type == KFNetDefine::CloseEvent )
        {
            return false;
        }

        _event_type = eventtype;
        return true;
    }

    void KFNetSession::HandleMainLoopEvent()
    {
        uint32 eventtype = _event_type.exchange( 0u );
        switch ( eventtype )
        {
        case KFNetDefine::ConnectEvent:
            StartSession();
            break;
        case KFNetDefine::SendEvent:
            StartSendMessage();
            break;
        case KFNetDefine::CloseEvent:
            CloseSession();
            break;
        case KFNetDefine::DisconnectEvent:
            OnDisconnect( 0, __FUNC_LINE__ );
            break;
        default:
            break;
        }
    }
}

