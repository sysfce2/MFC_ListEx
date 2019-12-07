## **List control for MFC applications**
## Table of Contents
* [Introduction](#introduction)
* [Long Story Short](#long-story-short)
* [Usage](#usage)
* [Create](#create)
    * [Manually](#manually)
    * [In Dialog](#in-dialog)
* [Tooltips](#tooltips)
* [Menu](#menu)
    * [Menu Example](#menu-example)
    * [Handle Menu Clicks](#handle-menu-clicks)
* [Public Methods](#public-methods)
* [Example](#example)
* [Appearance](#appearance)

## [](#)Introduction
When I faced a necessity in **List control** with the ability to display tooltips on arbitrary cells, I was discouraged with the fact that such obvious feature simply doesn’t exist in standard **MFC CListCtrl** class. That was the main reason behind the creation of this control.

![](docs/img/listex_mainwndtooltip.jpg)

## [](#)Long Story Short
`IListEx` is a `CMFCListCtrl` derived class with cells tooltip ability, and even more.  
It works with `LVS_OWNERDRAWFIXED` and `LVS_REPORT` styles.<br>
The tooltiping is achieved with the `public` method:

```cpp
SetCellTooltip(int iItem, int iSubitem, const wchar_t* pwszTooltip, const wchar_t* pwszCaption = nullptr)
```
With this method you can set a tooltip for any cell (item/subitem) in the list, and it will show up on mouse hovering.<br>
Tooltips are implemented as Windows balloons.

To remove tooltip from a cell just set it again with empty (`nullptr`) string.<br>
Control was built and tested in **Visual Studio 2019**.

## [](#)Usage
The usage of the control is quite simple:
1. Copy *ListEx* folder into your project's folder.
2. Add all files from *ListEx* folder into your project.
3. Add `#include "ListEx/ListEx.h"` where you suppose to use the control.
4. Declare `IListExPtr` variable: `IListExPtr myList { CreateListEx() };`

`IListExPtr` is, in fact, a pointer to the `IListEx` pure abstract base class, wrapped either in `std::unique_ptr` or `std::shared_ptr`. You can choose whatever is best for you by comment/uncomment one of this alliases in `ListEx.h`:
```cpp
	//using IListExPtr = IListExUnPtr;
	using IListExPtr = IListExShPtr;
```
This wrapper is used mainly for convenience, so you don't have to bother about object lifetime, it will be destroyed automatically.
That's why there is a call to the factory function `CreateListEx()`, to properly initialize a pointer.<br>

Control uses its own namespace `LISTEX`. So it's up to you, whether to use namespace prefix before declarations: 
```cpp
LISTEX::
```
or to define namespace in the source file's beginning:
```cpp
using namespace LISTEX;
```

## [](#)Create
### [](#)Manually
`Create` is the main method to create list control. It takes one argument - reference to the `LISTEXCREATESTRUCT` structure.
The `LISTEXCREATESTRUCT` is a helper structure which fields are described below. This struct is also used in `SetColor` method.

```cpp
struct LISTEXCREATESTRUCT {
    LISTEXCOLORSTRUCT stColor { };           //All control's colors.
    CRect             rect;                  //Initial rect.
    CWnd*             pwndParent { };        //Parent window.
    const LOGFONTW*   pListLogFont { };      //List font.
    const LOGFONTW*   pHdrLogFont { };       //Header font.
    DWORD             dwStyle { };           //Control's styles. Zero for default.
    UINT              uID { };               //Control Id.
    DWORD             dwListGridWidth { 1 }; //Width of the list grid.
    DWORD             dwHdrHeight { 20 };    //Header height.
    bool              fSortable { false };   //Is list sortable, by clicking on the header column?
    bool              fDialogCtrl { false }; //If it's a list within dialog.
};

```
`stColor` is a member of the `LISTEXCOLORSTRUCT` structure which fields are described below:
```cpp
struct LISTEXCOLORSTRUCT
{
    COLORREF clrListText { GetSysColor(COLOR_WINDOWTEXT) };				//List text color.
    COLORREF clrListBkRow1 { GetSysColor(COLOR_WINDOW) };				//List Bk color of the odd rows.
    COLORREF clrListBkRow2 { GetSysColor(COLOR_WINDOW) };				//List Bk color of the even rows.
    COLORREF clrListGrid { RGB(220, 220, 220) };						//List grid color.
    COLORREF clrListTextSelected { GetSysColor(COLOR_HIGHLIGHTTEXT) };	//Selected item text color.
    COLORREF clrListBkSelected { GetSysColor(COLOR_HIGHLIGHT) };		//Selected item bk color.
    COLORREF clrTooltipText { GetSysColor(COLOR_INFOTEXT) };			//Tooltip window text color.
    COLORREF clrTooltipBk { GetSysColor(COLOR_INFOBK) };				//Tooltip window bk color.
    COLORREF clrListTextCellTt { GetSysColor(COLOR_WINDOWTEXT) };		//Text color of a cell that has tooltip.
    COLORREF clrListBkCellTt { RGB(170, 170, 230) };					//Bk color of a cell that has tooltip.
    COLORREF clrHeaderText { GetSysColor(COLOR_WINDOWTEXT) };			//List header text color.
    COLORREF clrHeaderBk { GetSysColor(COLOR_WINDOW) };					//List header bk color.
    COLORREF clrBkNWA { GetSysColor(COLOR_WINDOW) };					//Bk of non working area.
};
```
Below is a simple example of the **control**'s creation:
```cpp
IListExPtr myList { CreateListEx() };
.
.
LISTEXCREATESTRUCT lcs;
lcs.pwndParent = this;
lcs.nID = ID_MY_LIST;
lcs.rect = CRect(0, 0, 500, 300);

myList->Create(lcs);
```
With `LISTEXCREATESTRUCT` structure you can adjust a plethora of list’s aspects:

* Color of the list text and bk (background). Bk is set separately for odd and even rows
* Color of the list header
* Height of the list header
* Font of the list header, and font of list itself
* Color of individual header's columns
* Color of tooltip's window text and bk
* Color of the text and bk of a cell that has tooltip
* Color of list grid, and even its width
* Color of row when it's selected

### [](#)In Dialog
To create the control in a *Dialog* you can manually do it with the [Create](#manually) method.

But most of the times you prefer to place a standard *List Control* onto the *Dialog*'s template, by dragging it from the **Toolbox** within **Visual studio**.  
To use the latter approach follow these steps:
1. Put standard *List Control* from the toolbox onto your dialog template. Give it appropriate ID (`IDC_LISTEX`) and make it desirable size.
2. Declare `IListExPtr` member varable within your dialog class: `IListExPtr m_myList { CreateListEx() };`
3. In your `OnInitDialog` method call `m_myList->CreateDialogCtrl(IDC_LISTEX, this);` function.

## [](#)Tooltips
To set a tooltip for a given cell, just write:

```cpp
myList->SetCellTooltip(0, 1, L"Tooltip text", L"Tooltip caption:");
```
This will set a tooltip for cell (0, 1) with the text: **_Tooltip text_**, and the caption **_Tooltip caption_**.

## [](#)Menu
`IListEx` class possesses innate ability to set popup menu for individual cells, as well as for the whole list.

![](docs/img/listex_mainwndmenu.jpg)

This is achieved with the help of two public methods:
```cpp
SetListMenu(CMenu* pMenu);
SetCellMenu(int iItem, int iSubitem, CMenu* pMenu);
```
### [](#)Menu Example
With the code below, we are going to set two separate menus:

* For the whole list - `CMenu m_menuList;`<br>
* For the individual cell - `CMenu m_menuCell;`
```cpp
//MyDlg.h
#include "ListEx/ListEx.h"

using namespace LISTEX;

constexpr auto IDC_LIST_MENU_CELL_FIRST = 0x1;
constexpr auto IDC_LIST_MENU_CELL_SECOND = 0x2;
constexpr auto IDC_LIST_MENU_GLOBAL_FIRST = 0x3;
constexpr auto IDC_LIST_MENU_GLOBAL_SECOND = 0x4;

class CMyDlg : public CDialogEx
{
private:
  CMenu m_menuCell;
  CMenu m_menuList;
  IListExPtr m_myList { CreateListEx() };
  .
  .
}


//MyDlg.cpp
CMyDlg::OnInitDialog()
{
  m_myList->CreateDialogCtrl();
  m_myList->InsertColumn(0, L"First column", 0, 100);
  m_myList->InsertColumn(...);
  m_myList->InsertItem(...);. 
  m_myList->SetItemText(...);

  m_menuCell.CreatePopupMenu();
  m_menuCell.AppendMenuW(MF_STRING, IDC_LIST_MENU_CELL_FIRST, L"Cell's first menu...");
  m_menuCell.AppendMenuW(MF_STRING, IDC_LIST_MENU_CELL_SECOND, L"Cell's second menu...");

  m_menuList.CreatePopupMenu();
  m_menuList.AppendMenuW(MF_STRING, IDC_LIST_MENU_GLOBAL_FIRST, L"List's first menu...");
  m_menuList.AppendMenuW(MF_STRING, IDC_LIST_MENU_GLOBAL_SECOND, L"List's second menu...");

  m_myList->SetListMenu(&m_menuList);
  m_myList->SetCellMenu(1, 0, &m_menuCell); //Set menu for row:1 column:0.
}
```
### [](#)Handle Menu Clicks
When user clicks a menu `IListEx` sends `WM_NOTIFY` message, with `NMITEMACTIVATE` struct pointer as `lParam`, to its parent window. So, in order to properly handle clicks you have to process this message in your list's parent window:

```cpp
BOOL CMyDialog::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
   	const LPNMITEMACTIVATE pNMI = reinterpret_cast<LPNMITEMACTIVATE>(lParam);

	if (pNMI->hdr.idFrom == IDC_LISTEX)
	{
		if (pNMI->hdr.code == LISTEX_MSG_MENUSELECTED)
		{
			CString ss;
			switch (pNMI->lParam)
			{
			case IDC_LIST_MENU_CELL_FIRST:
				ss.Format(L"Cell's first menu clicked. Row: %i, Column: %i", pNMI->iItem, pNMI->iSubItem);
				break;
			case IDC_LIST_MENU_CELL_SECOND:
				ss.Format(L"Cell's second menu clicked. Row: %i, Column: %i", pNMI->iItem, pNMI->iSubItem);
				break;
			case IDC_LIST_MENU_GLOBAL_FIRST:
				ss.Format(L"List's first menu clicked. Row: %i, Column: %i", pNMI->iItem, pNMI->iSubItem);
				break;
			case IDC_LIST_MENU_GLOBAL_SECOND:
				ss.Format(L"List's second menu clicked. Row: %i, Column: %i", pNMI->iItem, pNMI->iSubItem);
				break;
			}
			MessageBoxW(ss);
		}
	}

	return CDialogEx::OnNotify(wParam, lParam, pResult);
}
```
`IListEx` fills `NMITEMACTIVATE` struct with `NMITEMACTIVATE::hdr.code` equals `LISTEX_MSG_MENUSELECTED`. And `menuId` is stored as `NMITEMACTIVATE::lParam`.

`NMITEMACTIVATE::iItem` and `NMITEMACTIVATE::iSubItem` both point to a cell the menu was clicked on.

## [](#)Public Methods
`IListEx` class also has a set of additional public methods to help customize your control in many different aspects.
```cpp
bool Create(const LISTEXCREATESTRUCT& lcs);
void CreateDialogCtrl(UINT uCtrlID, CWnd* pwndDlg);
void Destroy();
DWORD_PTR GetCellData(int iItem, int iSubitem);
UINT GetFontSize();
int GetSortColumn();
bool GetSortAscending();
bool IsCreated();
void SetCellColor(int iItem, int iSubitem, COLORREF clr);
void SetCellData(int iItem, int iSubitem, DWORD_PTR dwData);
void SetCellMenu(int iItem, int iSubitem, CMenu* pMenu);
void SetCellTooltip(int iItem, int iSubitem, const wchar_t* pwszTooltip, const wchar_t* pwszCaption = nullptr);
void SetColor(const LISTEXCOLORSTRUCT& lcs);
void SetFont(const LOGFONTW* pLogFontNew);
void SetFontSize(UINT uiSize);
void SetHeaderHeight(DWORD dwHeight);
void SetHeaderFont(const LOGFONT* pLogFontNew);
void SetHeaderColumnColor(DWORD nColumn, COLORREF clr);
void SetListMenu(CMenu* pMenu);
void SetSortable(bool fSortable);
void SetSortFunc(int (CALLBACK *pfCompareFunc)(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort));
```
## [](#)Example
Let’s imagine that you need a list control with a non standard header height, and yellow background color.
Nothing is simpler, see code below:
```cpp
LISTEXCREATESTRUCT lcs;
lcs.rect = CRect(0, 0, 500, 300)
lcs.pwndParent = this;
lcs.dwHeaderHeight = 50;
lcs.stColor.clrListBkRow1 = RGB(255, 255, 0);
lcs.stColor.clrListBkRow2 = RGB(255, 255, 0);

IListExPtr myList { CreateListEx() };
myList->Create(lcs);

myList->InsertColumn(...);
myList->InsertItem(...);
```
Here, we set both - even and odd rows (`clrListBkRow1` and `clrListBkRow2`) to the same yellow color.

## [](#)Appearance
With the **Ctrl+MouseWheel** combination you can dynamically change list's font size.