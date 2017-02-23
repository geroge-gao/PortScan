// PortScan.cpp : 实现文件
//

#include "stdafx.h"
#include "Scan.h"
#include "PortScan.h"
#include "afxdialogex.h"


// CPortScan 对话框

IMPLEMENT_DYNAMIC(CPortScan, CDialogEx)

CPortScan::CPortScan(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CPortScan::~CPortScan()
{
}

void CPortScan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPortScan, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPortScan::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPortScan::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPortScan 消息处理程序


void CPortScan::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit *cp1, *cp2, *cstatus;
	cp1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	cp2 = (CEdit*)GetDlgItem(IDC_EDIT2);
	cstatus = (CEdit*)GetDlgItem(IDC_EDIT3);
	cp1->GetWindowTextA(m_Address);
	cp2->GetWindowTextA(m_Port);
	OnTcpScan();
	cstatus->SetWindowTextA(m_Status);
}


void CPortScan::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit *cp1, *cp2, *cstatus;
	cp1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	cp2 = (CEdit*)GetDlgItem(IDC_EDIT2);
	cstatus = (CEdit*)GetDlgItem(IDC_EDIT3);
	cp1->GetWindowTextA(m_Address);
	cp2->GetWindowTextA(m_Port);
	OnUdpScan();
	cstatus->SetWindowTextA(m_Status);
}

void CPortScan::OnTcpScan()
{
	UpdateData(true);
	m_Status += "TCP Port:";
	WSADATA WASdata;//建立套字节绑定
	if (WSAStartup(MAKEWORD(2, 2), &WASdata) != 0)//
	{
		MessageBox("初始化Winsock失败");
		return;
	}

	Socket = socket(AF_INET, SOCK_STREAM, 0);//创建连接套字节
	if (Socket == INVALID_SOCKET)
	{
		MessageBox("创建字符串失败!");
		WSACleanup();
		return;
	}
	int ipAddress;
	ipAddress = inet_addr(m_Address);
	if (ipAddress == INADDR_NONE)//inaddr_none指ip地址无效
	{
		hostent* pHostent = gethostbyname(m_Address);
		if (pHostent)
			ipAddress = (*(in_addr*)pHostent->h_addr).s_addr;
	}

	sockaddr_in DestHost;
	memset(&DestHost, 0, sizeof(DestHost));
	DestHost.sin_family = AF_INET;//协议族,Socket编程中只能使用AF_INET;
	int t = atoi(m_Port);
	DestHost.sin_port = htons(t);//设置端口,htons将主机字节序编程网络序
	DestHost.sin_addr.s_addr = ipAddress;//存数ip地址

	int nConnect;     //与服务器建立联系
	nConnect = connect(Socket, (sockaddr*)&DestHost, sizeof(DestHost));
	if (nConnect == SOCKET_ERROR)
	{
		m_Status += m_Port;
		m_Status += ": Close\r\n";
	}
	else
	{
		m_Status += m_Port;
		m_Status += ": Open\r\n";
	}
	UpdateData(false);
	closesocket(Socket);
	WSACleanup();//释放套接字绑定
}

void CPortScan::OnUdpScan()
{
	UpdateData(true);
	m_Status += "UDP Port:";
	WSADATA WSAData;//建立套字节
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		MessageBox("初始化Winsock失败!");
		return;
	}

	/*
	AF_INET表示ipv4协议
	SOCK_DGRAM表示udp协议
	*/
	Socket = socket(AF_INET, SOCK_DGRAM, 0);//创建连接套字节

	if (Socket == INVALID_SOCKET)
	{
		MessageBox("创建Socket失败!");
		WSACleanup();
		return;
	}
	int nIotrl;
	unsigned long Flag = 1;
	nIotrl = ioctlsocket(Socket, FIONBIO, &Flag);//非阻塞模式
	if (nIotrl == SOCKET_ERROR)
	{
		MessageBox("设置Socket模式失败!");
		closesocket(Socket);
		WSACleanup();
		return;
	}
	int ipAddress;
	ipAddress = inet_addr(m_Address);//按ip地址顺序转换成整形
	if (ipAddress == INADDR_NONE)
	{
		hostent *pHostent = gethostbyname(m_Address);
		if (pHostent)
			ipAddress = (*(in_addr*)pHostent->h_addr).s_addr;
	}
	sockaddr_in DestHost;
	memset(&DestHost, 0, sizeof(DestHost));
	DestHost.sin_family = AF_INET;
	DestHost.sin_port = htons(atoi(m_Port));
	DestHost.sin_addr.s_addr = ipAddress;

	//绑定host
	if (bind(Socket, (sockaddr*)&DestHost, sizeof(DestHost)) < 0)
	{
		m_Status += "bind error\r\n";
	}

	char *SendBuf;
	SendBuf = new char[4];
	memset(SendBuf, 0, 4);
	strcpy(SendBuf, "cd");
	int nSend;
	nSend = sendto(Socket, SendBuf, strlen(SendBuf), 0, (sockaddr*)&DestHost, sizeof(DestHost));
	if (nSend == SOCKET_ERROR)
	{
		MessageBox("UDP包发送失败!");
		closesocket(Socket);
		WSACleanup();
		return;
	}
	Sleep(500);

	sockaddr_in SourceHost;
	int SourceSize;
	SourceSize = sizeof(SourceHost);
	memset(&SourceHost, 0, SourceSize);
	char *RecvBuf;
	RecvBuf = new char[4];
	memset(RecvBuf, 0, 4);
	int nRecv;			//接受udp数据包
						//阻塞式方法
						//若无错误将返回读入的字节数
	nRecv = recvfrom(Socket, RecvBuf, 4, 0, (sockaddr*)&SourceHost, &SourceSize);
	if (nRecv == SOCKET_ERROR)
	{
		int ErrorCode = WSAGetLastError();
		if (ErrorCode == 10054)
		{
			m_Status += m_Port;
			m_Status += ": Close\r\n";
		}
		if (ErrorCode == 10035)
		{
			m_Status += m_Port;
			m_Status += ": 阻塞 \r\n";
		}
	}
	else
	{
		m_Status += m_Port;
		//		m_Status += "\r\ndata:";
		//		m_Status += RecvBuf;
		m_Status += ": Open\r\n";
	}

	UpdateData(false);
	closesocket(Socket);
	WSACleanup();
}