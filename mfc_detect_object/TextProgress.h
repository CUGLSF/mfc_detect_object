#pragma once


// CTextProgress

class CTextProgress : public CProgressCtrl
{
	DECLARE_DYNAMIC(CTextProgress)

public:
	CTextProgress();
	virtual ~CTextProgress();

protected:
	DECLARE_MESSAGE_MAP()
public:
	COLORREF m_crText;
	COLORREF m_crProgress;
	COLORREF m_crBlank;
	afx_msg void OnPaint();
};


