
// MFC_HyperlinkPDFDlg.h : header file
//


#pragma once


// CMFCHyperlinkPDFDlg dialog
class CMFCHyperlinkPDFDlg : public CDialogEx
{
// Construction
public:
	CMFCHyperlinkPDFDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
	CString m_strHyperlink;


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_HYPERLINKPDF_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnExport();
	

};
