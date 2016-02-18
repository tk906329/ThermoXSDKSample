// ClientSocket.cpp : implementation file
//

#include "..\stdafx.h"
#include "..\Resource.h"


// 主对话框指针

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
	memset(m_lpReceiveDataBuff,0,RECEIVE_PACK_SIZE*2);
	m_iReceiveDataLen = 0;	
	m_iReceivePtr = 0;
}

CClientSocket::~CClientSocket()
{

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions
// 关闭事件
void CClientSocket::OnClose(int nErrorCode) 
{
	g_pMyDlgOpenGLDlg->m_iNetState=0;
	Close();
}
// 连接事件
void CClientSocket::OnConnect(int nErrorCode) 
{
	CString strNet;
    if (nErrorCode != 0) 
	{
		Close();
		g_pMyDlgOpenGLDlg->m_iNetState=0;
	}
	else 
	{
		CString strNet;
		strNet.Format("网络：连接%s:%d成功",CConfig::RobotIP,CConfig::RobotPort);
		g_pMyDlgOpenGLDlg->GetDlgItem(IDC_STATUS_NET)->SetWindowText(strNet);
		g_pMyDlgOpenGLDlg->m_iNetState=1;
	}

	CAsyncSocket::OnConnect(nErrorCode);
}
// 接收数据
void CClientSocket::OnReceive(int nErrorCode) 
{
	g_pMyDlgOpenGLDlg->m_AtctiveTime = CTime::GetCurrentTime();
	WORD iPacketSize;
	DWORD dwFlag;
	int iBuffSize;
	if ( nErrorCode == 0 )
	{
		//memset(m_lpReceiveDataBuff,0,RECEIVE_PACK_SIZE);
		m_iReceiveDataLen = Receive(m_lpReceiveDataBuff+m_iReceivePtr,RECEIVE_PACK_SIZE);

		while(1)
		{
			iBuffSize = m_iReceiveDataLen+m_iReceivePtr;
			if(iBuffSize<8)
			{
				m_iReceivePtr += m_iReceiveDataLen;
				break;
			}
			dwFlag = *(DWORD*)m_lpReceiveDataBuff;
			if(dwFlag != 0x67FDFDFD)
			{
				m_iReceivePtr = 0;
				break;
			}
			iPacketSize = *(WORD*)(m_lpReceiveDataBuff+6);
			if(iPacketSize+8>iBuffSize)
			{
				m_iReceivePtr = iBuffSize;
				break;
			}
			g_pMyDlgOpenGLDlg->AddData(m_lpReceiveDataBuff,iPacketSize+8);
			g_pMyDlgOpenGLDlg->WriteDebugData(m_lpReceiveDataBuff,iPacketSize+8,TRUE);

			CopyMemory(m_lpReceiveDataBuff,m_lpReceiveDataBuff+iPacketSize+8,iBuffSize-(iPacketSize+8));
			m_iReceivePtr = iBuffSize-(iPacketSize+8);
			m_iReceiveDataLen = 0;
		}
	}	
	CAsyncSocket::OnReceive(nErrorCode);
}
// 发送数据
int CClientSocket::SendData(char *data, int len)
{
	g_pMyDlgOpenGLDlg->m_AtctiveTime = CTime::GetCurrentTime();
	int nResult =  Send((char*)data,len);
	if ( nResult > 0 )
	{
		g_pMyDlgOpenGLDlg->WriteDebugData((unsigned char*)data,len,FALSE);
	}
	return nResult;
}

