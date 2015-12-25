
// mfc_detect_objectDlg.h : 头文件
//

#pragma once
#include "textprogress.h"


// Cmfc_detect_objectDlg 对话框
class Cmfc_detect_objectDlg : public CDialogEx
{
// 构造
public:
	Cmfc_detect_objectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC_DETECT_OBJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenButton();
	afx_msg void OnBnClickedDetectButton();
	afx_msg void OnNMCustomdrawDetectProgress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CTextProgress m_Progress;
	afx_msg void OnBnClickedSaveButton();
	CString outPutFileName;
	int triggerCount;
};
