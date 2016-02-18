#pragma once

extern CString		OutInforStr;
// MyConnect

void ProcRxdData(int count, BYTE *pbVal);
void ProcessData();
void data_handle();

int		HexToChar(BYTE *pData,char *ReBuff,DWORD DataSize);
int		SendRS485(char *sendbuf,int len);
UINT	OpenRS485(CString RS485Port, int RS485Baud);
void	CloseRS485();

int		SendReadMacAddr(); //读设备地址
int		SendBeginEndGet(BYTE sign); //开始/停止采集
int		SendReadVolData(); //读电压数据
int		SendReadCurData(); //读电流数据
int		SendReadVolBianbi(); //读电压档位与电压变比
int		SendReadLingpianXishu(); //读电压零偏、系数
int		SendReadCurLingpianXishu(); //读电流零偏、系数
int		SendWriteLingpianXishu(double flingpian,double fxishu1,double fxishu2,double fxishu3); //写零偏、系数
int		SendWriteCurLingpianXishu(double flingpian,double fxishu); //写电流零偏、系数
int		SendWriteVolBianbi(double fbianbi); //写电压变比

int		SendWriteNGData(int data1, int data2); //写不合格
