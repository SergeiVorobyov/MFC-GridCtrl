#include "stdafx.h"
#include "../GridCtrl_src/GridCell.h"
#include "../GridCtrl_src/GridCtrl.h"

#include "GridCellListChesk.h"


/////////////////////////////////////////////////////////////////////////////


CInPlaceListCheck::CInPlaceListCheck(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                           int nRow, int nColumn, 
                           COLORREF crFore, COLORREF crBack,
						   CStringArray& Items, CString sInitText, 
						   UINT nFirstChar)
{
    m_crForeClr = crFore;
    m_crBackClr = crBack;

	m_nNumLines = Items.GetSize();;//4;
	m_sInitText = sInitText;
 	m_nRow		= nRow;
 	m_nCol      = nColumn;
 	m_nLastChar = 0; 
	m_bExitOnArrows = FALSE; //(nFirstChar != VK_LBUTTON);	// If mouse click brought us here,

	// Create the combobox
 	DWORD dwComboStyle = WS_BORDER|WS_CHILD|WS_VISIBLE|WS_VSCROLL|
 					     CBS_AUTOHSCROLL | dwStyle;
	dwComboStyle = WS_CHILD | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | WS_BORDER | WS_VSCROLL |WS_VISIBLE;

	int nHeight = rect.Height();
	CFont *pFnt = pParent->GetFont();
	LOGFONT hLg;
	pFnt->GetLogFont(&hLg);
	nHeight = 	abs(hLg.lfHeight);


	rect.bottom = rect.bottom + m_nNumLines*nHeight + ::GetSystemMetrics(SM_CYHSCROLL);
	if (!Create(dwComboStyle, rect, pParent, nID)) return;

	// Add the strings
	for (int i = 0; i < Items.GetSize(); i++) 
	{
		int iItem = AddString(Items[i]);
		if(sInitText.Find(Items[i]) != -1)
		{
			SetCheck(i,1);
		}
	}

	SetFont(pParent->GetFont());
	SetItemHeight(-1, nHeight);

    int nMaxLength = GetCorrectDropWidth();
    
    if (nMaxLength > rect.Width())
	    rect.right = rect.left + nMaxLength;
	// Resize the edit window and the drop down window
	MoveWindow(rect);
    


	SetHorizontalExtent(0); // no horz scrolling

 	SetFocus();
}

CInPlaceListCheck::~CInPlaceListCheck()
{
}
void CInPlaceListCheck::EndEdit()
{
	CString str;
	str="";
	CString s;
	for(int i=0; i < GetCount();i++)
	{
		if(GetCheck(i))
		{
			GetText(i,s);
			str+=s;
			str+="\r\n";
		}
	}

	// Send Notification to parent
	GV_DISPINFO dispinfo;

	dispinfo.hdr.hwndFrom = GetSafeHwnd();
	dispinfo.hdr.idFrom   = GetDlgCtrlID();
	dispinfo.hdr.code     = GVN_ENDLABELEDIT;

	dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
	dispinfo.item.row     = m_nRow;
	dispinfo.item.col     = m_nCol;
	dispinfo.item.strText = str;
	dispinfo.item.lParam  = (LPARAM) m_nLastChar; 

	CWnd* pOwner = GetOwner();
	if (IsWindow(pOwner->GetSafeHwnd()))
		pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );

	// Close this window (PostNcDestroy will delete this)
	if (::IsWindow(m_hWnd))
		PostMessage(WM_CLOSE, 0, 0);
}

int CInPlaceListCheck::GetCorrectDropWidth()
{
	const int nMaxWidth = 300;  // don't let the box be bigger than this

	// Reset the dropped width
	int nNumEntries = GetCount();
	int nWidth = 0;
	CString str;

	CClientDC dc(this);
	int nSave = dc.SaveDC();
	dc.SelectObject(GetFont());

	int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
	for (int i = 0; i < nNumEntries; i++)
	{
		GetText(i, str);
		int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
		nWidth = max(nWidth, nLength);
	}

	// Add margin space to the calculations
	nWidth += dc.GetTextExtent(_T("0")).cx;

	dc.RestoreDC(nSave);

	nWidth = min(nWidth, nMaxWidth);

	return nWidth;
	//SetDroppedWidth(nWidth);
}
void CInPlaceListCheck::PostNcDestroy() 
{
	CCheckListBox::PostNcDestroy();

	delete this;
}
void CInPlaceListCheck::OnKillFocus(CWnd* pNewWnd) 
{
	CCheckListBox::OnKillFocus(pNewWnd);

	if (GetSafeHwnd() == pNewWnd->GetSafeHwnd())
		return;

	// Only end editing on change of focus if we're using the CBS_DROPDOWNLIST style
//	if ((GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
		EndEdit();
}
void CInPlaceListCheck::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar == VK_PRIOR || nChar == VK_NEXT ||
		nChar == VK_DOWN  || nChar == VK_UP   ||
		nChar == VK_RIGHT || nChar == VK_LEFT) &&
		(m_bExitOnArrows || GetKeyState(VK_CONTROL) < 0))
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();
		return;
	}

	CCheckListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

// Need to keep a lookout for Tabs, Esc and Returns.
void CInPlaceListCheck::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE) 
		SetWindowText(m_sInitText);	// restore previous text

	if (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE)
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();	// This will destroy this window
		return;
	}

	CCheckListBox::OnKeyUp(nChar, nRepCnt, nFlags);
}
UINT CInPlaceListCheck::OnGetDlgCode() 
{
	return DLGC_WANTALLKEYS;
}
HBRUSH CInPlaceListCheck::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/) 
{
    /*
    static CBrush brush(m_crBackClr);
    pDC->SetTextColor(m_crForeClr);
    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH) brush.GetSafeHandle();
    */
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

BEGIN_MESSAGE_MAP(CInPlaceListCheck, CCheckListBox)
	//{{AFX_MSG_MAP(CInPlaceListCheck)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	ON_WM_GETDLGCODE()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	//ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendOK)
END_MESSAGE_MAP()



IMPLEMENT_DYNCREATE(CGridCellListChesk, CGridCell)

CGridCellListChesk::CGridCellListChesk() : CGridCell()
{
	SetStyle(CBS_DROPDOWN);  // CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE, CBS_SORT
}
BOOL CGridCellListChesk::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
	m_bEditing = TRUE;

	// CInPlaceList auto-deletes itself
	m_pEditWnd = new CInPlaceListCheck(GetGrid(), rect, GetStyle(), nID, nRow, nCol, 
		GetTextClr(), GetBackClr(), m_Strings, GetText(), nChar);

	return TRUE;
}
CWnd* CGridCellListChesk::GetEditWnd() const
{

	return NULL;
}
CSize CGridCellListChesk::GetCellExtent(CDC* pDC)
{    
	CSize sizeScroll (GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CYHSCROLL));    
	CSize sizeCell (CGridCell::GetCellExtent(pDC));    
	sizeCell.cx += sizeScroll.cx;    
	sizeCell.cy = max(sizeCell.cy,sizeScroll.cy);    
	return sizeCell;
}
void CGridCellListChesk::EndEdit()
{
	if (m_pEditWnd)
		((CInPlaceListCheck*)m_pEditWnd)->EndEdit();
}
BOOL CGridCellListChesk::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
#ifdef _WIN32_WCE
	return CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);
#else
	// Cell selected?
	//if ( !IsFixed() && IsFocused())
	if (GetGrid()->IsCellEditable(nRow, nCol) && !IsEditing())
	{
		// Get the size of the scroll box
		CSize sizeScroll(GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CYHSCROLL));

		// enough room to draw?
		if (sizeScroll.cy < rect.Width() && sizeScroll.cy < rect.Height())
		{
			// Draw control at RHS of cell
			CRect ScrollRect = rect;
			ScrollRect.left   = rect.right - sizeScroll.cx;
			ScrollRect.bottom = rect.top + sizeScroll.cy;

			// Do the draw 
			pDC->DrawFrameControl(ScrollRect, DFC_SCROLL, DFCS_SCROLLDOWN);

			// Adjust the remaining space in the cell
			rect.right = ScrollRect.left;
		}
	}

	CString strTempText = GetText();
	if (IsEditing())
		SetText(_T(""));

	// drop through and complete the cell drawing using the base class' method
	BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);

	if (IsEditing())
		SetText(strTempText);

	return bResult;
#endif
}
void CGridCellListChesk::SetOptions(const CStringArray& ar)
{ 
	m_Strings.RemoveAll();
	for (int i = 0; i < ar.GetSize(); i++)
		m_Strings.Add(ar[i]);
}
