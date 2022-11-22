
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
#include "afxinet.h"//添加
#include "AfxSock.h"
#include <iostream>

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
	AfxSocketInit();
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
		else if(status == FAILED_TYPE_2)
		{
			MessageBox("连接失败，请检查IP地址或网络连接！");
		}
		else
		{
			MessageBox("Socket服务错误！");
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
	else if (status == DISCONNECTED)
	{
		MessageBox("请先连接FTP服务器！");
	}
	else if (status == FAILED_TYPE_1)
	{
		MessageBox("网络连接错误！");
	}
	else
	{
		MessageBox("刷新失败！");
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
	else if (status == FAILED_TYPE_1)
	{
		MessageBox("socket接收失败");
	}
	else if(status == FAILED_TYPE_2)
	{
		MessageBox("socket发送失败");
	}
	else if (status == FAILED_TYPE_3)
	{
		MessageBox("被动模式启动失败");
	}
	else if (status == FAILED_TYPE_4)
	{
		MessageBox("数据连接建立失败");
	}
	else if (status == FAILED_TYPE_5)
	{
		MessageBox("数据连接断开");
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
	else if(status == FAILED_TYPE_1)
	{
		MessageBox("SOCKET发送失败！");
	}
	else if (status == FAILED_TYPE_2)
	{
		MessageBox("SOCKET接收失败！");
	}
	else if (status == FAILED_TYPE_3)
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
	char read_buf[BUFFER_SIZE], send_buf[BUFFER_SIZE];
	MEMSET(read_buf);
	MEMSET(send_buf);
	/* 初始化socket */
	control_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == control_sock)
	{
		return FAILED_TYPE_3;
	}

	struct sockaddr_in server;
	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = inet_addr(ipaddress);
	server.sin_port = htons(21);
	if (SOCKET_ERROR == connect(control_sock, (struct sockaddr*)&server, sizeof(server)))
	{
		return FAILED_TYPE_2;
	}
	recv(control_sock, read_buf, BUFFER_SIZE, 0);

	sprintf(send_buf, "USER %s\r\n", account);
	send(control_sock, send_buf, strlen(send_buf), 0);
	recv(control_sock, read_buf, BUFFER_SIZE, 0);

	sprintf(send_buf, "PASS %s\r\n", password);
	send(control_sock, send_buf, strlen(send_buf), 0);
	recv(control_sock, read_buf, BUFFER_SIZE, 0);

	if (strncmp(read_buf, "230", 3))
	{
		return FAILED_TYPE_1;
	}
	return SUCCESSFUL;
}

short CFTPClientDlg::OnDisconnect()
{
	// 断连成功返回 SUCCESSFUL
	// 断连失败返回 FAILED
	char send_buf[BUFFER_SIZE], read_buf[BUFFER_SIZE];
	sprintf(send_buf, "QUIT\r\n");
	send(control_sock, send_buf, strlen(send_buf), 0);
	recv(control_sock, read_buf, BUFFER_SIZE, 0);
	if (strncmp(read_buf, "221", 3) == 0)
	{
		closesocket(control_sock);
		return SUCCESSFUL;
	}
	else return FAILED;
}

short CFTPClientDlg::OnRefresh()
{
	// 顾名扬完成
	// 未连接服务器请返回 DISCONNECTED
	// 刷新成功请返回 SUCCESSFUL
	// 存在连接错误请返回 FAILED_TYPE_1
	// 刷新失败请返回 FAILED         
	// 如果需要添加错误类型，请模仿OnUpload部分，并修改OnBnClickedRefresh的MessageBox  232行
	SOCKET data_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //数据socket
	struct sockaddr_in serv_data_addr;//数据接口地址
	char rbuff[BUFFER_SIZE], sbuff[BUFFER_SIZE];
	if (connected == false) { return DISCONNECTED; }
	else {
		ListBox.ResetContent(); //先清空box里原有的内容
		MEMSET(rbuff);
		MEMSET(sbuff);
		sprintf(sbuff, "PASV\r\n");//PASV进入被动模式
		send(control_sock, sbuff, strlen(sbuff), 0);
		recv(control_sock, rbuff, BUFFER_SIZE, 0);
		if (strncmp(rbuff, "227", 3))
		{
			return FAILED_TYPE_1;
		}
		char* part[6];
		if (strtok(rbuff, "("))
		{
			for (int i = 0; i < 5; i++)
			{
				part[i] = strtok(NULL, ",");

			}
			part[5] = strtok(NULL, ")");
		}
		unsigned short ServerPort;   //获取服务器数据端口
		ServerPort = unsigned short((atoi(part[4]) << 8) + atoi(part[5]));
		char m_addr[200];
		strcpy_s(m_addr, part[0]);
		strcat_s(m_addr, ".");
		strcat_s(m_addr, part[1]);
		strcat_s(m_addr, ".");
		strcat_s(m_addr, part[2]);
		strcat_s(m_addr, ".");
		strcat_s(m_addr, part[3]);
		sockaddr_in serveraddr2;
		serveraddr2.sin_family = AF_INET;
		serveraddr2.sin_addr.s_addr = inet_addr(m_addr);
		serveraddr2.sin_port = htons(ServerPort);
		int iconnect = connect(data_sock, (SOCKADDR*)&serveraddr2, sizeof(SOCKADDR));//数据socket连接
		if (iconnect == SOCKET_ERROR) { return FAILED_TYPE_1; }// 连接错误
		
		sprintf(sbuff, "MLSD\r\n");//MLSD是 LIST 命令的替代品，旨在标准化目录列表的格式
		send(control_sock, sbuff, strlen(sbuff), 0);
		recv(control_sock, rbuff, BUFFER_SIZE, 0);
		recv(control_sock, rbuff, BUFFER_SIZE, 0);
		if (!strncmp(rbuff, "226", 3))//连接成功
		{
			int lens = recv(data_sock, rbuff, sizeof(rbuff), 0);
			if (lens == 0) { return FAILED; }//没有成功接收数据

			freopen("out.txt", "w", stdout);
			std::cout << rbuff << std::endl;
			fclose(stdout);
			//到project所在的文件夹里找一个叫out.txt的文件，里面就是rbuff的内容
			//研究一下MLSD返回的信息的格式

			while (lens != SOCKET_ERROR && lens != 0) {//接收残余数据
				ListBox.AddString(rbuff);
				lens = recv(data_sock, rbuff, sizeof(rbuff), 0);

			}
			/*
			while(lens != SOCKET_ERROR && lens != 0) {//接收残余数据
				lens = recv(data_sock, tmp, sizeof(tmp), 0);
				if (lens != 0 && lens != SOCKET_ERROR)
				{
					strcat(rbuff, tmp);
					memset(tmp,0,sizeof(tmp));
				}
			}
			ListBox.AddString(rbuff);*/
		}
		else // 连接错误
		{
			return FAILED_TYPE_1;
		}
	}

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
	int bcnt;//字节数
	int len;
	SOCKET data_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in serv_data_addr;//数据接口地址
	char rbuff[1024], sbuff[1024], cod[4];//收发缓冲区和返回的代码
	FILE* fd;
	if (connected == false) { return DISCONNECTED; }
	else {
		sprintf(sbuff, "PASV\r\n");//进入被动模式
		send(control_sock, sbuff, sizeof(sbuff),0);
		recv(control_sock,rbuff, sizeof(rbuff),0);
		strncpy(cod, rbuff, 3);
		cod[3] = '\0';
		if (cod != "227") {
			return FAILED_TYPE_1;
		}
		char* part[6];
		if (strtok(rbuff, "("))
		{
			for (int i = 0; i < 5; i++)
			{
				part[i] = strtok(NULL, ",");

			}
			part[5] = strtok(NULL, ")");
		}
		int ServerPort;
		ServerPort = (atoi(part[4]) << 8) + atoi(part[5]);
		char serv_addr[200];
		strcat_s(serv_addr, part[0]);
		strcat_s(serv_addr, ".");
		strcat_s(serv_addr, part[1]);
		strcat_s(serv_addr, ".");
		strcat_s(serv_addr, part[2]);
		strcat_s(serv_addr, ".");
		strcat_s(serv_addr, part[3]);
		serv_data_addr.sin_family = AF_INET;  //使用IPv4地址
		serv_data_addr.sin_addr.s_addr = inet_addr(serv_addr);//ip
		serv_data_addr.sin_port = htons(ServerPort);  //端口
		int iconnect = connect(data_sock, (SOCKADDR*)&serv_data_addr, sizeof(SOCKADDR));//数据socket是否连接成功
		if (iconnect == SOCKET_ERROR) {
			return FAILED_TYPE_1;
		}
		
		CString sname;
		char* strname;
		//弹出“打开”对话框
		//CFileDialog file(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "所有文件(*.*)|*.*|",this);
		CFileDialog file(TRUE);
		if (file.DoModal() == IDOK)
		{
			sname = file.GetFileName();
			USES_CONVERSION;
			//strname = T2A(sname);
			strname = "";
		}
		else {
			// 取消上传
			return CANCELED;
		}
		sprintf(sbuff, "SIZE %s\r\n", strname);//CString在这些函数中可能会出现类型不匹配的问题，到时候改
		memset(sbuff, 0, sizeof(sbuff));
		memset(rbuff, 0, sizeof(rbuff));
		send(data_sock, sbuff, sizeof(sbuff),0);//查看服务器中是否有该文件，若有则说明已经上传或传输中断
		recv(data_sock, rbuff, sizeof(rbuff),0);
		part[0]=strtok(rbuff, " ");
		part[1] = strtok(NULL, " ");
		if (part[0] == "213" && atoi(part[1]) > 0) {
			//存在文件，断点续传
			int foffset = atoi(part[1]);
			memset(sbuff, 0, sizeof(sbuff));
			memset(rbuff, 0, sizeof(rbuff));
			sprintf(sbuff, "REST %ld\r\n", foffset);
			send(data_sock, sbuff, sizeof(sbuff),0);//
			recv(data_sock, rbuff, sizeof(rbuff),0);
			sprintf(sbuff, "STOR %s\r\n", strname);
			send(data_sock, sbuff, sizeof(sbuff),0);//
			recv(data_sock, rbuff, sizeof(rbuff),0);
			strncpy(cod, rbuff, 3);
			cod[3] = '\0';
			if (cod == "150") {
				//连接成功
				fd = fopen(strname, "wb");//以二进制打开文件
				if (fd) {
					//打开成功
					memset(sbuff, '\0', sizeof(sbuff));
					bcnt = 0;
					len = 0;
					while (1) { //从文件中循环读取数据并发送
						//bcnt用来实现进度条
						if (bcnt < foffset) {//已传部分则不发送
							if ((bcnt + sizeof(sbuff)) > foffset) {
								len=fread(sbuff, 1, foffset - bcnt, fd);
							}
							else {
								len=fread(sbuff, 1, sizeof(sbuff), fd);
							}
							bcnt += len;
							continue;
						}
						else {
							len = fread(sbuff, 1, sizeof(sbuff), fd); //fread从file文件读取sizeof(sbuff)长度的数据到sbuff，返回成功读取的数据个数
							bcnt += len;

							if (send(data_sock, sbuff, len,0) == SOCKET_ERROR) {
								// closesocket(datatcps);要不要断开？
								// 连接错误
								return FAILED_TYPE_1;
							}
							if (len < sizeof(sbuff)) {
								closesocket(data_sock);//要不要断开？
								break; //传输完成
							}
						}
						
					}
					return SUCCESSFUL;
				}
				else {
					// 打开文件失败
					return FAILED_TYPE_2;
				}
			}
		}
		else {
			strncpy(cod, rbuff, 3);//零终止符的问题？
			cod[3] = '\0';
			if (cod == "150") {
				//连接成功
				fd = fopen(strname, "wb");//以二进制打开文件
				if (fd) {
					//打开成功
					memset(sbuff, '\0', sizeof(sbuff));
					while (1) { //从文件中循环读取数据并发送
						//len用来实现进度条
						int len = fread(sbuff, 1, sizeof(sbuff), fd); //fread从file文件读取sizeof(sbuff)长度的数据到sbuff，返回成功读取的数据个数
						if (send(data_sock, sbuff, len,0) == SOCKET_ERROR) {
							// closesocket(datatcps);要不要断开？
							// 连接错误
							return FAILED_TYPE_1;
						}
						if (len < sizeof(sbuff)) {
							// closesocket(datatcps);要不要断开？
							break; //传输完成
						}
					}
					return SUCCESSFUL;
				}
				else {
					// 打开文件失败
					return FAILED_TYPE_2;
				}
			}
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
	SOCKET data_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //数据socket
	char send_buf[1024] = {0};
	char recv_buf[1024] = {0};
	char cod[3] = {0};
	int recv_len = 1024;
	if (connected == false) { return DISCONNECTED; }
	else {
		//首先通过控制连接将服务器切换到被动模式
		sprintf(send_buf, "PASV\r\n");
		int isend = send(control_sock, send_buf, strlen(send_buf), 0);
		if (isend == SOCKET_ERROR) {
			return FAILED_TYPE_2;
		}
		int irecv = recv(control_sock, recv_buf, recv_len, 0);
		if (irecv == SOCKET_ERROR) {
			return FAILED_TYPE_1;
		}
		
		memcpy(cod, recv_buf, 3);
		if (cod != "227") {
			return FAILED_TYPE_3;
		}
		char* part[6];
		if (strtok(recv_buf, "("))
		{
			for (int i = 0; i < 5; i++)
			{
				part[i] = strtok(NULL, ",");
				
			}
			part[5] = strtok(NULL, ")");
		}
		unsigned short ServerPort;   //获取服务器数据端口
		ServerPort = unsigned short((atoi(part[4]) << 8) + atoi(part[5]));
		char m_addr[200];
		strcpy_s(m_addr, part[0]);
		strcat_s(m_addr, ".");
		strcat_s(m_addr, part[1]);
		strcat_s(m_addr, ".");
		strcat_s(m_addr, part[2]);
		strcat_s(m_addr, ".");
		strcat_s(m_addr, part[3]);

		sockaddr_in serveraddr2;
		serveraddr2.sin_family = AF_INET;
		serveraddr2.sin_addr.s_addr = inet_addr(m_addr);
		serveraddr2.sin_port = htons(ServerPort);
		
		int iconnect = connect(data_socket, (SOCKADDR*)&serveraddr2, sizeof(SOCKADDR));//数据socket连接
		if (iconnect == SOCKET_ERROR) {
			return FAILED_TYPE_4;
		}
		memset(send_buf, 0, sizeof(send_buf));
		memset(recv_buf, 0, sizeof(recv_buf));
		//下面为下载实现
		CString selfile;
		ListBox.GetText(ListBox.GetCurSel(), selfile);//获得想要下载资源名
		if (!selfile.IsEmpty())
		{
			
			CFileDialog file(FALSE, NULL, selfile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "所有文件(*.*)|*.*|", this);
			if (file.DoModal() == IDOK)
			{
				CString strname;
				strname = file.GetFileName();
				CString strdir;
				strdir = "";//这里填写本地目录位置
				//pFtpConnection->SetCurrentDirectory(strdir);
				//获取文件大小
				sprintf(send_buf,"SIZE %s\r\n",selfile);

				send(control_sock, send_buf, strlen(send_buf), 0);

				recv(control_sock, recv_buf, recv_len, 0);
				int file_len;
				strtok(recv_buf, " ");
				char* str;
				str = strtok(NULL, "\r\n");
				file_len = atoi(str);
				//发送下载命令：

				sprintf(send_buf, "RETR %s\r\n", selfile);

				send(control_sock, send_buf, strlen(send_buf),0);

				recv(control_sock, recv_buf, recv_len,0);

				strncpy(cod, recv_buf, 3);
				if (cod == "150") {
					FILE* op = NULL;
					op = fopen(strname, "wb");//打开本地文件夹
					int len = file_len;
					int buf_len = recv(data_socket, recv_buf, recv_len, 0);
					
					for( len ; len > 0; len = len - buf_len)
					{
						if (buf_len < 0) {
							
							break;
						}
						if (buf_len == 0)
							return FAILED_TYPE_5;
						fwrite(&recv_buf, 1, recv_len-1, op);
						buf_len = recv(data_socket, recv_buf, recv_len, 0);

					}
					if (len <= 0) {
						fclose(op);//关闭文件
						closesocket(data_socket);//关闭套接字
						return SUCCESSFUL;
					}
					//进行断点续传
					else {
						fseek(op, 0, SEEK_END);  //先用fseek将文件指针移到文件末尾
						int offset = ftell(op);
						
						sprintf(send_buf, "REST %ld\r\n", offset);

						send(control_sock, send_buf, strlen(send_buf), 0);

						recv(control_sock, recv_buf, recv_len, 0);

						sprintf(send_buf, "RETR %s\r\n", selfile);

						send(control_sock, send_buf, strlen(send_buf), 0);

						recv(control_sock, recv_buf, recv_len, 0);
						int len = file_len - offset;//继续传输
						int buf_len = recv(data_socket, recv_buf, recv_len, 0);

						for (len; len > 0; len = len - buf_len)
						{
							if (buf_len < 0) {

								break;
							}
							if (buf_len == 0)
								return FAILED_TYPE_5;
							fwrite(&recv_buf, 1, recv_len - 1, op);
							buf_len = recv(data_socket, recv_buf, recv_len, 0);

						}
						if (len <= 0) {
							fclose(op);//关闭文件
							closesocket(data_socket);//关闭套接字
							return SUCCESSFUL;
						}
						else {
							fclose(op);//关闭文件
							closesocket(data_socket);//关闭套接字
							return FAILED;
						}

					}

				}
				
			}
		}
	}
}

short CFTPClientDlg::OnDelete()
{
	// 胡雅馨完成
	// 未连接服务器请返回 DISCONNECTED
	// 删除成功请返回 SUCCESSFUL
	// 删除失败请返回 FAILED
	// 如果需要添加错误类型，请模仿OnUpload部分，并修改OnBnClickedDelete的MessageBox
	SOCKET data_sock;
	char rbuff[1024], sbuff[1024], cod[4];
	if (connected == false) { return DISCONNECTED; }
	else
	{
		CString selfile;
		ListBox.GetText(ListBox.GetCurSel(), selfile);//获取用户要删除的资源名
		char* filename = (LPSTR)(LPCTSTR)selfile;
		sprintf_s(sbuff, "RMD %s", filename);
		int ret = send(control_sock, sbuff, strlen(sbuff), 0);
		if (ret == -1)//SOCKET发送失败
			return FAILED_TYPE_1;
		memset(rbuff, 0, sizeof(rbuff));
		int len = recv(control_sock, rbuff, sizeof(rbuff), 0);
		if (len == SOCKET_ERROR) return FAILED_TYPE_2;
		strncpy(cod, rbuff, 3);
		if (cod != "250") return FAILED_TYPE_3;
	}

	return SUCCESSFUL;
}
