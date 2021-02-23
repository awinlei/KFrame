#ifndef	__KF_CONSTANT_CONFIG_H__
#define	__KF_CONSTANT_CONFIG_H__

#include "KFConfig.h"

namespace KFrame
{
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	class KFConstantSetting : public KFIntSetting
	{
	public:
		// �ֶ���(���ܸ�����ֶ�)
		std::string _name;

		// ��ֵ
		uint32 _key = 0u;

		// ������ֵ
		std::string _value;

	};

	/////////////////////////////////////////////////////////////////////////////////
	class KFConstantConfig : public KFConfigT<KFConstantSetting>, public KFInstance<KFConstantConfig>
	{
	public:
		KFConstantConfig()
		{
			_key_name = "id";
			_file_name = "constant";
		}

		~KFConstantConfig() = default;

	protected:
		virtual void ReadSetting( KFXmlNode& xml_node, std::shared_ptr<KFConstantSetting> setting )
		{
			setting->_name = xml_node.ReadString( "name", true );
			setting->_key = xml_node.ReadUInt32( "key", true );
			setting->_value = xml_node.ReadString( "value", true );
		}

	};

	/////////////////////////////////////////////////////////////////////////////////
}
#endif
