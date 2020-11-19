#ifndef	__KF_MAIL_CONFIG_H__
#define	__KF_MAIL_CONFIG_H__

#include "KFConfig.h"

namespace KFrame
{
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	class KFMailSetting : public KFIntSetting
	{
	public:
		// ����
		uint32 _type = 0u;

		// ����
		std::string _title;

		// ����
		std::string _content;

		// ��Чʱ��(��)
		uint32 _valid_time = 0u;

		// ������������
		KFElements _reward;

		// ��ȡ��Ļظ��ʼ�id(0��ʾ���ظ�)
		uint32 _reply_id = 0u;

	};

	/////////////////////////////////////////////////////////////////////////////////
	class KFMailConfig : public KFConfigT< KFMailSetting >, public KFInstance< KFMailConfig >
	{
	public:
		KFMailConfig()
		{
			_key_name = "id";
			_file_name = "mail";
		}

		~KFMailConfig() = default;

		virtual void LoadAllComplete()
		{
			for ( auto& iter : _settings._objects )
			{
				auto kfsetting = iter.second;

				KFGlobal::Instance()->ParseElement( kfsetting->_reward, _file_name.c_str(), kfsetting->_row );
			}
		}

	protected:
		virtual void ReadSetting( KFXmlNode& xmlnode, KFMailSetting* kfsetting )
		{
			kfsetting->_type = xmlnode.ReadUInt32( "type", true );
			kfsetting->_title = xmlnode.ReadString( "title", true );
			kfsetting->_content = xmlnode.ReadString( "content", true );
			kfsetting->_valid_time = xmlnode.ReadUInt32( "validtime", true );
			kfsetting->_reward._str_parse = xmlnode.ReadString( "reward", true );
			kfsetting->_reply_id = xmlnode.ReadUInt32( "replyid", true );
		}

	};

	/////////////////////////////////////////////////////////////////////////////////
}
#endif
