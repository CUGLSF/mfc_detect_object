
// mfc_detect_objectDlg.h : ͷ�ļ�
//

#pragma once
#include "textprogress.h"


// Cmfc_detect_objectDlg �Ի���
class Cmfc_detect_objectDlg : public CDialogEx
{
// ����
public:
	Cmfc_detect_objectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_DETECT_OBJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
