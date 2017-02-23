
// ScanDlg.h : 头文件
//

#pragma once

#define ICMP_MIN 8
#define DEF_PACKET 32
#define MAX_PACKET 1024

// CScanDlg 对话框
class CScanDlg : public CDialogEx
{
// 构造
public:
	CScanDlg(CWnd* pParent = NULL);	// 标准构造函数

	CString m_Address;
	CString m_Status;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	afx_msg void OnScan();

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

private:
	short CheckSum(short *buffer, int size);
	SOCKET Socket;
	typedef struct IP_HEAD
	{
		union
		{
			unsigned char Version;	//版本
			unsigned char HeadLen;	//头部长度
		};
		unsigned char ServiceType;	//服务类型
		unsigned short TotalLen;	//总长度
		unsigned short Identifier;	//标识符
		union
		{
			unsigned short Flags;	//标志
			unsigned short FragOffset;	//片偏移
		};
		unsigned char TimeToLive;
		unsigned char Protocol;		//协议
		unsigned short HeadCheckSum;	//头部校验和
		unsigned int SourceAddr;	//源地址
		unsigned int DestAddr;		//目的ip
	}ip_head;

	typedef struct ICMP_HEAD
	{
		unsigned char Type;
		unsigned char Code;   //代码
		unsigned HeadCheckSum;  //校验和
		unsigned short Identifier;
		unsigned short Sequence;
	}icmp_head;
};
