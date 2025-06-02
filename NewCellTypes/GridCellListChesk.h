#pragma once
#include "..\GridCell.h"

class CGridCellListChesk : public CGridCell
{
	friend class CGridCtrl;
	DECLARE_DYNCREATE(CGridCellListChesk)

public:
	CGridCellListChesk();

	// editing cells
public:
	virtual BOOL  Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	virtual CWnd* GetEditWnd() const;
	virtual void  EndEdit();

	// Operations
public:
	virtual CSize GetCellExtent(CDC* pDC);

	// CGridCellCombo specific calls
public:
	void  SetOptions(const CStringArray& ar);
	void  SetStyle(DWORD dwStyle)           { m_dwStyle = dwStyle; }
	DWORD GetStyle()                        { return m_dwStyle;    }

protected:
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

	CStringArray m_Strings;
	DWORD        m_dwStyle;
};


#define IDC_LISTEDIT 1002

/*class CCellListCheskoEdit : public CEdit
{
	// Construction
public:
	CCellListCheskoEdit();

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCellListCheskoEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CCellListCheskoEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCellListCheskoEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};*/
class CInPlaceListCheck : public CCheckListBox 
{
//	friend class CInPlaceList;

	// Construction
public:
	CInPlaceListCheck(CWnd* pParent,         // parent
		CRect& rect,           // dimensions & location
		DWORD dwStyle,         // window/combobox style
		UINT nID,              // control ID
		int nRow, int nColumn, // row and column
		COLORREF crFore, COLORREF crBack,  // Foreground, background colour
		CStringArray& Items,   // Items in list
		CString sInitText,     // initial selection
		UINT nFirstChar);      // first character to pass to control

	// Attributes
public:
//	CCellListCheskoEdit m_edit;  // subclassed edit control

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceListCheck)
protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CInPlaceListCheck();
	void EndEdit();

protected:
	int GetCorrectDropWidth();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceList)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDropdown();
	afx_msg UINT OnGetDlgCode();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	//afx_msg void OnSelendOK();

	DECLARE_MESSAGE_MAP()

private:
	int		 m_nNumLines;
	CString  m_sInitText;
	int		 m_nRow;
	int		 m_nCol;
	UINT     m_nLastChar; 
	BOOL	 m_bExitOnArrows; 
	COLORREF m_crForeClr, m_crBackClr;
};
