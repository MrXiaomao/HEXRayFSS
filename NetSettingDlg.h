#pragma once


// NetSettingDlg 对话框

class NetSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(NetSettingDlg)

public:
	NetSettingDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~NetSettingDlg();
	virtual BOOL OnInitDialog();	
	
	//监测各个输入控件是否改动。
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETSETTING_DLG };
#endif

private:
	//探测器IP
	CString StrIP_Detector;
	//探测器端口号
	int Port_Detector;
	BOOL isDataChange;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void SaveNetSetting();
};
