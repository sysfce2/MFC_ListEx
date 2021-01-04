/****************************************************************************************
* Copyright © 2018-2021 Jovibor https://github.com/jovibor/                             *
* This is very extended and featured version of CMFCListCtrl class.                     *
* Official git repository: https://github.com/jovibor/ListEx/                           *
* This class is available under the "MIT License".                                      *
****************************************************************************************/
#pragma once
#include <unordered_map>

namespace LISTEX { struct LISTEXCOLORS; } //Forward declaration.

namespace LISTEX::INTERNAL
{
	/********************************************
	* CListExHdr class declaration.             *
	********************************************/
	class CListExHdr final : public CMFCHeaderCtrl
	{
		struct SHDRCOLOR;
		struct SHDRICON;
	public:
		explicit CListExHdr();
		~CListExHdr();
		void SetHeight(DWORD dwHeight);
		void SetFont(const LOGFONTW* pLogFontNew);
		void SetColor(const LISTEXCOLORS& lcs);
		void SetColumnColor(int iColumn, COLORREF clrBk, COLORREF clrText);
		void SetColumnIcon(int iColumn, int iIconIndex, bool fClick);
		void SetSortable(bool fSortable);
		void SetSortArrow(int iColumn, bool fAscending);
	private:
		[[nodiscard]] SHDRICON* HasIcon(int iColumn);
	protected:
		afx_msg void OnDrawItem(CDC* pDC, int iItem, CRect rcOrig, BOOL bIsPressed, BOOL bIsHighlighted)override;
		afx_msg LRESULT OnLayout(WPARAM wParam, LPARAM lParam);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnDestroy();
		DECLARE_MESSAGE_MAP()
	private:
		CFont m_fontHdr;
		CPen m_penGrid;
		CPen m_penLight;
		CPen m_penShadow;
		COLORREF m_clrBkNWA { }; //Bk of non working area.
		COLORREF m_clrText { };
		COLORREF m_clrBk { };
		COLORREF m_clrHglInactive { };
		COLORREF m_clrHglActive { };
		DWORD m_dwHeaderHeight { 19 }; //Standard (default) height.
		std::unordered_map<int, SHDRCOLOR> m_umapColors { }; //Colors for columns.
		std::unordered_map<int, SHDRICON> m_umapIcons { };   //Icons for columns.
		int m_iSortColumn { -1 };   //Column to draw sorting triangle at. -1 is to avoid triangle before first clicking.
		bool m_fSortable { false }; //Need to draw sortable triangle or not?
		bool m_fSortAscending { };  //Sorting type.
	};
}