
// FTPClientDlg.h: 头文件
//

#pragma once

#define SUCCESSFUL 1
#define DISCONNECTED 2
#define FAILED 3
#define FAILED_TYPE_1 4
#define FAILED_TYPE_2 5
#define FAILED_TYPE_3 6
#define FAILED_TYPE_4 7
#define FAILED_TYPE_5 8
#define FAILED_TYPE_6 9
#define CANCELED 10

#define BUFFER_SIZE 1024

#define MEMSET(x) memset(x, 0, sizeof x)

#define SEND(x) send(x, sbuff, strlen(sbuff), 0)
#define RECV(x) recv(x, rbuff, sizeof rbuff, 0)

struct Server{
	unsigned short ServerPort;
	char* ServerAddr;
};

// CFTPClientDlg 对话框
class CFTPClientDlg : public CDialogEx
{
// 构造
public:
	CFTPClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTPCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton Connect;
	CButton Refresh;
	CButton Upload;
	CButton Download;
	CButton Delete;
	CEdit IPAddress;
	CEdit Port;
	CEdit Account;
	CEdit Password;
	CListBox ListBox;
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedUpload();
	afx_msg void OnBnClickedDownload();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult);

	short OnConnect(CString, CString, CString, CString);
	short OnRefresh();
	short OnUpload();
	short OnDownload();
	short OnDelete();

	Server GetPASVAddr(char*);

	bool connected; //全局变量，记录是否连接到服务器上
	SOCKET control_sock; //控制接口

	CProgressCtrl m_pro;
};
