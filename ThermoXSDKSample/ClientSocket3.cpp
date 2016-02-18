// 机器人共享用客户端
//

#include "stdafx.h"
#include "resource.h"
#include "ClientSocket3.h"

// 主对话框指针

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CClientSocket3

CClientSocket3::CClientSocket3()
{
	memset(m_lpReceiveDataBuff, 0, SH_REC_PACK_SIZE_CLIENT * 2);
	m_iReceiveDataLen = 0;	
	m_iReceivePtr = 0;

	for (int i = 0; i<SH_MAX_YX_C; i++)
	{
		m_SHiVirtualYX[i] = 0;
	}
	for (int i = 0; i<SH_MAX_YC_C; i++)
	{
		m_SHdbYCData[i] = 0;
	}
	for (int i = 0; i<SH_MAX_STRING_C; i++)
	{
		memset(m_lpString[i], 0, SH_MAX_STRING_LEN_C + 1);
		m_iStringLen[i] = 0;
	}
	m_iticket = GetTickCount();
	m_bConenctStates = FALSE;
}

CClientSocket3::~CClientSocket3()
{

}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket3, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSocket3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket3 member functions
// 关闭事件
void CClientSocket3::OnClose(int nErrorCode) 
{
	m_bConenctStates = FALSE;
	Close();
}
// 连接事件
void CClientSocket3::OnConnect(int nErrorCode) 
{
    if (nErrorCode != 0) 
	{
		m_bConenctStates = FALSE;
		Close();
	}
	else 
	{
		m_bConenctStates = TRUE;
		m_iReceivePtr = 0;
	}

	CAsyncSocket::OnConnect(nErrorCode);
}
// 接收数据
void CClientSocket3::OnReceive(int nErrorCode) 
{
	if ( nErrorCode == 0 )
	{
		m_iReceiveDataLen = Receive(m_lpReceiveDataBuff + m_iReceivePtr, SH_REC_PACK_SIZE_CLIENT);

		if (m_iReceiveDataLen > 0) //处理收包	
		{
			unsigned short iPacketSize; //完整包的大小
			int iBuffSize;//剩余长度
			int iPtr = 0;//已处理的位置

			while (1)
			{
				//剩下的长度
				iBuffSize = m_iReceiveDataLen + m_iReceivePtr - iPtr;

				if (iBuffSize <= 0)//剩下的长度<=0,已处理完
				{
					m_iReceivePtr = 0;
					break;
				}

				//缓存中不足8字节,留下次处理
				if ((m_iReceiveDataLen + m_iReceivePtr) < 8)
				{
					m_iReceivePtr += m_iReceiveDataLen;
					break;
				}

				//搜索头
				bool bFound = false;
				for (int k = 0; k < (iBuffSize - 4); k++)
				{
					if (m_lpReceiveDataBuff[iPtr + k] == 0xFD && m_lpReceiveDataBuff[iPtr + k + 1] == 0xFD
						&& m_lpReceiveDataBuff[iPtr + k + 2] == 0xFD && m_lpReceiveDataBuff[iPtr + k + 3] == 0x67)
					{
						iPtr = iPtr + k;
						bFound = true;
						break;
					}
				}
				//没找到头,整个包丢弃
				if (!bFound)
				{
					m_iReceivePtr = 0;
					break;
				}

				//包不完整，留下次处理
				iPacketSize = m_lpReceiveDataBuff[6 + iPtr] + (m_lpReceiveDataBuff[6 + iPtr + 1] << 8) + 8;
				if (iPacketSize>SH_REC_PACK_SIZE_CLIENT*2)     //包长度过长，大于能接收的2倍包长，容易造成缓存溢出
				{
					m_iReceivePtr = 0;
					break;
				}
				if (iBuffSize < iPacketSize)
				{
					memcpy(m_lpReceiveDataBuff, m_lpReceiveDataBuff + iPtr, iBuffSize);
					m_iReceivePtr = iBuffSize;
					break;
				}

				//处理完整包
				//CDllRobotDemoDlg *pdlg = (CDllRobotDemoDlg *)AfxGetMainWnd();
				AnalysisPack(m_lpReceiveDataBuff + iPtr, iPacketSize);
				iPtr += iPacketSize;
			}
		}
	}	
	CAsyncSocket::OnReceive(nErrorCode);
}

//分析服务器返回的数据包
void CClientSocket3::AnalysisPack(BYTE* buf, int ilen)
{
	if (ilen < 8)
	{
		return;
	}

	if (!(buf[0] == 0xFD && buf[1] == 0xFD && buf[2] == 0xFD && buf[3] == 0x67))
	{
		return;
	}
	m_iticket = GetTickCount();
	switch (buf[4])
	{
	case 0x81:
		ProcSHYX((char*)(buf + 8));
		break;
	case 0x82:
		ProcSHYC((char*)(buf + 8));
		break;
	case 0x83:
		ProcSHCmd((char*)(buf + 8));
	case 0x84:
		ProcString_share((char*)(buf + 8), ilen - 8);
		break;
	default:
		break;
	}
}

void CClientSocket3::ProcSHCmd(char*buff)
{
	int nCmd = buff[0] | buff[1] << 8;
	//int nResult = buff[2] | buff[3] << 8 ;
	switch (nCmd)
	{
	case 0x01:
		// printf("\n setYx return %d",nResult);
		break;
	case 0x02:
		//printf("\n setYc return %d",nResult);
		break;
	case 0x03:
		// printf("\n setMYx return %d",nResult);
		break;
	case 0x04:
		//  printf("\n setMYc return %d",nResult);
		break;
	case 0x05:
		//  printf("\n disYxYC return %d",nResult);
		break;
	case 0x06:
		//  printf("\n disYxYC return %d",nResult);
		break;
	case 0x07:
		//  printf("\n disYxYC return %d",nResult);
		break;
	default:
		break;
	}
}

void CClientSocket3::ProcSHYX(char *buff)
{
	memcpy(m_SHiVirtualYX, buff, SH_MAX_YX_C * 4);
}

void CClientSocket3::ProcSHYC(char *buff)
{
	memcpy(m_SHdbYCData, buff, SH_MAX_YX_C * 4);
}

int CClientSocket3::SHYx(int iNo)
{
	if (iNo<0 || iNo>(SH_MAX_YX_C - 1)) return 0;

	return (m_SHiVirtualYX[iNo]);
}
double CClientSocket3::SHYc(int iNo)
{
	if (iNo<0 || iNo>(SH_MAX_YC_C - 1)) return 0.0;

	return (m_SHdbYCData[iNo]);
}
//处理共享字符串
void CClientSocket3::ProcString_share(char *buff, int len)
{
	int iYxNo = 0;//Yx地址
	int iNum = 0;//字符串个数
	int iLen;//字符串长度

	if (len < 1)//
	{
		return;
	}

	iYxNo = 0;//YX地址
	iNum = 16;//字符串个数
	if (((iYxNo + iNum) >= 0) &&
		((iYxNo + iNum) <= SH_MAX_STRING_C) &&
		(iYxNo >= 0) &&
		(iNum <= SH_MAX_STRING_C))
		;
	else
	{
		return;
	}

	int lensum = 0;
	for (int i = 0; i < iNum; i++)
	{
		iLen = buff[lensum];
		if ((iLen >= 0) && (iLen <= SH_MAX_STRING_LEN_C))
		{
			memset(m_lpString[iYxNo + i], 0, SH_MAX_STRING_LEN_C + 1);
			memcpy(m_lpString[iYxNo + i], buff + lensum + 1, iLen);
			lensum += (iLen + 1);
			m_iStringLen[iYxNo + i] = iLen;
		}
		else
		{
			break;
		}
	}
}

//获取字符串
char* CClientSocket3::SHString(int iNo)
{
	if (iNo<0 || iNo>(SH_MAX_STRING_C - 1)) return (char*)("");

	return (m_lpString[iNo]);
}
//设置字符串
int CClientSocket3::SetSHString(int iNo, char* str)
{
	if (iNo<0 || iNo>(SH_MAX_STRING_C - 1)) return 0;

	char pBuffer[SH_MAX_STRING_LEN_C + 20];
	int iPtr = 0;

	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0x67;

	pBuffer[iPtr++] = 0x03;	//发送命令
	pBuffer[iPtr++] = 0x00;

	pBuffer[iPtr++] = 0x00;	//数据长度
	pBuffer[iPtr++] = 0x00;

	pBuffer[iPtr++] = 0x06;	//set string
	pBuffer[iPtr++] = 0x00;

	pBuffer[iPtr++] = iNo & 0xFF;//地址
	pBuffer[iPtr++] = (iNo >> 8) & 0xFF;

	int iLen = strlen(str);

	if (iLen>SH_MAX_STRING_LEN_C)
	{
		pBuffer[iPtr++] = SH_MAX_STRING_LEN_C;//len
		memcpy(pBuffer + iPtr, str, SH_MAX_STRING_LEN_C); //值
		iPtr += SH_MAX_STRING_LEN_C;
	}
	else
	{
		pBuffer[iPtr++] = iLen;//len
		memcpy(pBuffer + iPtr, str, iLen); //值
		iPtr += iLen;
	}

	pBuffer[6] = (iPtr - 8) & 0xFF;	//数据长度
	pBuffer[7] = ((iPtr - 8) >> 8) & 0xFF;

	return (SendData(pBuffer, iPtr));
}
// 发送数据
int CClientSocket3::SendData(char *data, int len)
{
	int nResult =  Send(data,len);

	return nResult;
}

//发送心跳
int CClientSocket3::SendHeartData()
{
	char pBuffer[20];
	int iPtr = 0;

	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0x67;

	pBuffer[iPtr++] = 0x00;	
	pBuffer[iPtr++] = 0x00;
	pBuffer[iPtr++] = 0x00;	
	pBuffer[iPtr++] = 0x00;

	return (SendData(pBuffer, iPtr));
}
//设置单个共享YC值
int CClientSocket3::SetSHYc(int iNo, double dbValue)
{
	if (iNo<0 || iNo>(SH_MAX_YC_C - 1)) return 0;

	char pBuffer[20];
	int iPtr = 0;

	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0x67;

	pBuffer[iPtr++] = 0x03;	//发送命令
	pBuffer[iPtr++] = 0x00;

	pBuffer[iPtr++] = 0x0C;	//数据长度
	pBuffer[iPtr++] = 0x00;

	pBuffer[iPtr++] = 0x02;	//设置单个共享YC值
	pBuffer[iPtr++] = 0x00;

	pBuffer[iPtr++] = iNo & 0xFF;//地址
	pBuffer[iPtr++] = (iNo >> 8) & 0xFF;

	memcpy(pBuffer + iPtr, &dbValue, 8); //值
	iPtr += 8;
	
	return (SendData(pBuffer, iPtr));
}

//设置多个共享YC值
int CClientSocket3::SetSHYcs(int iNo, int num,double *dbValue)
{
	if (iNo<0 || num<=0 || (iNo+num)>(SH_MAX_YC_C - 1)) return 0;

	char pBuffer[100];//14+4*8
	int iPtr = 0;

	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0x67;

	pBuffer[iPtr++] = 0x03;	//发送命令
	pBuffer[iPtr++] = 0x00;

	int len = 6 + 8 * num;
	pBuffer[iPtr++] = len & 0xFF;	//数据长度
	pBuffer[iPtr++] = (len >> 8) & 0xFF;

	pBuffer[iPtr++] = 0x04;	//设置单个共享YC值
	pBuffer[iPtr++] = 0x00;

	pBuffer[iPtr++] = iNo & 0xFF;//地址
	pBuffer[iPtr++] = (iNo >> 8) & 0xFF;

	pBuffer[iPtr++] = num & 0xFF;//个数
	pBuffer[iPtr++] = (num >> 8) & 0xFF;

	memcpy(pBuffer + iPtr, dbValue, 8*num); //值
	iPtr += 8 * num;

	return (SendData(pBuffer, iPtr));
}
//设置单个共享YX值
int CClientSocket3::SetSHYx(int iNo, int iValue)
{
	if (iNo<0 || iNo>(SH_MAX_YX_C - 1)) return 0;

	char pBuffer[16];
	int iPtr = 0;

	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0x67;

	pBuffer[iPtr++] = 0x03;	//发送命令
	pBuffer[iPtr++] = 0x00;

	pBuffer[iPtr++] = 0x08;	//数据长度
	pBuffer[iPtr++] = 0x00;

	pBuffer[iPtr++] = 0x01;	//设置单个共享YX值
	pBuffer[iPtr++] = 0x00;

	pBuffer[iPtr++] = iNo & 0xFF;//地址
	pBuffer[iPtr++] = (iNo >> 8) & 0xFF;

	pBuffer[iPtr++] = iValue & 0xFF; //值
	pBuffer[iPtr++] = (iValue >> 8) & 0xFF;
	pBuffer[iPtr++] = (iValue >> 16) & 0xFF;
	pBuffer[iPtr++] = (iValue >> 24) & 0xFF;

	return (SendData(pBuffer, iPtr));
}

//设置多个共享YX值
int CClientSocket3::SetSHYxs(int iNo, int num, int *intValue)
{
	if (iNo<0 || num<=0|| (iNo+num)>(SH_MAX_YX_C - 1)) return 0;

	char pBuffer[100];//14+4*8
	int iPtr = 0;

	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0xFD;
	pBuffer[iPtr++] = 0x67;

	pBuffer[iPtr++] = 0x03;	//发送命令
	pBuffer[iPtr++] = 0x00;

	int len = 6 + 4 * num;
	pBuffer[iPtr++] = len & 0xFF;	//数据长度
	pBuffer[iPtr++] = (len >> 8) & 0xFF;

	pBuffer[iPtr++] = 0x03;	//设置多个共享YX值
	pBuffer[iPtr++] = 0x00;

	pBuffer[iPtr++] = iNo & 0xFF;//地址
	pBuffer[iPtr++] = (iNo >> 8) & 0xFF;

	pBuffer[iPtr++] = num & 0xFF;//个数
	pBuffer[iPtr++] = (num >> 8) & 0xFF;

	memcpy(pBuffer + iPtr, intValue, 4 * num); //值
	iPtr += 4 * num;

	return (SendData(pBuffer, iPtr));
}
void CClientSocket3::reset()
{
	memset(m_lpReceiveDataBuff, 0, SH_REC_PACK_SIZE_CLIENT * 2);
	m_iReceiveDataLen = 0;
	m_iReceivePtr = 0;

	memset(m_SHiVirtualYX, 0, SH_MAX_YX_C * 4);

	memset(m_SHdbYCData, 0, SH_MAX_YX_C * 4);

	for (int i = 0; i<SH_MAX_STRING_C; i++)
	{
		memset(m_lpString[i], 0, SH_MAX_STRING_LEN_C + 1);
		m_iStringLen[i] = 0;
	}
}