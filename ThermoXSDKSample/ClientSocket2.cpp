// 机器人共享用客户端
//

#include "stdafx.h"
#include "resource.h"
#include "ClientSocket2.h"
#include "ThermoXSDKSample.h"

// 主对话框指针

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_COM_BUF 100
#define MAX_INT_NUM 20
#define MODBUS_SLAVE_NO 5
static char b[MAX_COM_BUF] = { 0 };

unsigned int crc_chk(unsigned char* data, unsigned char length);
/////////////////////////////////////////////////////////////////////////////
// CClientSocket2

CClientSocket2::CClientSocket2()
{
	memset(m_lpReceiveDataBuff, 0, 512);
	m_iReceiveDataLen = 0;	
	m_iReceivePtr = 0;

	m_iticket = GetTickCount();
	m_bConenctStates = FALSE;
}

CClientSocket2::~CClientSocket2()
{

}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket2, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSocket3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket3 member functions
// 关闭事件
void CClientSocket2::OnClose(int nErrorCode) 
{
	m_bConenctStates = FALSE;
	Close();
}
// 连接事件
void CClientSocket2::OnConnect(int nErrorCode) 
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
void CClientSocket2::OnReceive(int nErrorCode) 
{
	if ( nErrorCode == 0 )
	{
		m_iReceiveDataLen = Receive(m_lpReceiveDataBuff + m_iReceivePtr, SH_REC_PACK_SIZE_CLIENT);

		if (m_iReceiveDataLen > 0) //处理收包	
		{
			m_iticket = GetTickCount();
			m_iReceivePtr += m_iReceiveDataLen;
			if (m_iReceivePtr >= 8)
			{
				AnalysisPack(m_lpReceiveDataBuff, m_iReceivePtr);
				m_iReceivePtr = 0;
			}
		}
	}	
	CAsyncSocket::OnReceive(nErrorCode);
}

//分析服务器返回的数据包
void CClientSocket2::AnalysisPack(BYTE* buf, int ilen)
{
	if (buf[0] != MODBUS_SLAVE_NO)		//从站号不匹配
		return;
	unsigned int reg_crc = crc_chk(buf, ilen - 2);
	if ((buf[ilen - 1] << 8 | buf[ilen - 2]) != reg_crc)	//校验码错误
		return;

	int addressType = (buf[2] & 0xF0) >> 4;
	int address = buf[2] & 0x7f << 8 | buf[3];
	int num = buf[4] << 8 | buf[5];

	if (address > 50)
		return;

	int intarray[MAX_INT_NUM] = { 0 };
	switch (buf[1])
	{
	case 0x01:
		if (addressType == 0)				////读位
		{
			b[0] = MODBUS_SLAVE_NO;
			b[1] = buf[1];
			b[2] = num;
			b[3] = theApp.m_pClientSocket->SHYx(address);
			reg_crc = crc_chk((unsigned char *)b, 4);
			b[4] = reg_crc & 0xff;
			b[5] = reg_crc >> 8;
			SendData(b, 6);
		}
		break;
	case 0x0f:
		if (addressType == 0)			//写位
		{
			memcpy(b, buf, 6);
			reg_crc = crc_chk((unsigned char *)b, 6);
			b[6] = reg_crc & 0xff;
			b[7] = reg_crc >> 8;
			SendData(b, 8);
			theApp.m_pClientSocket->SetSHYx(address, buf[7]);
		}
		break;
	case 0x03:
		if (addressType == 1)				//读整数
		{
			b[0] = MODBUS_SLAVE_NO;
			b[1] = buf[1];
			b[2] = num;
			for (size_t i = 0; i < num; i++)
			{
				//memcpy(b+3, theApp.m_pClientSocket->m_SHiVirtualYX+address,num*4);
				b[3 + i * 2] = theApp.m_pClientSocket->SHYx(address + i) >> 8;
				b[4 + i * 2] = theApp.m_pClientSocket->SHYx(address + i) & 0xFF;
			}
			reg_crc = crc_chk((unsigned char *)b, 3 + num * 4);
			b[3 + num * 2] = reg_crc & 0xff;
			b[4 + num * 2] = reg_crc >> 8;
			SendData(b, 5 + num * 2);
		}
		break;
	case 0x10:
		if (addressType == 1)			//写多个整数
		{
			if (num > MAX_INT_NUM)
				return;
			memcpy(b, buf, 6);
			reg_crc = crc_chk((unsigned char *)b, 6);
			b[6] = reg_crc & 0xff;
			b[7] = reg_crc >> 8;
			SendData(b, 8);
			for (size_t i = 0; i < num; i++)
			{
				intarray[i] = buf[7 + i * 2] << 8 | buf[8 + i * 2];
			}
			theApp.m_pClientSocket->SetSHYxs(address, num, intarray);
		}
		break;
	case 0x06:
		if (addressType == 1)			//写单个整数
		{
			memcpy(b, buf, 6);
			reg_crc = crc_chk((unsigned char *)b, 4);
			b[6] = reg_crc & 0xff;
			b[7] = reg_crc >> 8;
			SendData(b, 8);
			theApp.m_pClientSocket->SetSHYx(address, buf[4] << 8 | buf[5]);
		}
		break;
	default:
		break;
	}
}

// 发送数据
int CClientSocket2::SendData(char *data, int len)
{
	int nResult =  Send(data,len);

	return nResult;
}
/*
unsigned int crc_chk(unsigned char* data, unsigned char length)
{
	int j;
	unsigned int reg_crc = 0xFFFF;
	while (length--)
	{
		reg_crc ^= *data++;
		for (j = 0; j<8; j++)
		{
			if (reg_crc & 0x01)
			{
				reg_crc = (reg_crc >> 1) ^ 0xA001;
			}
			else {
				reg_crc = (reg_crc >> 1);
			}
		}
	}
	return reg_crc;
}*/