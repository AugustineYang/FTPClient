
// FTPClientDlg.h: 头文件
//

#pragma once


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
	CEdit Account;
	CEdit Password;
	CListBox ListBox;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedUpload();
	afx_msg void OnBnClickedDownload();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult);

	void OnConnect(CString, CString, CString);
	void OnRefresh();
	void OnUpload();
	void OnDownload();
	void OnDelete();
};
