// TextProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "mfc_detect_object.h"
#include "TextProgress.h"
#include "cv.h"


// CTextProgress

IMPLEMENT_DYNAMIC(CTextProgress, CProgressCtrl)

CTextProgress::CTextProgress()
{
	m_crText = RGB(0,255,0);      
	m_crProgress = RGB(255,255,255);   
	m_crBlank = RGB(0,0,255);
}

CTextProgress::~CTextProgress()
{
}


BEGIN_MESSAGE_MAP(CTextProgress, CProgressCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTextProgress 消息处理程序
void CTextProgress::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	//打开下一张图片时，肯定需要把进度条调成空白
	PAINTSTRUCT ps; 
	CDC* pDC = BeginPaint(&ps); 
	int nPos = GetPos(); 
	CRect clientRC; 
	GetClientRect(clientRC); 
	pDC->SetBkMode(TRANSPARENT); 
	int nMin, nMax; 
	GetRange(nMin, nMax);  
	double dFraction = (double)clientRC.Width() / (nMax-nMin); 
	int nLeft = nPos * dFraction; 
	CRect leftRC  = clientRC; 
	leftRC.right  = nLeft; 
	CRect rightRC = clientRC; 
	rightRC.left  = nLeft;
	for(int m=255;m>0;m--) 
	{ 
		int x; 
		x = leftRC.Width() * m / 255; 
		pDC->FillRect(CRect(0,0,x,leftRC.Height()),&CBrush(RGB(255,m,0))); 
	} 
	pDC->FillRect(rightRC, &CBrush(RGB(255, 255, 255)));  
	ReleaseDC(pDC); 
	EndPaint(&ps);  
	// 不为绘图消息调用 CProgressCtrl::OnPaint()
}
