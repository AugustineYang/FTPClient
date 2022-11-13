
// FTPClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FTPClient.h"
#include "FTPClientDlg.h"
#include "afxdialogex.h"
#include "Winsock.h"
#include "windows.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include "io.h"

#pragma comment(lib, "wsock32.lib")
#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CFTPClientDlg 对话框



CFTPClientDlg::CFTPClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FTPCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	connected = false;
}

void CFTPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Connect,	Connect);
	DDX_Control(pDX, IDC_Refresh,	Refresh);
	DDX_Control(pDX, IDC_Upload,	Upload);
	DDX_Control(pDX, IDC_Download,	Download);
	DDX_Control(pDX, IDC_Delete,	Delete);
	DDX_Control(pDX, IDC_IPAddress,	IPAddress);
	DDX_Control(pDX, IDC_Account,	Account);
	DDX_Control(pDX, IDC_Password,	Password);
	DDX_Control(pDX, IDC_LIST1,		ListBox);
}

BEGIN_MESSAGE_MAP(CFTPClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(IDC_LIST1, &CFTPClientDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_Connect, &CFTPClientDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_Refresh, &CFTPClientDlg::OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_Upload, &CFTPClientDlg::OnBnClickedUpload)
	ON_BN_CLICKED(IDC_Download, &CFTPClientDlg::OnBnClickedDownload)
	ON_BN_CLICKED(IDC_Delete, &CFTPClientDlg::OnBnClickedDelete)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CFTPClientDlg::OnNMCustomdrawProgress1)
END_MESSAGE_MAP()


// CFTPClientDlg 消息处理程序

BOOL CFTPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFTPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFTPClientDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFTPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFTPClientDlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CFTPClientDlg::OnBnClickedConnect()
{
	if (connected == false)
	{
		CString ipaddress;
		CString account;
		CString password;
		GetDlgItemText(IDC_IPAddress, ipaddress);
		GetDlgItemText(IDC_Account, account);
		GetDlgItemText(IDC_Password, password);
		short status = OnConnect(ipaddress, account, password);
		if (status == SUCCESSFUL)
		{
			MessageBox("连接成功！");
			GetDlgItem(IDC_Connect)->SetWindowText("断开连接");
			connected = true;
			OnRefresh();
		}
		else if (status == FAILED_TYPE_1)
		{
			MessageBox("用户名或密码错误！");
		}
		else
		{
			MessageBox("连接失败，请检查IP地址或网络连接！");
		}
	}
	else
	{
		short status = OnDisconnect();
		if (status == SUCCESSFUL)
		{
			MessageBox("断开连接成功！");
			GetDlgItem(IDC_Connect)->SetWindowText("连接");
			connected = false;
			ListBox.ResetContent();
		}
		else
		{
			MessageBox("断开连接失败！");
		}
	}
	
}

void CFTPClientDlg::OnBnClickedRefresh()
{
	short status = OnRefresh();
	if (status == SUCCESSFUL)
	{
		MessageBox("刷新成功！");
	}
	else if (status == FAILED)
	{
		MessageBox("刷新失败！");
	}
	else
	{
		MessageBox("请先连接FTP服务器！");
	}
}


void CFTPClientDlg::OnBnClickedUpload()
{
	short status = OnUpload();
	if (status == SUCCESSFUL)
	{
		MessageBox("上传成功！");
		OnRefresh();
	}
	else if(status == FAILED_TYPE_1)
	{
		MessageBox("网络连接错误！");
	}
	else if(status == FAILED_TYPE_2)
	{
		MessageBox("文件打开失败！");
	}
	else if (status == DISCONNECTED)
	{
		MessageBox("请先连接FTP服务器！");
	}
	else // status == CANCELED
	{
		// Do Nothing. 
	}
}


void CFTPClientDlg::OnBnClickedDownload()
{
	short status = OnDownload();
	if (status == SUCCESSFUL)
	{
		MessageBox("下载成功！");
		OnRefresh();
	}
	else if(status == FAILED)
	{
		MessageBox("下载失败！");
	}
	else
	{
		MessageBox("请先连接FTP服务器！");
	}
}


void CFTPClientDlg::OnBnClickedDelete()
{
	short status = OnDelete();
	if (status == SUCCESSFUL)
	{
		MessageBox("删除成功！");
		OnRefresh();
	}
	else if(status == FAILED)
	{
		MessageBox("删除失败！");
	}
	else
	{
		MessageBox("请先连接FTP服务器！");
	}
}


void CFTPClientDlg::OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 进度条组件
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

short CFTPClientDlg::OnConnect(CString ipaddress, CString account, CString password)
{
	// 沈大为完成
	// 连接成功请返回 SUCCESSFUL
	// 账户密码错误请返回 FAILED_TYPE_1
	// 其他错误导致的连接失败请返回 FAILED_TYPE_2
	// 如果需要添加错误类型，请模仿OnUpload部分，并修改OnBnClickedConnect的MessageBox
	return SUCCESSFUL;
}

short CFTPClientDlg::OnDisconnect()
{
	// 断连成功返回 SUCCESSFUL
	// 断连失败返回 FAILED
	char send_buf[100], read_buf[100];
	int read_len = 100;
	sprintf(send_buf, "QUIT\r\n");
	write(control_sock, send_buf, strlen(send_buf));
	read(control_sock, read_buf, read_len);
	if (strncmp(read_buf, "221", 3) == 0)
	{
		close(control_sock);
		return SUCCESSFUL;
	}
	else return FAILED;
}

short CFTPClientDlg::OnRefresh()
{
	// 顾名扬完成
	// 未连接服务器请返回 DISCONNECTED
	// 刷新成功请返回 SUCCESSFUL
	// 刷新失败请返回 FAILED
	// 如果需要添加错误类型，请模仿OnUpload部分，并修改OnBnClickedRefresh的MessageBox
	if (connected == false) { return DISCONNECTED; }


	return SUCCESSFUL;
}

short CFTPClientDlg::OnUpload()
{
	// 梁川完成
	// 未连接服务器请返回 DISCONNECTED
	// 上传成功请返回 SUCCESSFUL
	// 连接错误请返回 FAILED_TYPE_1
	// 打开文件失败返回 FAILED_TYPE_2
	// 取消上传返回 CANCELED
	char rbuff[1024], sbuff[1024], cod[4];//收发缓冲区和返回的代码
	FILE* fd;
	if (connected == false) { return DISCONNECTED; }
	else {
		CString strname;
		//弹出“打开”对话框
		CFileDialog file(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "所有文件(*.*)|*.*|", this);
		if (file.DoModal() == IDOK)
		{
			strname = file.GetFileName();
		}
		else {
			// MessageBox("取消上传！");
			// return FAILED;
			return CANCELED;
		}
		sprintf(sbuff, "SIZE %s\r\n", strname);//CString在这些函数中可能会出现类型不匹配的问题，到时候改
		write(data_sock, sbuff, sizeof(sbuff));
		read(data_sock, rbuff, sizeof(rbuff));
		strncpy(cod, rbuff, 3);
		if (cod == "150") {
		//连接成功
			fd = fopen(strname, "wb");//以二进制打开文件
			if (fd) {
				//打开成功
				memset(sbuff, '\0', sizeof(sbuff));
				while (1) { //从文件中循环读取数据并发送
					//len用来实现进度条
					int len = fread(sbuff, 1, sizeof(sbuff), fd); //fread从file文件读取sizeof(sbuff)长度的数据到sbuff，返回成功读取的数据个数
					if (write(data_sock, sbuff, len) == SOCKET_ERROR) {
						// closesocket(datatcps);要不要断开？
						// 连接错误
						return FAILED_TYPE_1;
					}
					if (len < sizeof(sbuff)) {
						// closesocket(datatcps);要不要断开？
						break; //传输完成
					}
				}
			}
			else {
				// 打开文件失败
				return FAILED_TYPE_2;
			}
		}
		else {
			// 连接错误
			return FAILED_TYPE_1;
		}
		
	}

	return SUCCESSFUL;
}

short CFTPClientDlg::OnDownload()
{
	// 李睿哲完成
	// 未连接服务器请返回 DISCONNECTED
	// 下载成功请返回 SUCCESSFUL
	// 下载失败请返回 FAILED
	// 取消下载返回 CANCELED
	// 如果需要添加错误类型，请模仿OnUpload部分，并修改OnBnClickedDownload的MessageBox
	if (connected == false) { return DISCONNECTED; }


	return SUCCESSFUL;
}

short CFTPClientDlg::OnDelete()
{
	// 胡雅馨完成
	// 未连接服务器请返回 DISCONNECTED
	// 删除成功请返回 SUCCESSFUL
	// 删除失败请返回 FAILED
	// 如果需要添加错误类型，请模仿OnUpload部分，并修改OnBnClickedDelete的MessageBox
	if (connected == false) { return DISCONNECTED; }


	return SUCCESSFUL;
}