#if !defined(AFX_CLIENTSOCKET_H__BF27E892_9DC4_4461_8E3F_ACA905F633E9__INCLUDED_)
#define AFX_CLIENTSOCKET_H__BF27E892_9DC4_4461_8E3F_ACA905F633E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//
// ���հ���С Ϊ�˱���tcp�ְ�����ͨ�Ź滮��Լ�����Ĵ�С1024�����ݣ�+8��ͷ��
// ���ֵ���ܵ������޸�
const long RECEIVE_PACK_SIZE = 4096+8+8;

/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CClientSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CClientSocket();
	virtual ~CClientSocket();	
 

// Overrides
public:	
	int SendData(char *data,int len);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:

	BYTE m_lpReceiveDataBuff[RECEIVE_PACK_SIZE*2];//�������ݻ�����
	int m_iReceiveDataLen;	//�������ݳ���
	int m_iReceivePtr;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__BF27E892_9DC4_4461_8E3F_ACA905F633E9__INCLUDED_)
