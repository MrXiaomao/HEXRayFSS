
// HERayFSSDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "HERayFSS.h"
#include "HERayFSSDlg.h"
#include "afxdialogex.h"
#include "json/json.h"
#include "MyConst.h"

//VerQueryValue()函数需要添加本lib库, 或者右键项目 → 属性 → 链接器 → 输入 → 附加依赖项
#pragma comment(lib,"Version.lib")
#include "GitVerison.h"
#include <string>

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	CString GetFileVer();
	CString getGitVersion();
	CString m_strVersion; // 版本号

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
	m_strVersion = _T("123");
	m_strVersion = GetFileVer();
}

//获取软件当前的版本
//该版本号是在资源文件中的../Version/VS_VERSION_INFO/FILEVERSION的值
CString CAboutDlg::GetFileVer()
{
	TCHAR cPath[200];// szVersionBuffer[200];
	DWORD dwHandle, InfoSize;
	CString strVersion;
	::GetModuleFileName(NULL, cPath, 200); //首先获得版本信息资源的长度
	InfoSize = GetFileVersionInfoSize(cPath, &dwHandle); //将版本信息资源读入缓冲区
	if (InfoSize == 0) return _T("None VerSion Supprot");
	TCHAR* InfoBuf = new TCHAR[InfoSize];
	GetFileVersionInfo(cPath, 0, InfoSize, InfoBuf); //获得生成文件使用的代码页及文件版本
	unsigned int  cbTranslate = 0;
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;
	VerQueryValueW(InfoBuf, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate);
	// Read the file description for each language and code page.
	for (int i = 0; i < (cbTranslate / sizeof(struct LANGANDCODEPAGE)); i++)
	{
		TCHAR   SubBlock[200];
		wsprintf(SubBlock,
			TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
			lpTranslate[i].wLanguage,
			lpTranslate[i].wCodePage);
		void* lpBuffer = NULL;
		unsigned int dwBytes = 0;
		VerQueryValue(InfoBuf,
			SubBlock,
			&lpBuffer,
			&dwBytes);
		CString strTemp = (TCHAR*)lpBuffer;
		strVersion += strTemp;
	}
	return strVersion;
}

CString CAboutDlg::getGitVersion() {
	CString commitHash;
	string commitVer = GIT_VER;
	commitHash = commitVer.c_str();
	return commitHash;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//设置软件标题名称
	/*CString AppTitle = _T("垂直硬X射线相机阵列");//默认名称
	Json::Value jsonSetting = ReadSetting(_T("Setting.json"));
	if (!jsonSetting.isNull()) {
		if (jsonSetting.isMember("SoftwareTitle"))
		{
			// AppTitle = jsonSetting["SoftwareTitle"].asCString();
			const char* s = jsonSetting["SoftwareTitle"].asCString();
			int nLenW = ::MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
			wchar_t* wszBuffer = new wchar_t[nLenW];
			::MultiByteToWideChar(CP_UTF8, 0, s, -1, wszBuffer, nLenW);

			// 将 Unicode 编码转换为 GB2312 编码（也就是简体中文编码）
			int nLenA = ::WideCharToMultiByte(CP_ACP, 0, wszBuffer, -1, NULL, 0, NULL, NULL);
			char* szBuffer = new char[nLenA];
			::WideCharToMultiByte(CP_ACP, 0, wszBuffer, -1, szBuffer, nLenA, NULL, NULL);

			// 输出结果
			std::string strResult(szBuffer);
			const char* tmp = strResult.c_str();
			AppTitle = tmp;
		}
		else {
			string pStrTitle = _UnicodeToUtf8(AppTitle);
			// char* pStrTitle = CstringToWideCharArry(AppTitle);
			jsonSetting["SoftwareTitle"] = pStrTitle;
		}
	}
	WriteSetting(_T("Setting.json"), jsonSetting);
	*/
	// 将版本号设置为静态文本控件的文本
	GetDlgItem(IDC_STATIC_VERSION)->SetWindowText(m_strVersion);
	CString commitHash;
	string commitVer = GIT_VER;
	commitHash = commitVer.c_str();
	GetDlgItem(IDC_STATIC_COMMIT)->SetWindowText(commitHash);
	return TRUE;
}

// CHERayFSSDlg 对话框
CHERayFSSDlg::CHERayFSSDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_HERAYFSS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHERayFSSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHERayFSSDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32776, &CHERayFSSDlg::OnAbout)
END_MESSAGE_MAP()


// CHERayFSSDlg 消息处理程序

BOOL CHERayFSSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu menu; //菜单栏
	menu.LoadMenu(IDR_MENU1);  //IDR_MENU1为菜单栏ID号  
	SetMenu(&menu);

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

	//设置软件标题名称
	CString AppTitle = _T("HEXRay");//默认名称
	Json::Value jsonSetting = ReadSetting(_T("Setting.json"));
	if (!jsonSetting.isNull()) {
		if (jsonSetting.isMember("SoftwareTitle"))
		{
			// AppTitle = jsonSetting["SoftwareTitle"].asCString();
			const char* s = jsonSetting["SoftwareTitle"].asCString();
			int nLenW = ::MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
			wchar_t* wszBuffer = new wchar_t[nLenW];
			::MultiByteToWideChar(CP_UTF8, 0, s, -1, wszBuffer, nLenW);

			// 将 Unicode 编码转换为 GB2312 编码（也就是简体中文编码）
			int nLenA = ::WideCharToMultiByte(CP_ACP, 0, wszBuffer, -1, NULL, 0, NULL, NULL);
			char* szBuffer = new char[nLenA];
			::WideCharToMultiByte(CP_ACP, 0, wszBuffer, -1, szBuffer, nLenA, NULL, NULL);

			// 输出结果
			std::string strResult(szBuffer);
			const char* tmp = strResult.c_str();
			AppTitle = tmp;
		}
		else {
			string pStrTitle = _UnicodeToUtf8(AppTitle);
			// char* pStrTitle = CstringToWideCharArry(AppTitle);
			jsonSetting["SoftwareTitle"] = pStrTitle;
		}
	}
	WriteSetting(_T("Setting.json"), jsonSetting);
	SetWindowText(AppTitle);

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHERayFSSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHERayFSSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHERayFSSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHERayFSSDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
