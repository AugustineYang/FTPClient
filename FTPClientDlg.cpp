
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
#include <cstring>
#include <cstddef>
#include<string>
#include <direct.h>
#pragma comment(lib, "wsock32.lib")
#pragma warning(disable:4996)
#pragma once

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
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
	DDX_Control(pDX, IDC_Connect, Connect);
	DDX_Control(pDX, IDC_Refresh, Refresh);
	DDX_Control(pDX, IDC_Upload, Upload);
	DDX_Control(pDX, IDC_Download, Download);
	DDX_Control(pDX, IDC_Delete, Delete);
	DDX_Control(pDX, IDC_IPAddress, IPAddress);
	DDX_Control(pDX, IDC_Account, Account);
	DDX_Control(pDX, IDC_Password, Password);
	DDX_Control(pDX, IDC_LIST1, ListBox);
	DDX_Control(pDX, IDC_PROGRESS1, m_pro);
}

BEGIN_MESSAGE_MAP(CFTPClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_LBN_SELCHANGE(IDC_LIST1, &CFTPClientDlg::OnLbnSelchangeList1)
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
	m_pro.SetRange(0,100);
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

void CFTPClientDlg::OnBnClickedConnect()
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
		MessageBox(_T("连接成功！"));
		connected = true;
		OnRefresh();
	}
	else if (status == FAILED_TYPE_1)
	{
		MessageBox(_T("用户名或密码错误！"));
	}
	else if (status == FAILED_TYPE_2)
	{
		MessageBox(_T("连接失败，请检查IP地址或网络连接！"));
	}
	else
	{
		MessageBox(_T("Socket服务错误！"));
	}
	
}

void CFTPClientDlg::OnBnClickedRefresh()
{
	short status = OnRefresh();
	if (status == SUCCESSFUL)
	{
		MessageBox(_T("刷新成功！"));
	}
	else if (status == DISCONNECTED)
	{
		MessageBox(_T("请先连接FTP服务器！"));
	}
	else if (status == FAILED_TYPE_1)
	{
		MessageBox(_T("网络连接错误！"));
	}
	else
	{
		MessageBox(_T("刷新失败！"));
	}
}


void CFTPClientDlg::OnBnClickedUpload()
{
	short status = OnUpload();
	if (status == SUCCESSFUL)
	{
		MessageBox(_T("上传成功！"));
		OnRefresh();
	}
	else if(status == FAILED_TYPE_1)
	{
		MessageBox(_T("网络连接错误！"));
	}
	else if(status == FAILED_TYPE_2)
	{
		MessageBox(_T("文件打开失败！"));
	}
	else if (status == DISCONNECTED)
	{
		MessageBox(_T("请先连接FTP服务器！"));
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
		MessageBox(_T("下载成功！"));
		OnRefresh();
	}
	else if(status == FAILED)
	{
		MessageBox(_T("下载失败！"));
	}
	else if (status == FAILED_TYPE_3)
	{
		MessageBox(_T("被动模式启动失败"));
	}
	else if (status == FAILED_TYPE_4)
	{
		MessageBox(_T("数据连接建立失败"));
	}
	else if (status == FAILED_TYPE_5)
	{
		MessageBox(_T("网络连接错误！"));
	}
	else if (status == FAILED_TYPE_6)
	{
		MessageBox(_T("请选择要下载的文件"));
	}
	else if(status == DISCONNECTED)
	{
		MessageBox(_T("请先连接FTP服务器！"));
	}
	else // status == CANCELED
	{
		// Do Nothing.
	}
}


void CFTPClientDlg::OnBnClickedDelete()
{
	short status = OnDelete();
	if (status == SUCCESSFUL)
	{
		MessageBox(_T("删除成功！"));
		OnRefresh();
	}
	else if(status == FAILED_TYPE_1)
	{
		MessageBox(_T("删除失败！"));
	}
	else
	{
		MessageBox(_T("请先连接FTP服务器！"));
	}
}


void CFTPClientDlg::OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 进度条组件
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

Server CFTPClientDlg::GetPASVAddr(char* rbuff)
{
	char* part[6];
	MEMSET(part);
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
	char ServerAddr[200];
	MEMSET(ServerAddr);
	strcpy_s(ServerAddr, part[0]);
	strcat_s(ServerAddr, ".");
	strcat_s(ServerAddr, part[1]);
	strcat_s(ServerAddr, ".");
	strcat_s(ServerAddr, part[2]);
	strcat_s(ServerAddr, ".");
	strcat_s(ServerAddr, part[3]);
	return Server{ ServerPort, ServerAddr };
}

short CFTPClientDlg::OnConnect(CString ipaddress, CString account, CString password)
{
	// 沈大为、杨元钊完成
	// 连接成功返回 SUCCESSFUL
	// 账户密码错误返回 FAILED_TYPE_1
	// 其他错误导致的连接失败返回 FAILED_TYPE_2
	char rbuff[BUFFER_SIZE], sbuff[BUFFER_SIZE];
	MEMSET(sbuff); MEMSET(rbuff);
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
	RECV(control_sock);

	MEMSET(sbuff); MEMSET(rbuff);
	sprintf(sbuff, "USER %s\r\n", account);
	SEND(control_sock);
	RECV(control_sock);

	MEMSET(sbuff); MEMSET(rbuff);
	sprintf(sbuff, "PASS %s\r\n", password);
	SEND(control_sock);
	RECV(control_sock);

	if (strncmp(rbuff, "230", 3))
	{
		return FAILED_TYPE_1;
	}
	else return SUCCESSFUL;
}

short CFTPClientDlg::OnRefresh()
{
	// 顾名扬、杨元钊完成
	// 未连接服务器返回 DISCONNECTED
	// 刷新成功返回 SUCCESSFUL
	// 存在连接错误返回 FAILED_TYPE_1
	// 刷新失败返回 FAILED         
	SOCKET data_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //数据socket
	struct sockaddr_in serv_data_addr;//数据接口地址
	char rbuff[BUFFER_SIZE*10], sbuff[BUFFER_SIZE];
	if (connected == false) { return DISCONNECTED; }
	else {
		ListBox.ResetContent(); //先清空box里原有的内容
		MEMSET(rbuff); MEMSET(sbuff);
		sprintf(sbuff, "PASV\r\n");//PASV进入被动模式
		SEND(control_sock);
		RECV(control_sock);
		if (strncmp(rbuff, "227", 3))
		{
			return FAILED_TYPE_1;
		}

		Server server = GetPASVAddr(rbuff);
		unsigned short ServerPort = server.ServerPort;
		char* ServerAddr = server.ServerAddr;
		sockaddr_in serveraddr2;
		serveraddr2.sin_family = AF_INET;
		serveraddr2.sin_addr.s_addr = inet_addr(ServerAddr);
		serveraddr2.sin_port = htons(ServerPort);
		int iconnect = connect(data_sock, (SOCKADDR*)&serveraddr2, sizeof(SOCKADDR));//数据socket连接
		if (iconnect == SOCKET_ERROR) { return FAILED_TYPE_1; }// 连接错误
		
		MEMSET(sbuff); MEMSET(rbuff);
		sprintf(sbuff, "MLSD\r\n");//MLSD是 LIST 命令的替代品，旨在标准化目录列表的格式
		SEND(control_sock);
		RECV(control_sock);
		RECV(control_sock);

		if (strncmp(rbuff, "226", 3) == 0)//连接成功
		{
			MEMSET(rbuff);
			int lens = RECV(data_sock);
			if (lens == 0) { return FAILED; }//没有成功接收数据
			while (lens != SOCKET_ERROR && lens != 0) {//接收残余数据
				char* p = strtok(rbuff, "\r\n");
				while (p)
				{
					char* pp = strstr(p, "; ");
					ListBox.AddString(pp + 2);
					p = strtok(NULL, "\r\n");
				}
				MEMSET(rbuff);
				lens = RECV(data_sock);

				/*
				//另一种实现方式 by 顾名扬
				std::string str = rbuff;
				int num = 0;//用来计算个数
				int index1 = 0;//用来查找文件名前面出现的空格
				int index2 = 1;//用来查找文件名后面紧跟的下一个文件的type，从而截取中间的文件名
				while ((index1 = str.find(" ", index1)) < str.length()) {
					if((index2 = str.find("type=", index2)) < str.length()) {
						std::string filename = str.substr(index1 + 1, index2 - index1 - 1);
						char* file_name;
						file_name = new char[filename.size() + 1];//filename只是用于中间格式转换的过渡
						strcpy(file_name, filename.c_str());
						ListBox.AddString(file_name);
				while ((index1 = str.find(";", index1)) < str.length()) {
					num++;
					if (num % 3 == 0) {
						if ((index2 = str.find("\r", index1)) < str.length()) {
							std::string filename = str.substr(index1 + 2, index2 - index1 );
							char* file_name;
							file_name = new char[filename.size() + 1];//filename只是用于中间格式转换的过渡
							strcpy(file_name, filename.c_str());
							ListBox.AddString(file_name);
						}
					}
					index1++;
					index2++;
				}
				*/
			}
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
	// 未连接服务器返回 DISCONNECTED
	// 上传成功返回 SUCCESSFUL
	// 连接错误请返回 FAILED_TYPE_1
	// 打开文件失败返回 FAILED_TYPE_2
	// 取消上传返回 CANCELED
	long int bcnt;//字节数
	int len;
	SOCKET data_sock = socket(AF_INET, SOCK_STREAM, 0);
	char rbuff[1024], sbuff[1024];//收发缓冲区和返回的代码
	FILE* fd;
	if (connected == false) { return DISCONNECTED; }
	else {
		MEMSET(rbuff); MEMSET(sbuff);
		sprintf(sbuff, "TYPE A\r\n");//进入被动模式
		SEND(control_sock);
		RECV(control_sock);

		MEMSET(rbuff); MEMSET(sbuff);
		sprintf(sbuff, "PASV\r\n");//进入被动模式
		SEND(control_sock);
		RECV(control_sock);

		if (strncmp(rbuff, "227", 3)) {
			return FAILED_TYPE_1;
		}

		Server server = GetPASVAddr(rbuff);
		unsigned short ServerPort = server.ServerPort;
		char* ServerAddr = server.ServerAddr;
		sockaddr_in serv_data_addr;//数据接口地址
		serv_data_addr.sin_family = AF_INET;  //使用IPv4地址
		serv_data_addr.sin_addr.s_addr = inet_addr(ServerAddr);//ip
		serv_data_addr.sin_port = htons(ServerPort);  //端口
		int iconnect = connect(data_sock, (SOCKADDR*)&serv_data_addr, sizeof(SOCKADDR));//数据socket是否连接成功
		if (iconnect == SOCKET_ERROR) {
			return FAILED_TYPE_1;
		}

		CString strname, strpath;
		//弹出“打开”对话框
		//CFileDialog file(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "所有文件(*.*)|*.*|",this);
		CFileDialog file(TRUE);
		if (file.DoModal() == IDOK)
		{
			strname = file.GetFileName();
			strpath = file.GetPathName();
			SetCurrentDirectoryA(strpath);
		}
		else {
			// 取消上传
			return CANCELED;
		}
		MEMSET(rbuff); MEMSET(sbuff);
		sprintf(sbuff, "SIZE %s\r\n", strname);
		SEND(control_sock);
		RECV(control_sock);
		if (strncmp(rbuff, "550", 3) == 0) {
			MEMSET(rbuff); MEMSET(sbuff);
			sprintf(sbuff, "STOR %s\r\n", strname);//CString在这些函数中可能会出现类型不匹配的问题，到时候改
			SEND(control_sock);//查看服务器中是否有该文件，若有则说明已经上传或传输中断
			RECV(control_sock);
			if (strncmp(rbuff, "150", 3) == 0) {
				//连接成功
				fd = fopen(strpath, "rb");//以二进制打开文件
				if (fd != NULL) {
					//打开成功
					bcnt = 0;
					long int size = 0;
					fseek(fd, 0, SEEK_END);
					size = ftell(fd);
					fseek(fd, 0, SEEK_SET);
					while (1) { //从文件中循环读取数据并发送
						//len用来实现进度条
						MEMSET(sbuff);
						int len = fread(sbuff, 1, sizeof(sbuff), fd); //fread从file文件读取sizeof(sbuff)长度的数据到sbuff，返回成功读取的数据个数
						if (send(data_sock, sbuff, len, 0) == SOCKET_ERROR) {
							closesocket(data_sock);
							return FAILED_TYPE_1;
						}
						bcnt += len;
						float percent = float(bcnt) / float(size) * 100;
						m_pro.SetPos(percent);
						if (len < sizeof(sbuff)) {
							closesocket(data_sock);
							break; //传输完成
						}
					}
					fclose(fd);
					MEMSET(rbuff);
					RECV(control_sock);
					return SUCCESSFUL;
				}
				else {
					// 打开文件失败
					return FAILED_TYPE_2;
				}
			}
		}
		else {
			long int offset = atoi(strtok(rbuff + 4, "\r"));
			MEMSET(rbuff); MEMSET(sbuff);
			sprintf(sbuff, "REST %d\r\n", offset);//CString在这些函数中可能会出现类型不匹配的问题，到时候改
			SEND(control_sock);//查看服务器中是否有该文件，若有则说明已经上传或传输中断
			RECV(control_sock);

			MEMSET(rbuff); MEMSET(sbuff);
			sprintf(sbuff, "STOR %s\r\n", strname);//CString在这些函数中可能会出现类型不匹配的问题，到时候改
			SEND(control_sock);//查看服务器中是否有该文件，若有则说明已经上传或传输中断
			RECV(control_sock);

			if (strncmp(rbuff, "150", 3) == 0) {
				//连接成功
				fd = fopen(strpath, "rb");//以二进制打开文件
				if (fd != NULL) {
					//打开成功
					bcnt = offset;
					long int size = 0;
					fseek(fd, 0, SEEK_END);
					size = ftell(fd);
					fseek(fd, offset, SEEK_SET);
					while (1) { //从文件中循环读取数据并发送
						//len用来实现进度条
						MEMSET(sbuff);
						int len = fread(sbuff, 1, sizeof(sbuff), fd); //fread从file文件读取sizeof(sbuff)长度的数据到sbuff，返回成功读取的数据个数
						if (send(data_sock, sbuff, len, 0) == SOCKET_ERROR) {
							closesocket(data_sock);
							return FAILED_TYPE_1;
						}
						bcnt += len;
						float percent = float(bcnt) / float(size) * 100;
						m_pro.SetPos(percent);
						if (len < sizeof(sbuff)) {
							closesocket(data_sock);
							break; //传输完成
						}
					}
					fclose(fd);
					MEMSET(rbuff);
					RECV(control_sock);
					return SUCCESSFUL;
				}
				else {
					// 打开文件失败
					return FAILED_TYPE_2;
				}
			}
		}


	}
}

short CFTPClientDlg::OnDownload()
{
	// 李睿哲完成
	// 未连接服务器请返回 DISCONNECTED
	// 下载成功请返回 SUCCESSFUL
	// 下载失败请返回 FAILED
	// 取消下载返回 CANCELED
	// 如果需要添加错误类型，请模仿OnUpload部分，并修改OnBnClickedDownload的MessageBox
	SOCKET data_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //数据socket
	char sbuff[BUFFER_SIZE], rbuff[BUFFER_SIZE];
	MEMSET(rbuff); MEMSET(sbuff);
	if (connected == false) { return DISCONNECTED; }
	else {
		sprintf(sbuff, "TYPE I\r\n");
		SEND(control_sock);
		RECV(control_sock);

		//首先通过控制连接将服务器切换到被动模式
		MEMSET(rbuff); MEMSET(sbuff);
		sprintf(sbuff, "PASV\r\n");
		SEND(control_sock);
		RECV(control_sock);
		
		if (strncmp(rbuff, "227", 3)) {
			return FAILED_TYPE_3;
		}

		Server server = GetPASVAddr(rbuff);
		unsigned short ServerPort = server.ServerPort;
		char* ServerAddr = server.ServerAddr;
		sockaddr_in serveraddr2;
		serveraddr2.sin_family = AF_INET;
		serveraddr2.sin_addr.s_addr = inet_addr(ServerAddr);
		serveraddr2.sin_port = htons(ServerPort);
		int iconnect = connect(data_sock, (SOCKADDR*)&serveraddr2, sizeof(SOCKADDR));//数据socket连接
		if (iconnect == SOCKET_ERROR) {
			return FAILED_TYPE_4;
		}

		MEMSET(rbuff); MEMSET(sbuff);
		//下面为下载实现
		CString selfile;
		int n = ListBox.GetCurSel();
		if (n == -1) {
			return FAILED_TYPE_6;
		}
		ListBox.GetText(n, selfile);//获得想要下载资源名
		if (!selfile.IsEmpty())
		{
			CFolderPickerDialog folder;
			if (folder.DoModal() == IDOK)
			{
				CString des;
				des = folder.GetPathName();
				CString strpath = des + "\\" + selfile;
				FILE* fd;
				
				fd = fopen(strpath, "ab");
				if (!fd) {
					return FAILED;
				}
				fseek(fd, 0, SEEK_END);//定位文件指针到文件尾。

				int size = ftell(fd);//获取文件指针偏移量，即文件大小。

				MEMSET(rbuff); MEMSET(sbuff);
				sprintf(sbuff, "SIZE %s\r\n", selfile);
				SEND(control_sock);
				RECV(control_sock);
				int file_len;
				strtok(rbuff, " ");
				char* str;
				str = strtok(NULL, "\r\n");
				file_len = atoi(str);

				int off = 0;
				if (size == 0) {//如果是空文件那么从头下载
					//发送下载命令：
					MEMSET(rbuff); MEMSET(sbuff);
					sprintf(sbuff, "RETR %s\r\n", selfile);
					SEND(control_sock);
					RECV(control_sock);
					
					if (strncmp(rbuff, "150", 3) == 0) {
						int len = file_len;
						MEMSET(rbuff);
						int buf_len = RECV(data_sock);
						while (buf_len > 0)
						{
							CString temp = _T(rbuff);
							fwrite(rbuff, 1, buf_len, fd);
							off += buf_len;
							float percent = float(off) / float(file_len) * 100;
							m_pro.SetPos(percent);
							MEMSET(rbuff);
							buf_len = RECV(data_sock);
						}
						if (buf_len <= 0 && off < file_len) {
							return FAILED_TYPE_5;
						}
						if (off == file_len) {
							MEMSET(rbuff);
							RECV(control_sock);
							fclose(fd);
							closesocket(data_sock);//关闭套接字
							return SUCCESSFUL;
						}
					}
					
				}
				else {//断点续传
					int offset = size;
					off += offset;

					MEMSET(rbuff); MEMSET(sbuff);
					sprintf(sbuff, "REST %ld\r\n", offset);
					SEND(control_sock);
					RECV(control_sock);

					MEMSET(rbuff); MEMSET(sbuff);
					sprintf(sbuff, "RETR %s\r\n", selfile);
					SEND(control_sock);
					RECV(control_sock);

					int len = file_len - offset;//继续传输
					MEMSET(rbuff);
					int buf_len = RECV(data_sock);
					while (buf_len > 0)
					{
						fwrite(rbuff, 1, buf_len, fd);
						off += buf_len;
						MEMSET(rbuff);
						float percent = float(off) / float(file_len) * 100;
						m_pro.SetPos(percent);
						buf_len = RECV(data_sock);
					}
					if (buf_len <= 0 && off < file_len) {
						return FAILED_TYPE_5;
					}
					if (off == file_len) {
						MEMSET(rbuff);
						RECV(control_sock);
						fclose(fd);
						closesocket(data_sock);//关闭套接字
						return SUCCESSFUL;
					}
				}
			}
			else
			{
				return CANCELED;
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
	char rbuff[BUFFER_SIZE], sbuff[BUFFER_SIZE];
	if (connected == false) { return DISCONNECTED; }
	else
	{
		CString selfile;
		ListBox.GetText(ListBox.GetCurSel(), selfile);//获取用户要删除的资源名
		CString filename = selfile;
		MEMSET(sbuff);
		MEMSET(rbuff);
		sprintf(sbuff, "DELE %s\r\n", filename);
		SEND(control_sock);
		RECV(control_sock);
		if (strncmp(rbuff,"250",3)) return FAILED_TYPE_1;
	}

	return SUCCESSFUL;
}

	
