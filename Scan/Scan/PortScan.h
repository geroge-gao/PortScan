#pragma once


// CPortScan 对话框

class CPortScan : public CDialogEx
{
	DECLARE_DYNAMIC(CPortScan)

public:
	CPortScan(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPortScan();
	CString m_Address;
	CString m_Status;
	CString m_Port;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTcpScan();
	afx_msg void OnUdpScan();
private:
	SOCKET Socket;
};
