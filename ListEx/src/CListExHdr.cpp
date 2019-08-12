/********************************************************************************
* Copyright (C) 2018-2019, Jovibor: https://github.com/jovibor/					*
* Github repository URL: https://github.com/jovibor/ListEx						*
* This software is available under the "MIT License".							*
* This is an extended and featured version of CMFCListCtrl class.				*
* CListEx - list control class with the ability to set tooltips on arbitrary	*
* cells, and also with a lots of other stuff to customize your control in many	*
* different aspects. For more info see official documentation on github.		*
********************************************************************************/
#include "pch.h"
#include "CListExHdr.h"

using namespace LISTEX;

/****************************************************
* CListExHdr class implementation.					*
****************************************************/
BEGIN_MESSAGE_MAP(CListExHdr, CMFCHeaderCtrl)
	ON_MESSAGE(HDM_LAYOUT, &CListExHdr::OnLayout)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

CListExHdr::CListExHdr()
{
	NONCLIENTMETRICSW ncm { };
	ncm.cbSize = sizeof(NONCLIENTMETRICSW);
	SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
	ncm.lfMessageFont.lfHeight = 18; //For some weird reason above func returns this value as MAX_LONG.

	m_fontHdr.CreateFontIndirectW(&ncm.lfMessageFont);

	m_hdItem.mask = HDI_TEXT;
	m_hdItem.cchTextMax = MAX_PATH;
	m_hdItem.pszText = m_wstrHeaderText;
}

void CListExHdr::OnDrawItem(CDC* pDC, int iItem, CRect rect, BOOL /*bIsPressed*/, BOOL /*bIsHighlighted*/)
{
	if (iItem < 0) { //Non working area, after last column.
		pDC->FillSolidRect(&rect, m_clrBkNWA);
		return;
	}

	CMemDC memDC(*pDC, rect);
	CDC& rDC = memDC.GetDC();

	if (m_umapClrColumn.find(iItem) != m_umapClrColumn.end())
		rDC.FillSolidRect(&rect, m_umapClrColumn[iItem]);
	else
		rDC.FillSolidRect(&rect, m_clrBk);
	rDC.SetTextColor(m_clrText);
	rDC.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
	rDC.SelectObject(&m_fontHdr);

	//Set item's text buffer first char to zero,
	//then getting item's text and Draw it.
	m_wstrHeaderText[0] = L'\0';
	GetItem(iItem, &m_hdItem);

	if (StrStrW(m_wstrHeaderText, L"\n"))
	{	//If it's multiline text, first � calculate rect for the text,
		//with CALC_RECT flag (not drawing anything),
		//and then calculate rect for final vertical text alignment.
		CRect rcText;
		rDC.DrawTextW(m_wstrHeaderText, &rcText, DT_CENTER | DT_CALCRECT);
		rect.top = rect.Height() / 2 - rcText.Height() / 2;
		rDC.DrawTextW(m_wstrHeaderText, &rect, DT_CENTER);
	}
	else
		rDC.DrawTextW(m_wstrHeaderText, &rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

LRESULT CListExHdr::OnLayout(WPARAM /*wParam*/, LPARAM lParam)
{
	CMFCHeaderCtrl::DefWindowProcW(HDM_LAYOUT, 0, lParam);

	LPHDLAYOUT pHDL = reinterpret_cast<LPHDLAYOUT>(lParam);

	//New header height.
	pHDL->pwpos->cy = m_dwHeaderHeight;
	//Decreasing list's height begining by the new header's height.
	pHDL->prc->top = m_dwHeaderHeight;

	return 0;
}

void CListExHdr::SetHeight(DWORD dwHeight)
{
	m_dwHeaderHeight = dwHeight;
}

void CListExHdr::SetColor(COLORREF clrText, COLORREF clrBk)
{
	m_clrText = clrText;
	m_clrBk = clrBk;

	RedrawWindow();
}

void CListExHdr::SetColumnColor(DWORD dwColumn, COLORREF clr)
{
	m_umapClrColumn[dwColumn] = clr;
	RedrawWindow();
}

void CListExHdr::SetFont(const LOGFONTW * pLogFontNew)
{
	if (!pLogFontNew)
		return;

	m_fontHdr.DeleteObject();
	m_fontHdr.CreateFontIndirectW(pLogFontNew);

	//If new font's height is higher than current height (m_dwHeaderHeight)
	//we adjust current height as well.
	TEXTMETRIC tm;
	CDC* pDC = GetDC();
	pDC->SelectObject(&m_fontHdr);
	GetTextMetricsW(pDC->m_hDC, &tm);
	ReleaseDC(pDC);
	DWORD dwHeightFont = tm.tmHeight + tm.tmExternalLeading + 1;
	if (dwHeightFont > m_dwHeaderHeight)
		SetHeight(dwHeightFont);
}