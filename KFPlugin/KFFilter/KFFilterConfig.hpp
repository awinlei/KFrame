﻿#ifndef __KF_FILTER_CONFIG_H__
#define __KF_FILTER_CONFIG_H__

#include "KFrame.h"
#include "KFConfig.h"

namespace KFrame
{
    class KFFilterSetting : public KFIntSetting
    {

    };

    ////////////////////////////////////////////////////////////////////////////////
    class KFWordData
    {
    public:
        // 清空
        void Clear();

        // 判断单词列表为空
        bool IsEmpty();

        // 插入单词
        KFWordData* InsertChild( int8 word );

        // 查找单词
        KFWordData* FindChild( int8 word );
    private:
        // 包含的单词列表
        std::map< int8, KFWordData > _child;
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    class KFFilterConfig : public KFConfigT< KFFilterSetting >, public KFInstance< KFFilterConfig >
    {
    public:
        KFFilterConfig()
        {
            _file_name = "filter";
        }

        // 检查是否有屏蔽字符
        bool CheckFilter( const char* source, uint32 length );

        // 检查屏蔽字符并替换成 **
        uint32 CensorFilter( const char* source, uint32 length );

    protected:
        // 清空配置
        virtual void ClearSetting();

        // 创建配置
        virtual KFFilterSetting* CreateSetting( KFNode& xmlnode );

        // 读取配置
        virtual void ReadSetting( KFNode& xmlnode, KFFilterSetting* kfsetting );

        // 插入单词
        void InsertWord( KFWordData* kfworddata, const char* source, uint32 length, uint32 index );

        // 查找屏蔽单词
        uint32 FindWord( KFWordData* kfworddata, const char* source, uint32 length, uint32 index, uint32& count );

    private:
        // 屏蔽字字典
        KFWordData _root_word_data;
    };
}

#endif