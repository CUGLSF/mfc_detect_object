
// mfc_detect_object.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cmfc_detect_objectApp:
// �йش����ʵ�֣������ mfc_detect_object.cpp
//

class Cmfc_detect_objectApp : public CWinApp
{
public:
	Cmfc_detect_objectApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cmfc_detect_objectApp theApp;