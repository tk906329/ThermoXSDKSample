// MyConnect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyConnect2.h"
#include <winsock.h>
#include <math.h>
#include <IO.H>
#include <DIRECT.H>
#include "ThermoXSDKSample.h"
// MyConnect
#define MAX_COM_BUF 100
#define MAX_INT_NUM 20
#define MODBUS_SLAVE_NO 5

static HANDLE		hCom =(HANDLE)-1;		 //���ھ��
static CString		OutInforStr;

static int	m_iCount =0;
static byte a[MAX_COM_BUF] = { 0 };
static char b[MAX_COM_BUF] = { 0 };

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
			{ /*LSB(bit 0 ) = 1 */
				reg_crc = (reg_crc >> 1) ^ 0xA001;
			}
			else {
				reg_crc = (reg_crc >> 1);
			}
		}
	}
	return reg_crc;
}
BOOL SendRS4852(char *sendbuf,int len) 
{
	if(hCom !=(HANDLE)-1)
	{
		BOOL	fWriteStat;
		DWORD	dwBytesWritten;
		fWriteStat = WriteFile(hCom, sendbuf, len,&dwBytesWritten,NULL); //д����

		return fWriteStat;
	}
	return FALSE;
}

UINT ReceiveRS4852(LPVOID asvoid)
{
	BYTE	recvbuf[512];
	DWORD	bytesRecv=0;
	m_iCount = 0;
	while(hCom !=(HANDLE)-1)
	{
		//memset(recvbuf,0,sizeof(recvbuf));
		if(ReadFile(hCom, recvbuf, 512, &bytesRecv,NULL))
		{
			if(bytesRecv <=0)
			{
				Sleep(10);
				continue;
			}
			if (m_iCount + bytesRecv>=MAX_COM_BUF)
			{
				m_iCount = 0;
				continue;
			}
			memcpy(a + m_iCount, recvbuf, bytesRecv);
			m_iCount += bytesRecv;
			if (m_iCount >= 8)
			{
				ProcessData2();
				m_iCount = 0;
			}
		}
		else //����ʧ��
		{
			AfxMessageBox(L"123");
			CloseRS4852();
			return 0;
		}
	}
	return 1;
}

void ProcessData2()
{
	if (a[0] != MODBUS_SLAVE_NO)		//��վ�Ų�ƥ��
		return;
	unsigned int reg_crc = crc_chk(a, m_iCount-2);
	if ((a[m_iCount - 1] << 8 | a[m_iCount-2]) != reg_crc)	//У�������
		return;

	int addressType = (a[2] & 0xF0) >> 4;
	int address = a[2] & 0x7f << 8 | a[3];
	int num = a[4] << 8 | a[5];

	if (address > 50)
		return;

	int intarray[MAX_INT_NUM] = { 0 };
	switch (a[1])
	{
	case 0x01:
		if (addressType == 0)				////��λ
		{
			b[0] = MODBUS_SLAVE_NO;
			b[1] = a[1];
			b[2] = num;
			b[3] = theApp.m_pClientSocket->SHYx(address);
			reg_crc = crc_chk((unsigned char *)b, 4);
			b[4] = reg_crc & 0xff;
			b[5] = reg_crc >> 8;
			SendRS4852(b, 6);
		}
		break;
	case 0x0f:
		if (addressType == 0)			//дλ
		{
			memcpy(b, a, 6);
			reg_crc = crc_chk((unsigned char *)b, 6);
			b[6] = reg_crc & 0xff;
			b[7] = reg_crc >> 8;
			SendRS4852(b, 8);
			theApp.m_pClientSocket->SetSHYx(address, a[7]);
		}
		break;
	case 0x03:
		if (addressType==1)				//������
		{
			b[0] = MODBUS_SLAVE_NO;
			b[1] = a[1];
			b[2] = num;
			for (size_t i = 0; i < num; i++)
			{
				//memcpy(b+3, theApp.m_pClientSocket->m_SHiVirtualYX+address,num*4);
				b[3 + i*2] = theApp.m_pClientSocket->SHYx(address + i) >> 8;
				b[4 + i*2] = theApp.m_pClientSocket->SHYx(address + i) & 0xFF;
			}
			reg_crc = crc_chk((unsigned char *)b, 3+num*4);
			b[3 + num * 2] = reg_crc & 0xff;
			b[4 + num * 2] = reg_crc >> 8;
			SendRS4852(b, 5 + num * 2);
		}
		break;
	case 0x10:
		if (addressType == 1)			//д�������
		{
			if (num > MAX_INT_NUM)
				return;
			memcpy(b, a, 6);
			reg_crc = crc_chk((unsigned char *)b, 6);
			b[6] = reg_crc & 0xff;
			b[7] = reg_crc >> 8;
			SendRS4852(b, 8);
			for (size_t i = 0; i < num; i++)
			{
				intarray[i] = a[7+i*2] << 8 | a[8+i*2];
			}
			theApp.m_pClientSocket->SetSHYxs(address, num,intarray);
		}
		break;
	case 0x06:						
		if (addressType == 1)			//д��������
		{
			memcpy(b, a, 6);
			reg_crc = crc_chk((unsigned char *)b, 4);
			b[6] = reg_crc & 0xff;
			b[7] = reg_crc >> 8;
			SendRS4852(b, 8);
			theApp.m_pClientSocket->SetSHYx(address, a[4] << 8 | a[5]);
		}
		break;
	default:
		break;
	}
}

UINT OpenRS4852(CString g_RS485Port, int g_RS485Baud)
{
	hCom =CreateFile(g_RS485Port, GENERIC_READ | GENERIC_WRITE, // �����д
		0, // �������Ϊ0
		NULL,	// no security attrs
		OPEN_EXISTING,	//���ò�����ʽ
		0,	// ͬ��ͨ��
		NULL );
	if(hCom==(HANDLE)-1)
	{
		OutInforStr =L"�� " +g_RS485Port +L" ʧ��,��ȷ�����ļ�������иö˿ڲ��Ҹö˿�δ��ʹ��!";
		MessageBox(0,OutInforStr,L"��ʾ",MB_TOPMOST|0x30);
		return 0;
	}
	DCB dcb;
	GetCommState(hCom,&dcb);
	dcb.BaudRate=g_RS485Baud; //������
	dcb.ByteSize=8;		//ÿ���ֽ���8λ
	dcb.Parity=NOPARITY; //����żУ��λ
	dcb.StopBits=ONESTOPBIT; //ֹͣλ
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	
	SetCommState(hCom,&dcb);
	
	COMMTIMEOUTS TimeOuts;
	GetCommTimeouts(hCom, &TimeOuts);
	//�趨����ʱ////////�ڶ�һ�����뻺���������ݺ���������������أ��������Ƿ������Ҫ����ַ���
	TimeOuts.ReadIntervalTimeout=20;		//����ʱ�����ַ�������ʱ��
	TimeOuts.ReadTotalTimeoutMultiplier=0;  //��ȡÿ�ֽڵĳ�ʱ
	TimeOuts.ReadTotalTimeoutConstant=100;   //���������ݵĹ̶���ʱ
//	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier=10;
	TimeOuts.WriteTotalTimeoutConstant=1000;
	SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ

	OutInforStr =L"�� " +g_RS485Port +L" �ɹ�!";
	Sleep(200);
	::AfxBeginThread(ReceiveRS4852,THREAD_PRIORITY_NORMAL);
	return 1;
}

void CloseRS4852()
{
	if(hCom!=(HANDLE)-1)
	{
		CloseHandle(hCom);
	}
	hCom =(HANDLE)-1;
	Sleep(200);
}