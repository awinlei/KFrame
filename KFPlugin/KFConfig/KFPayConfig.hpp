#ifndef	__KF_PAY_CONFIG_H__
#define	__KF_PAY_CONFIG_H__

#include "KFConfig.h"

namespace KFrame
{
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	class KFPaySetting : public KFStrSetting
	{
	public:
		// �۸�
		uint32 _price = 0u;

		// ��ֵ����
		KFElements _reward;

		// ��¼��ֵ���id
		uint32 _note_id = 0u;

		// �׳影��
		KFElements _first_reward;

	};

	/////////////////////////////////////////////////////////////////////////////////
	class KFPayConfig : public KFConfigT< KFPaySetting >, public KFInstance< KFPayConfig >
	{
	public:
		KFPayConfig()
		{
			_key_name = "id";
			_file_name = "pay";
		}

		~KFPayConfig() = default;

		virtual void LoadAllComplete()
		{
			for ( auto& iter : _settings._objects )
			{
				auto kfsetting = iter.second;

				KFGlobal::Instance()->ParseElement( kfsetting->_reward, _file_name.c_str(), kfsetting->_row );
				KFGlobal::Instance()->ParseElement( kfsetting->_first_reward, _file_name.c_str(), kfsetting->_row );
			}
		}

	protected:
		virtual void ReadSetting( KFXmlNode& xmlnode, KFPaySetting* kfsetting )
		{
			kfsetting->_price = xmlnode.ReadUInt32( "price", true );
			kfsetting->_reward._str_parse = xmlnode.ReadString( "reward", true );
			kfsetting->_note_id = xmlnode.ReadUInt32( "noteid", true );
			kfsetting->_first_reward._str_parse = xmlnode.ReadString( "firstreward", true );
		}

	};

	/////////////////////////////////////////////////////////////////////////////////
}
#endif
