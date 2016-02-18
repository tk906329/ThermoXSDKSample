#if !defined(AFX_CLIENTSOCKET_H__BF27E892_9DC4_4461_8E3F_ACA905F633E91__INCLUDED_)
#define AFX_CLIENTSOCKET_H__BF27E892_9DC4_4461_8E3F_ACA905F633E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClientSocket3 command target

class CClientSocket2 : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CClientSocket2();
	virtual ~CClientSocket2();	
 

// Overrides
public:	
	void AnalysisPack(BYTE* buf, int ilen);

	int SendData(char *data, int len);
	DWORD m_iticket;
	BOOL m_bConenctStates;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket3)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket3)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:

	BYTE m_lpReceiveDataBuff[512];//接收数据缓冲区
	int m_iReceiveDataLen;	//接收数据长度
	int m_iReceivePtr;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__BF27E892_9DC4_4461_8E3F_ACA905F633E9__INCLUDED_)
