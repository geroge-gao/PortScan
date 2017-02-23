
// ScanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Scan.h"
#include "ScanDlg.h"
#include "afxdialogex.h"
#include"PortScan.h"

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


// CScanDlg 对话框



CScanDlg::CScanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SCAN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScanDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CScanDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CScanDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CScanDlg 消息处理程序

BOOL CScanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

void CScanDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CScanDlg::OnPaint()
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
HCURSOR CScanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CScanDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit *p1, *p2, *pStatus;
	CString address_1, address_2, Address;
	p1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	p2 = (CEdit*)GetDlgItem(IDC_EDIT2);
	pStatus = (CEdit*)GetDlgItem(IDC_EDIT3);
	p1->GetWindowText(address_1);
	p2->GetWindowText(address_2);

	CString s1 = address_1.Right(3);
	CString s2 = address_2.Right(3);
	Address = address_1.Left(address_1.GetLength() - 3);
	//将网络号转换成整数
	int a1 = atoi(s1);
	int a2 = atoi(s2);

	//循环进行端口扫描
	for (int i = a1; i <= a2; i++)
	{
		CString port, temp;
		port.Format("%d", i);
		temp = Address;
		temp.Append(port);
		m_Address = temp;
		OnScan();
		pStatus->SetWindowText(m_Status);
	}
}


void CScanDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CPortScan cpdlg;
	cpdlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CScanDlg::OnScan()
{
	UpdateData(true);
	WSADATA WSAData;             //建立与Socket的绑定
								 //MAKEWORD(2,2)表示使用WINSOCK2版本.wsaData用来存储系统传回的关于WINSOCK的资料.
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		MessageBox("WSAStartup初始化失败!");
		WSACleanup();
		return;
	}
	/*
	AF_INET表示ipv4协议
	SOCK_RAW表示原始套字节
	IPPROTO_ICMP表示ICMP协议
	*/
	Socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	int code = WSAGetLastError();
	if (Socket == INVALID_SOCKET)
	{
		MessageBox("Sokcet创建失败");
		WSACleanup();
		return;
	}
	int SendTimeout = 1000;//设置发送时延
	if (setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&SendTimeout, sizeof(SendTimeout)) == SOCKET_ERROR)
	{
		MessageBox("设置发送超时!");
		closesocket(Socket);
		WSACleanup();
		return;
	}
	int RecvTimeout = 1000;              //设置接受超时时间
										 //防止发生拥塞
	if (setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&RecvTimeout, sizeof(RecvTimeout)) == SOCKET_ERROR)
	{
		MessageBox("设置发送超时时间!");
		closesocket(Socket);
		WSACleanup();
		return;
	}
	int IpAddress;
	IpAddress = inet_addr(m_Address);
	if (IpAddress == INADDR_NONE)
	{
		hostent *pHostent = gethostbyname(m_Address);
		if (pHostent)
			IpAddress = (*(in_addr*)pHostent->h_addr).s_addr;
	}
	sockaddr_in DestHost;     //定义目的主机地址
	memset(&DestHost, 0, sizeof(DestHost));
	DestHost.sin_family = AF_INET;
	DestHost.sin_addr.s_addr = IpAddress;
	char IcmpData[MAX_PACKET];//填充icmp数据包
	icmp_head *IcmpHead;
	int IcmpSize;
	memset(IcmpData, 0, MAX_PACKET);
	IcmpHead = (icmp_head*)IcmpData;
	IcmpSize = DEF_PACKET + sizeof(icmp_head);
	IcmpHead->Type = 8;
	IcmpHead->Code = 0;
	IcmpHead->Identifier = (unsigned short)GetCurrentThreadId();
	IcmpHead->Sequence = 1;
	IcmpHead->HeadCheckSum = CheckSum((short*)IcmpData, IcmpSize);
	int nSend;                  //发送数据包
	nSend = sendto(Socket, IcmpData, IcmpSize, 0, (sockaddr*)&DestHost, sizeof(DestHost));
	if (nSend == SOCKET_ERROR || nSend < IcmpSize)
	{
		MessageBox("ICMP包发送失败!");
		closesocket(Socket);
		WSACleanup();
		return;
	}
	sockaddr_in SourceHost;
	int SourceSize;
	char *RecvBuf;
	int RecvSize;
	SourceSize = sizeof(SourceHost);
	memset(&SourceHost, 0, SourceSize);
	RecvSize = MAX_PACKET + sizeof(ip_head);
	RecvBuf = new char[RecvSize];
	memset(RecvBuf, 0, RecvSize);
	int nRecv;
	nRecv = recvfrom(Socket, RecvBuf, RecvSize, 0, (sockaddr*)&SourceHost, &SourceSize);
	if (nRecv == SOCKET_ERROR)
	{
		m_Status += m_Address;
		m_Status += ": Closed\r\n";
	}
	else
	{
		ip_head *IpHead;
		int IpSize;
		IpHead = (ip_head*)RecvBuf;
		IpSize = (IpHead->HeadLen & 0x0f) * 4;
		IcmpHead = (icmp_head*)(RecvBuf + IpSize);
		if (nRecv < IpSize + ICMP_MIN)
		{
			m_Status += m_Address;
			m_Status += ": Closed\r\n";
		}
		else
		{
			if (IcmpHead->Type != 0)
			{
				m_Status += m_Address;
				m_Status += ": Closed\r\n";
			}
			else
			{
				if (IcmpHead->Identifier != (short)GetCurrentThreadId())
				{
					m_Status += m_Address;
					m_Status += ": Closed\r\n";
				}
				else
				{
					m_Status += m_Address;
					m_Status += ": Open\r\n";
				}
			}
		}
	}

	UpdateData(false);
	closesocket(Socket);
	WSACleanup();
}

short CScanDlg::CheckSum(short *buffer,int size)
{
	int Sum = 0;
	while (size > 0)
	{
		Sum += *(buffer++);
		size -= sizeof(short);
	}
	if (size)
		Sum += *(char*)buffer;
	Sum = (Sum >> 16) + (Sum & 0xffff);
	Sum += (Sum >> 16);
	return (short)(~Sum);
}
