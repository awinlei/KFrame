#ifndef	__KF_EXECUTE_CONFIG_H__
#define	__KF_EXECUTE_CONFIG_H__

#include "KFConfig.h"

namespace KFrame
{
	/////////////////////////////////////////////////////////////////////////////////
	class ExecuteData
	{
	public:
		// ִ���߼��б�
		ExecuteDataPtr _execute;

	};

	/////////////////////////////////////////////////////////////////////////////////
	class KFExecuteSetting : public KFIntSetting
	{
	public:
		// ���ִ���б�
		std::vector<ExecuteData> _execute_data;
	};

	/////////////////////////////////////////////////////////////////////////////////
	class KFExecuteConfig : public KFConfigT< KFExecuteSetting >, public KFInstance< KFExecuteConfig >
	{
	public:
		KFExecuteConfig()
		{
			_key_name = "id";
			_file_name = "execute";
		}

		~KFExecuteConfig() = default;

		virtual void LoadAllComplete()
		{
			for ( auto& iter : _setting_list._objects )
			{
				auto setting = iter.second;

				for ( auto& executedata : setting->_execute_data)
				{
					auto& execute = executedata._execute;
					if ( execute->_name == __STRING__( data ) )
					{
						auto& data_value = execute->_param_list._params[ 0 ]->_str_value;
						auto& data_name = execute->_param_list._params[ 1 ]->_str_value;
						auto& data_key = execute->_param_list._params[ 2 ]->_int_value;
						KFGlobal::Instance()->FormatElement( execute->_elements, data_name, data_value, data_key );
					}
				}

			}
		}

	protected:
		virtual void ReadSetting( KFXmlNode& xml_node, std::shared_ptr<KFExecuteSetting> setting )
		{
		
			ExecuteData executedata;
			executedata._execute = xml_node.ReadExecuteData( "execute", true );
			setting->_execute_data.push_back( executedata );
		}

	};

	/////////////////////////////////////////////////////////////////////////////////
}
#endif
