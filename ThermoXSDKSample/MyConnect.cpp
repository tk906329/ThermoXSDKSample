// MyConnect.cpp : 实现文件
//

#include "stdafx.h"
#include "MyConnect.h"
#include <winsock.h>
#include <math.h>
#include <IO.H>
#include <DIRECT.H>
#include "ThermoXSDKSample.h"
// MyConnect

HANDLE		hCom =(HANDLE)-1;		 //串口句柄
CString		OutInforStr;

int iFlagComm = 0;
int			m_updateCount =0;
int a1, a2, a3, a4, a5, a6, a7, a8;

int	HexToChar(BYTE *pData,char *ReBuff,DWORD DataSize)
{
	DWORD	i=0;
	for(;i<DataSize;i++)
	{
		BYTE	temp=pData[i];
		if((temp>>4)<10)
		{
			ReBuff[i*2]='0'+(temp>>4);
		}
		else
			ReBuff[i*2]='A'+((temp>>4)-10);

		if((temp&0x0f)<10)
			ReBuff[i*2+1]='0'+(temp&0x0f);
		else
			ReBuff[i*2+1]='A'+((temp&0x0f)-10);
	}
	return DataSize;
}

void WriteEvent(CString writestring) //输出到文件
{
	
}

int SendRS485(char *sendbuf,int len) 
{
	if(hCom !=(HANDLE)-1)
	{
		BOOL	fWriteStat;
		DWORD	dwBytesWritten;
		fWriteStat = WriteFile(hCom, sendbuf, len,&dwBytesWritten,NULL); //写数据
		return 1;
	}
	return -1;
}

UINT ReceiveRS485(LPVOID asvoid)
{
	BYTE	recvbuf[512];
	DWORD	bytesRecv=0;
	while(hCom !=(HANDLE)-1)
	{
		/*Sleep(10);
		srand(time(0));
		for (int i = 0; i < DATAMAX; i++)
		{
			g_fDataValue1[i] = (g_fDataValue1[i]+1);
			g_fDataValue2[i] = (g_fDataValue2[i]+1);
		}*/
		memset(recvbuf,0,sizeof(recvbuf));
		if(ReadFile(hCom, recvbuf, 512, &bytesRecv,NULL))
		{//通知主线程，串口收到数据
			/*memset(bb,0,sizeof(bb));
			HexToChar((unsigned char*)recvbuf,bb,bytesRecv);
			m_logfile.MyWriteFile(bb,strlen(bb));
			m_logfile.MyWriteFile("\r\n",2);*/
			if(bytesRecv <=0)
			{
				Sleep(10);
				continue;
			}
			ProcRxdData(bytesRecv, recvbuf);
		}
		else //调用失败
		{
			AfxMessageBox(L"123");
			CloseRS485();
			return 0;
		}
	}
	return 1;
}

UINT OpenRS485(CString g_RS485Port, int g_RS485Baud)
{
	hCom =CreateFile(g_RS485Port, GENERIC_READ | GENERIC_WRITE, // 允许读写
		0, // 此项必须为0
		NULL,	// no security attrs
		OPEN_EXISTING,	//设置产生方式
		0,	// 同步通信
		NULL );
	if(hCom==(HANDLE)-1)
	{
		OutInforStr =L"打开 " +g_RS485Port +L" 失败,请确认您的计算机上有该端口并且该端口未被使用!";
		MessageBox(0,OutInforStr,L"提示",MB_TOPMOST|0x30);
		return 0;
	}
	DCB dcb;
	GetCommState(hCom,&dcb);
	dcb.BaudRate=g_RS485Baud; //波特率
	dcb.ByteSize=8;		//每个字节有8位
	dcb.Parity=NOPARITY; //无奇偶校验位
	dcb.StopBits=ONESTOPBIT; //停止位
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	
	SetCommState(hCom,&dcb);
	
	COMMTIMEOUTS TimeOuts;
	GetCommTimeouts(hCom, &TimeOuts);
	//设定读超时////////在读一次输入缓冲区的内容后读操作就立即返回，而不管是否读入了要求的字符。
//	TimeOuts.ReadIntervalTimeout=30;		//接收时，两字符间最大的时延
//	TimeOuts.ReadTotalTimeoutMultiplier=0;  //读取每字节的超时
//	TimeOuts.ReadTotalTimeoutConstant=20;   //读串口数据的固定超时
//	//设定写超时
//	TimeOuts.WriteTotalTimeoutMultiplier=100;
//	TimeOuts.WriteTotalTimeoutConstant=500;
//	SetCommTimeouts(hCom, &TimeOuts); //设置超时

	OutInforStr =L"打开 " +g_RS485Port +L" 成功!";
	Sleep(200);
	::AfxBeginThread(ReceiveRS485,THREAD_PRIORITY_NORMAL);
	return 1;
}

void CloseRS485()
{
	if(hCom!=(HANDLE)-1)
	{
		CloseHandle(hCom);
	}
	hCom =(HANDLE)-1;
	Sleep(200);
}

void ProcessData()
{
	float ftemp;
	theApp.m_dwComData = GetTickCount();
	if (iFlagComm!=3)
	{
		iFlagComm++;
		return;
	}
	iFlagComm = 0;
	int value = a5 + a6 * 256;
	//theApp.m_fHeight = value;
	//return;
	if (value<479)
	{
		value = -5000;
	}
	if (value>3180)
	{
		value = -5000;
	}
	//theApp.m_fHeight =  (float)(value - 479)* 0.944095 + 450;//(3000-450)/(3180-479)*(x-479)+450
	ftemp = theApp.m_fa1_distance*(float)value + theApp.m_fa0_distance;      //1.64m
	//ftemp = (float)value ;
	theApp.m_distance = ftemp;
	if (ftemp<1000 || ftemp>2000)
	{
		return;
	}
	memcpy(theApp.m_fHeightValue, theApp.m_fHeightValue + 1, (HEIGHT_NUM - 1) * 4);
	theApp.m_fHeightValue[HEIGHT_NUM - 1] = ftemp;
	if (theApp.m_pClientSocket->SHYx(33) == 2)
	{
		if (theApp.m_fHeightValue[HEIGHT_NUM - 1] > (theApp.m_fHeightValue[HEIGHT_NUM - 2] + 20))
		{
			theApp.m_fHeightValue[HEIGHT_NUM - 1] = (theApp.m_fHeightValue[HEIGHT_NUM - 2] + 20);
		}
		if (theApp.m_fHeightValue[HEIGHT_NUM - 1] < (theApp.m_fHeightValue[HEIGHT_NUM - 2] - 20))
		{
			theApp.m_fHeightValue[HEIGHT_NUM - 1] = (theApp.m_fHeightValue[HEIGHT_NUM - 2] - 20);
		}
	}
}

void data_handle()
{
}

void ProcRxdData(int count, BYTE *pbVal)
{
	int m_cbRxdData,i;
	bool m_bIsReceive;
	static int m_iChecksum=0;
	static int m_iStep=0;

	// printf("\nPackeg:  ");
	for(i=0;i<count;i++)
	{
		m_cbRxdData=pbVal[i] & 0xFF;
		// printf("%2x  ",m_cbRxdData);
		switch(m_iStep)
		{
		case 0:
			if(m_cbRxdData==0xfe)
			{
				m_iStep=1;
				m_iChecksum=0;
			}
			else
			{
				printf("\nPackeg error  ");
				m_bIsReceive = FALSE;
			}
			break;
		case 1:
			if(m_cbRxdData==0xfe)
				m_iStep=2;
			else
			{
				m_iStep=0;
				printf("\nPackeg error  ");
				m_bIsReceive = FALSE;
			}
			break;
		case 2:
			if(m_cbRxdData==0xfe)
				m_iStep=3;
			else
			{
				m_iStep=0;
				printf("\nPackeg error  ");
				m_bIsReceive = FALSE;
			}
			break;
		case 3:
			if(m_cbRxdData==0x68)
			{
				m_iStep=4;
				m_iChecksum=0;
			}
			else
			{
				m_iStep=0;
				printf("\nPackeg error  ");
				m_bIsReceive = FALSE;
			}
			break;
		case 4:
			a1=m_cbRxdData;
			m_iChecksum=m_cbRxdData;
			m_iStep=5;
			break;
		case 5:
			a2=m_cbRxdData;
			m_iChecksum+=m_cbRxdData;
			m_iStep=6;
			break;
		case 6:
			a3=m_cbRxdData;
			m_iChecksum+=m_cbRxdData;
			m_iStep=7;
			break;
		case 7:
			a4=m_cbRxdData;
			m_iChecksum+=m_cbRxdData;
			m_iStep=8;
			break;
		case 8:
			a5 = m_cbRxdData;
			m_iChecksum += m_cbRxdData;
			m_iStep = 9;
			break;
		case 9:
			a6 = m_cbRxdData;
			m_iChecksum += m_cbRxdData;
			m_iStep = 10;
			break;
		case 10:
			a7 = m_cbRxdData;
			m_iChecksum += m_cbRxdData;
			m_iStep = 11;
			break;
		case 11:
			a8 = m_cbRxdData;
			m_iChecksum += m_cbRxdData;
			m_iStep = 12;
			break;

		case 12:
			if((m_iChecksum & 0xFF)==m_cbRxdData)
				m_iStep = 13;
			else
			{
				m_iStep = 0;
				printf("\nChecksum error!");
				m_bIsReceive = FALSE;
			}
			break;
		case 13:
			if(m_cbRxdData == 0x16)
			{                
				m_bIsReceive = TRUE;
				ProcessData();
			}
			m_iStep=0;
			break;
		}
	}
}

//**************************普通数据请求***************************//

int SendReadMacAddr() //读设备地址
{
	unsigned char sendbuff[9]={0xFE,0xFE,0xFE,0x68,0xFF,0x2F,0x00,0xCC,0x16};
	int i=0,checksum=0;
	for(i=4; i<=6; i++)
	{
		checksum=checksum+sendbuff[i];
	}
	sendbuff[i++]=(unsigned char)(checksum & 0xFF);
	return SendRS485((char *)sendbuff,sizeof(sendbuff));
}

int SendBeginEndGet(BYTE sign) //开始/停止采集
{
	return 1;
}

int SendReadVolData() //读电压数据 
{
	return 1;
}

int SendReadVolBianbi() //读电压档位与电压变比
{
	return 1;
}

int SendReadLingpianXishu() //读电压零偏、系数
{
	return 1;
}

int SendReadCurData() //读电流数据
{
	return 1;
}

int SendReadCurLingpianXishu() //读电流零偏、系数
{
	return 1;
}

int SendWriteLingpianXishu(double flingpian,double fxishu1,double fxishu2,double fxishu3) //写电压零偏、系数
{
	return 1;
}

int SendWriteCurLingpianXishu(double flingpian,double fxishu) //写电流零偏、系数
{
	return 1;
}

int SendWriteVolBianbi(double fbianbi) //写电压变比
{
	return 1;
}


int SendWriteNGData(int data1, int data2) //写不合格
{
	unsigned char sendbuff[8]={0xFE,0xFE,0xFE,0x68,0x00,0x00,0xCC,0x16};

	int i=0,checksum=0;
	for(i=4; i<=5; i++)
	{
		checksum=checksum+sendbuff[i];
	}
	sendbuff[i++]=(unsigned char)(checksum & 0xFF);
	return SendRS485((char *)sendbuff,sizeof(sendbuff));
}
