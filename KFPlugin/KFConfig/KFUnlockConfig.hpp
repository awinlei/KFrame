#ifndef	__KF_UNLOCK_CONFIG_H__
#define	__KF_UNLOCK_CONFIG_H__

#include "KFConfig.h"

namespace KFrame
{
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	class KFUnlockSetting : public KFIntSetting
	{
	public:
		// ������
		std::string _data_name;

		// ����id
		uint32 _data_key = 0u;

		// ����������
		std::string _child_name;

		// ����ֵ
		uint32 _data_value = 0u;

		// ��������
		DynamicConditionGroupPtr _unlock_condition;

	};

	/////////////////////////////////////////////////////////////////////////////////
	class KFUnlockConfig : public KFConfigT< KFUnlockSetting >, public KFInstance< KFUnlockConfig >
	{
	public:
		KFUnlockConfig()
		{
			_key_name = "id";
			_file_name = "unlock";
		}

		~KFUnlockConfig() = default;

	protected:
		virtual void ReadSetting( KFXmlNode& xml_node, std::shared_ptr<KFUnlockSetting> setting )
		{
			setting->_data_name = xml_node.ReadString( "dataname", true );
			setting->_data_key = xml_node.ReadUInt32( "datakey", true );
			setting->_child_name = xml_node.ReadString( "childname", true );
			setting->_data_value = xml_node.ReadUInt32( "datavalue", true );
			setting->_unlock_condition = xml_node.ReadDynamicConditionGroup( "unlockcondition", true );
		}

	};

	/////////////////////////////////////////////////////////////////////////////////
}
#endif
