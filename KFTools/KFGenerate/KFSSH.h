#ifndef __KF_SSH_H__
#define __KF_SSH_H__

#include "KFGenerateDefine.h"
#include "libssh2.h"

namespace KFrame
{
    class KFSSH
    {
    public:
        static void Init();

        // �ر���Դ
        static void Shutdown();

    public:
        KFSSH( const std::string& ip, uint32 port, const std::string& user, const std::string& password );
        ~KFSSH();

        // command ��Զ��ִ�е�����磺whoami
        bool Execute( const std::string& command );

    protected:
        int32 GetErrorCode() const;

        // ��ô�����Ϣ
        std::string GetErrorMessage() const;

        // �����Դ
        void Cleanup();

        // ��������
        bool CreateSession( bool nonblocking );

        // ����
        void HandShake();

        // �ȴ���ʱ
        bool TimeWaitSocket();

        // ����host
        void SetKnownHosts();

        // ��֤����
        void ValidateAuthorization( const std::string& password );

        // ��ssh channel
        LIBSSH2_CHANNEL* OpenChannel();

        // �ر� ssh channel
        int32 CloseChannel( LIBSSH2_CHANNEL* channel, std::string* exitsignal, std::string* errmsg );

        // ��ȡ ssh channel
        int32 ReadChannel( LIBSSH2_CHANNEL* channel );
    private:
        int32 _socket_fd = 0;
        LIBSSH2_SESSION* _session = nullptr;
        std::string _ip;
        uint32 _port = 0u;
        std::string _user;
    };
}

#endif
