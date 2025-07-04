// NetSettingDlg.cpp: 实现文件
//

#include "pch.h"
#include "HEXRayFSS.h"
#include "NetSettingDlg.h"
#include "afxdialogex.h"

#include "MyConst.h"
// NetSettingDlg 对话框

IMPLEMENT_DYNAMIC(NetSettingDlg, CDialog)

NetSettingDlg::NetSettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_NETSETTING_DLG, pParent),
	StrIP_Detector(_T("192.168.10.22")),
	Port_Detector(5000),
	isDataChange(false)
{

}

NetSettingDlg::~NetSettingDlg()
{
	//保存界面参数
	Json::Value jsonSetting = ReadSetting(_T("Setting.json"));
	char* pStrIP = CstringToWideCharArry(StrIP_Detector);
	jsonSetting["IP_Detector"] = pStrIP;
	jsonSetting["Port_Detector"] = Port_Detector;
	WriteSetting(_T("Setting.json"), jsonSetting);
}

void NetSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IPADDRESS1, StrIP_Detector);
	DDX_Text(pDX, IDC_PORT, Port_Detector);
	DDV_MinMaxInt(pDX, IDC_PORT, 0, 65535);
}


BEGIN_MESSAGE_MAP(NetSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_NETSETTING_OK, &NetSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_NETSETTING_CANCEL, &NetSettingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_NETSETTING_APPLY, &NetSettingDlg::SaveNetSetting)
END_MESSAGE_MAP()


BOOL NetSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// ------------------读取配置参数并设置到相应控件上---------------------
	Json::Value jsonSetting = ReadSetting(_T("Setting.json"));
	if (!jsonSetting.isNull()) 
	{
		if (jsonSetting.isMember("IP_Detector")) {
			StrIP_Detector = jsonSetting["IP_Detector"].asCString();
		}
		if (jsonSetting.isMember("Port_Detector")) {
			Port_Detector = jsonSetting["Port_Detector"].asInt();
		}
	}

	UpdateData(FALSE);
	GetDlgItem(IDC_NETSETTING_APPLY)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}
// NetSettingDlg 消息处理程序


void NetSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (isDataChange) SaveNetSetting();
	CDialog::OnOK();
}


void NetSettingDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


// CNetSetting 消息处理程序
void NetSettingDlg::SaveNetSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	//将控件中的value给该控件的变量赋值。
	UpdateData(TRUE); 

	Json::Value jsonSetting = ReadSetting(_T("Setting.json"));
	if (!jsonSetting.isNull()) {
		// 写入配置文件
		Json::Value jsonSetting = ReadSetting(_T("Setting.json"));
		char* pStrIP1 = CstringToWideCharArry(StrIP_Detector);
		jsonSetting["IP_Detector"] = pStrIP1;
		jsonSetting["Port_Detector"] = Port_Detector;

		if (WriteSetting(_T("Setting.json"), jsonSetting) == 0) {
			GetDlgItem(IDC_NETSETTING_APPLY)->EnableWindow(FALSE);
		}
	}
}


BOOL NetSettingDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	int wmEVENT = HIWORD(wParam);
	if (wmEVENT == EN_CHANGE)
	{
		//IP控件
		int wmID = LOWORD(wParam);
		if (wmID == IDC_IPADDRESS1) {
			CString StrIP_Det;
			GetDlgItemText(IDC_IPADDRESS1, StrIP_Det);
			if (StrIP_Det != StrIP_Detector) {
				isDataChange = TRUE;
			}
		}
		
		//Port控件
		if (wmID == IDC_PORT) {
			CString strText;
			GetDlgItemText(IDC_PORT, strText);
			int newValue = _ttoi(strText);  // _ttoi 支持 Unicode/ANSI
			if (newValue != Port_Detector) {
				isDataChange = TRUE;
			}
		}

		// 若控件发生了编辑动作，则激活“应用”按钮
		if (isDataChange) GetDlgItem(IDC_NETSETTING_APPLY)->EnableWindow(TRUE);
	}
	return CDialog::OnCommand(wParam, lParam);
}