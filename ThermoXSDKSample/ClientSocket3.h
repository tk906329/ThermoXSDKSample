#if !defined(AFX_CLIENTSOCKET_H__BF27E892_9DC4_4461_8E3F_ACA905F633E91__INCLUDED_)
#define AFX_CLIENTSOCKET_H__BF27E892_9DC4_4461_8E3F_ACA905F633E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//

const int SH_REC_PACK_SIZE_CLIENT = 2048;//tcpһ�ν��մ�С
const int SH_MAX_YX_C = 128;//yx ���ݸ���
const int SH_MAX_YC_C = 64;//yc ���ݸ���

const int SH_MAX_STRING_C = 16;//�ַ�������
const int SH_MAX_STRING_LEN_C = 31;//����ַ�������
/////////////////////////////////////////////////////////////////////////////
// CClientSocket3 command target

class CClientSocket3 : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CClientSocket3();
	virtual ~CClientSocket3();	
 

// Overrides
public:	
	void reset();
	int SendData(char *data,int len);
	int SendHeartData();
	int SetSHYx(int iNo, int iValue);
	int SetSHYc(int iNo, double dbValue);
	int SetSHYcs(int iNo, int num, double *dbValue);
	int SetSHYxs(int iNo, int num, int *intValue);
	void AnalysisPack(BYTE* buf, int ilen);
	void ProcSHCmd(char*buff);
	void ProcSHYX(char *buff);
	void ProcSHYC(char *buff);
	int SHYx(int iNo);
	double SHYc(int iNo);
	void ProcString_share(char *buff, int len);
	char* SHString(int iNo);
	int SetSHString(int iNo, char* str);

	int m_SHiVirtualYX[SH_MAX_YX_C];  //Yxֵ
	double m_SHdbYCData[SH_MAX_YC_C];  //Yc����
	char m_lpString[SH_MAX_STRING_C][SH_MAX_STRING_LEN_C + 1];//�����ַ���
	int m_iStringLen[SH_MAX_STRING_C];//�����ַ�����

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

	BYTE m_lpReceiveDataBuff[SH_REC_PACK_SIZE_CLIENT * 2];//�������ݻ�����
	int m_iReceiveDataLen;	//�������ݳ���
	int m_iReceivePtr;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__BF27E892_9DC4_4461_8E3F_ACA905F633E9__INCLUDED_)
