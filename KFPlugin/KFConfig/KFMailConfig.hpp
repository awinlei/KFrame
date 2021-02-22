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
			for ( auto& iter : _setting_list._objects )
			{
				auto setting = iter.second;

				KFGlobal::Instance()->ParseElement( setting->_reward, _file_name.c_str(), setting->_row );
			}
		}

	protected:
		virtual void ReadSetting( KFXmlNode& xml_node, std::shared_ptr<KFMailSetting> setting )
		{
			setting->_type = xml_node.ReadUInt32( "type", true );
			setting->_title = xml_node.ReadString( "title", true );
			setting->_content = xml_node.ReadString( "content", true );
			setting->_valid_time = xml_node.ReadUInt32( "validtime", true );
			setting->_reward._str_parse = xml_node.ReadString( "reward", true );
			setting->_reply_id = xml_node.ReadUInt32( "replyid", true );
		}

	};

	/////////////////////////////////////////////////////////////////////////////////
}
#endif
