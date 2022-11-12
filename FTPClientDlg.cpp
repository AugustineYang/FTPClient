
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
	CString ipaddress;
	CString account;
	CString password;
	GetDlgItemText(IDC_IPAddress, ipaddress);
	GetDlgItemText(IDC_Account, account);
	GetDlgItemText(IDC_Password, password);
	short status = OnConnect(ipaddress, account, password);
	switch (status)
	{
	case 1:	 MessageBox("连接成功！"); OnRefresh(); break;
	case 2:  MessageBox("用户名或密码错误！"); break;
	default: MessageBox("连接失败！");
	}
}

void CFTPClientDlg::OnBnClickedRefresh()
{
	OnRefresh() ? MessageBox("刷新成功！") : MessageBox("刷新失败！");
}


void CFTPClientDlg::OnBnClickedUpload()
{
	if (OnUpload())
	{
		MessageBox("上传成功！");
		OnRefresh();
	}
	else
	{
		MessageBox("上传失败！");
	}
}


void CFTPClientDlg::OnBnClickedDownload()
{
	if (OnDownload())
	{
		MessageBox("下载成功！");
		OnRefresh();
	}
	else
	{
		MessageBox("下载失败！");
	}
}


void CFTPClientDlg::OnBnClickedDelete()
{
	if (OnDelete())
	{
		MessageBox("删除成功！");
		OnRefresh();
	}
	else
	{
		MessageBox("删除失败！");
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
	// 连接成功请返回1
	// 账户密码错误请返回2
	// 其他错误导致的连接失败请返回3
	return 1;
}

bool CFTPClientDlg::OnRefresh()
{
	// 顾名扬完成
	// 刷新成功请返回1
	// 刷新失败请返回0
	return 1;
}

bool CFTPClientDlg::OnUpload()
{
	// 梁川完成
	// 上传成功请返回1
	// 上传失败请返回0
	return 1;
}

bool CFTPClientDlg::OnDownload()
{
	// 李睿哲完成
	// 下载成功请返回1
	// 下载失败请返回0
	return 1;
}

bool CFTPClientDlg::OnDelete()
{
	// 胡雅馨完成
	// 删除成功请返回1
	// 删除失败请返回0
	return 1;
}