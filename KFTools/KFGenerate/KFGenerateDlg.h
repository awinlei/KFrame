﻿
// KFGenerateDlg.h: 头文件
//

#pragma once
#include "KFGenerateLogic.h"
#include "KFGenerateParse.h"
#include "KFGenerateVersion.h"
#include "KFGenerateRepository.h"
#include "KFGenerateSSH.h"
using namespace KFrame;


// CKFGenerateDlg 对话框
class CKFGenerateDlg : public CDialogEx
{
    // 构造
public:
    CKFGenerateDlg( CWnd* pParent = nullptr );	// 标准构造函数

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_KFGENERATE_DIALOG };
#endif

protected:
    virtual void DoDataExchange( CDataExchange* pDX );	// DDX/DDV 支持


    // 初始化
    void InitGenerateDialog();
    // 加载配置
    void LoadXmlData();
    // 初始化界面数据
    void InitControlData();

    // 注册事件函数
    void InitEventFunction();

    // 初始化仓库
    void InitRepository();

    // 关闭
    void ShutdownGenerate();

    // 选择文件夹
    std::string BrowsePath();


protected:
    // 添加信息到列表
    void AddInfoToList( const std::string& info );

    // 添加Excel文件
    void AddExcelFile( EventData* eventdata );

    // 删除Excel文件
    void RemoveExcelFile( EventData* eventdata );

    // 查找Excel文件
    int32 FindExcelFile( const std::string& filename );

    // 解析所有excel文件
    void ParseAllExcels( bool repository );

    // 修改文件类型
    void ChangeExcelType( uint32 type );

    // 重置Excel文件列表
    void ResetExcelFileList();

    // 解析文件成功
    void ParseExcelOk( EventData* eventdata );

    // 解析文件成功
    void ParseExcelFailed( EventData* eventdata );

    // 解析文件完成
    void ParseExcelFinish( EventData* eventdata );

    // 显示信息
    void ShowLogicMessage( EventData* eventdata );

    // 仓库提交上传
    void ThreadRunPushCommit();

    // 解析文件完成
    void RepositoryPushOk( EventData* eventdata );

    // 执行ssh失败
    void ExecuteSSHOk( EventData* eventdata );
    void ExecuteSSHFailed( EventData* eventdata );

    // 远程执行命令
    void ThreadRunSSHCommand();

protected:
    // 是否上传仓库
    bool _need_repository = false;;

    // 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL PreTranslateMessage( MSG* pMsg );
    CEdit  _edit_cpp_path;
    CEdit _edit_server_xml_path;
    afx_msg void OnBnClickedButton8();
    CEdit _edit_client_xml_path;
    afx_msg void OnBnClickedButton5();
    CEdit _edit_csharp_path;
    CEdit _edit_lua_path;
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    CButton _check_server_type;
    CButton _check_client_type;
    CComboBox _combo_repository_list;
    CComboBox _combo_server_list;
    afx_msg void OnCbnSelchangeCombo1();
    afx_msg void OnCbnSelchangeCombo2();
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnBnClickedCheck2();
    CListCtrl _list_excel;
    afx_msg void OnTimer( UINT_PTR nIDEvent );
    afx_msg void OnClose();
    afx_msg void OnNMRClickList1( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnChangeExcelServerType();
    afx_msg void OnChangeExcelClientType();
    afx_msg void OnChangeExcelCommonType();
    afx_msg void OnBnClickedButton6();
    CListBox _list_info;
    CButton _button_generate;
    afx_msg void OnBnClickedButton7();
    CButton _button_repository;
};
