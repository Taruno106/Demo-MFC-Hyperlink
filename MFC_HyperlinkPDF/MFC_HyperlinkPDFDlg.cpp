
// MFC_HyperlinkPDFDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC_HyperlinkPDF.h"
#include "MFC_HyperlinkPDFDlg.h"
#include "afxdialogex.h"
#include "hpdf.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCHyperlinkPDFDlg dialog



CMFCHyperlinkPDFDlg::CMFCHyperlinkPDFDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_HYPERLINKPDF_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCHyperlinkPDFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LINK, m_strHyperlink);
}

BEGIN_MESSAGE_MAP(CMFCHyperlinkPDFDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CMFCHyperlinkPDFDlg::OnBnClickedBtnExport)
END_MESSAGE_MAP()


// CMFCHyperlinkPDFDlg message handlers

BOOL CMFCHyperlinkPDFDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCHyperlinkPDFDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCHyperlinkPDFDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCHyperlinkPDFDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void ErrorHandler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void* user_data) {
	CString msg;
	msg.Format(_T("LibHaru Error:\nError code = 0x%04X\nDetail code = 0x%04X"), error_no, detail_no);
	AfxMessageBox(msg);
}

bool ValidateUrl(const CString& url)
{
	CString trimmed = url;
	trimmed.Trim();

	const bool isHttp = trimmed.Left(7).CompareNoCase(_T("http://")) == 0;
	const bool isHttps = trimmed.Left(8).CompareNoCase(_T("https://")) == 0;

	return !trimmed.IsEmpty() && (isHttp || isHttps);
}

std::string ConvertCStringToUtf8(const CString& str)
{
	CStringW wideStr(str);
	CStringA utf8Str(wideStr);
	return std::string(utf8Str.GetString());
}

bool ExportToPDF(const std::string& linkText)
{
	HPDF_Doc pdf = HPDF_New(ErrorHandler, NULL);
	if (!pdf)
		return false;

	HPDF_Page page = HPDF_AddPage(pdf);
	HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

	// Original position to print hyperlink
	const float x = 50;
	const float y = 750;
	const float fontSize = 14;

	HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
	HPDF_Page_SetFontAndSize(page, font, fontSize);

	HPDF_Page_SetRGBFill(page, 0, 0, 1); // set text to blue color

	// Now it's safe to get text width
	float textWidth = HPDF_Page_TextWidth(page, linkText.c_str());
	float height = fontSize + 2;

	// Draw text
	HPDF_Page_BeginText(page);
	HPDF_Page_TextOut(page, x, y, linkText.c_str());
	HPDF_Page_EndText(page);

	// Create clickable link annotation
	HPDF_Rect rect = { x - 5, y - 5, x + textWidth + 5, y + height };
	HPDF_Page_CreateURILinkAnnot(page, rect, linkText.c_str());

	HPDF_STATUS ret = HPDF_SaveToFile(pdf, "output.pdf");
	HPDF_Free(pdf);

	return (ret == HPDF_OK);
}

void CMFCHyperlinkPDFDlg::OnBnClickedBtnExport()
{
	UpdateData(TRUE); // Get data from UI

	if (!ValidateUrl(m_strHyperlink)) {
		AfxMessageBox(_T("Please enter a valid URL (e.g., https://www.google.com)"));
		return;
	}

	std::string utf8Link = ConvertCStringToUtf8(m_strHyperlink);

	try {
		if (!ExportToPDF(utf8Link)) {
			AfxMessageBox(_T("Failed to save PDF!"));
			return;
		}
		
		TCHAR buffer[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, buffer);

		CString outputPath(buffer);  // convert to CString
		outputPath += _T("\\output.pdf");

		CString msg;
		msg.Format(_T("Exported successfully!\nFile saved at:\n%s"), outputPath);
		AfxMessageBox(msg);
	}
	catch (...) {
		AfxMessageBox(_T("Exception occurred. Cannot create PDF."));
	}
}


