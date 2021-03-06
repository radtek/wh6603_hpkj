#include "stdafx.h"
#include "GamePlaza.h"
#include "ImgRioButton.h"
#include "gdipng/MemDC.h"


// CImgRioButton

IMPLEMENT_DYNAMIC(CImgRioButton, CButton)

CImgRioButton::CImgRioButton()
: m_pIamge(NULL)
, m_bHaveBitmaps(false)
// , m_pCurBtn(NULL)
, m_imgWidth(0)
, m_imgHeight(0)
, m_bIsHovering(false)
, m_bIsTracking(false)
, m_bIsDisabled(false)
, m_bPushed(false)
, m_TextClr(RGB(250, 250, 250))

{
	m_bDCStored = false;
	m_crTextColor = RGB(250, 250, 250);
	m_font.CreateFont(20,0,0,0,FW_NORMAL,0,0,0,0,3,2,ANTIALIASED_QUALITY,2,TEXT("微软雅黑"));
}

CImgRioButton::~CImgRioButton()
{
	if(m_pIamge) 
	{
		delete m_pIamge;
		m_pIamge = NULL;
	}
// 	if (m_dcStd.m_hDC != NULL)
// 	{
// 		m_dcStd.DeleteDC();
// 	}
// 	if (m_dcBk.m_hDC != NULL)
// 	{
// 		m_dcBk.DeleteDC();
// 	}
// 	if (m_dcStdH.m_hDC != NULL)
// 	{
// 		m_dcStdH.DeleteDC();
// 	}
// 	if (m_dcGS.m_hDC != NULL)
// 	{
// 		m_dcGS.DeleteDC();
// 	}
// 	if (m_dcStdP.m_hDC != NULL)
// 	{
// 		m_dcStdP.DeleteDC();
// 	}

}
VOID CImgRioButton::SetOwnerTextColor(COLORREF crTextColor)
{
	m_crTextColor = crTextColor;
}

void CImgRioButton::Refresh()
{
	CRect	rect;
	GetClientRect(&rect);	
	CClientDC  dc(this);

	dc.BitBlt (0,0,rect.Width (),rect.Height (),&memDC,0,0,SRCCOPY);
}

void CImgRioButton::SetImage(LPCTSTR imgFile)
{
	if(m_pIamge) 
	{
		delete m_pIamge;
		m_pIamge = NULL;
	}

	m_pIamge = new Bitmap(imgFile);
	m_imgWidth = m_pIamge->GetWidth() / 4;
	m_imgHeight = m_pIamge->GetHeight();
}

void CImgRioButton::SetPushed(bool bPushed) 
{
	if (m_bPushed != bPushed) 
	{	
		m_bPushed = bPushed;
		Invalidate();
	}
}

BEGIN_MESSAGE_MAP(CImgRioButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE( WM_MOUSEHOVER, OnMouseHover )  
	ON_MESSAGE( WM_MOUSELEAVE, OnMouseLeave ) 
	ON_WM_PAINT()

END_MESSAGE_MAP()

BOOL CImgRioButton::OnEraseBkgnd(CDC* pDC)
{

	if(!m_bDCStored)
	{
		CRect  clientRect;
		GetClientRect(&clientRect);
		int cx=clientRect.Width ();
		int cy=clientRect.Height ();
		// Store our orignal DC.
		CBitmap	bitmap;
		memDC.CreateCompatibleDC (pDC);
		bitmap.CreateCompatibleBitmap (pDC,cx,cy);
		memDC.SelectObject (&bitmap);
		memDC.BitBlt (0,0,cx,cy,pDC,0,0,SRCCOPY);
		m_bDCStored=true;
	}

	//Invalidate();

//	PaintText(pDC);
// 	CString strLog;
// 	strLog.Format(L"ONERASEBKGND %s",this->GetWindowTextLength());
// 	OutputDebugString(strLog);
	return TRUE;
}
// void CImgRioButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
// {
// 
// 	//DrawButton(lpDrawItemStruct->hDC);
// }

void CImgRioButton::PreSubclassWindow()
{

	//m_pIamge = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_rio_back.png"));
	if(m_pIamge == NULL)
	{
		CString str;
		GetWindowText(str);

		int nLength = str.GetLength();
		if(nLength < 2)
			m_pIamge = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_rio_back_01.png"));
		else if(nLength<5)
			m_pIamge = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_rio_back_02.png"));
		else
			m_pIamge = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\game\\btn_rio_back.png"));
		m_imgWidth = m_pIamge->GetWidth() / 4;
		m_imgHeight = m_pIamge->GetHeight();

	}
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();}

void CImgRioButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{

	Refresh();
	if(m_pIamge == NULL)
		return;
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	Gdiplus::Graphics graphics(pDC->m_hDC);

	graphics.DrawImage(m_pIamge, Rect(0, 0, m_imgWidth, m_imgHeight), 0, 0, m_imgWidth, m_imgHeight, UnitPixel);

	// handle disabled state
	if(m_bIsDisabled)
	{
		graphics.DrawImage(m_pIamge, Rect(0, 0, m_imgWidth, m_imgHeight), m_imgWidth*3, 0, m_imgWidth, m_imgHeight, UnitPixel);
		return;
	}

	BOOL bIsPressed = (lpDIS->itemState & ODS_SELECTED);
	if (m_bPushed) 
	{
		graphics.DrawImage(m_pIamge, Rect(0, 0, m_imgWidth, m_imgHeight), m_imgWidth*2, 0, m_imgWidth, m_imgHeight, UnitPixel);
	}
	else 
	{
		if(bIsPressed)
		{
			graphics.DrawImage(m_pIamge, Rect(0, 0, m_imgWidth, m_imgHeight), m_imgWidth*2, 0, m_imgWidth, m_imgHeight, UnitPixel);
		}
		else if(m_bIsHovering)
		{
			graphics.DrawImage(m_pIamge, Rect(0, 0, m_imgWidth, m_imgHeight), m_imgWidth, 0, m_imgWidth, m_imgHeight, UnitPixel);
		}
		else
		{
			graphics.DrawImage(m_pIamge, Rect(0, 0, m_imgWidth, m_imgHeight), 0, 0, m_imgWidth, m_imgHeight, UnitPixel);
		}
	}

	// paint the button
	PaintText(pDC);
	return;
}

LRESULT CImgRioButton::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
	m_bIsHovering = true;
	Invalidate();
	return 1;
}

LRESULT CImgRioButton::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	Invalidate();
	return 1;
}

void CImgRioButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bIsTracking = !!_TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}

void CImgRioButton::PaintText(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	CRect textRect(rect.left-3, rect.top, rect.right, rect.bottom);


	CString str;
	GetWindowText(str);
	CString prefix = _T(" ");

	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_font);
	COLORREF oldTextClr = pDC->SetTextColor(m_TextClr);
	pDC->DrawText(prefix + str, &textRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(oldTextClr);
	pDC->SelectObject(pOldFont);
//	pDC->SetBkColor(RGB(28,0,14));
//	pDC->SetBkMode(oldBkMode);
}