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

int		SendReadMacAddr(); //���豸��ַ
int		SendBeginEndGet(BYTE sign); //��ʼ/ֹͣ�ɼ�
int		SendReadVolData(); //����ѹ����
int		SendReadCurData(); //����������
int		SendReadVolBianbi(); //����ѹ��λ���ѹ���
int		SendReadLingpianXishu(); //����ѹ��ƫ��ϵ��
int		SendReadCurLingpianXishu(); //��������ƫ��ϵ��
int		SendWriteLingpianXishu(double flingpian,double fxishu1,double fxishu2,double fxishu3); //д��ƫ��ϵ��
int		SendWriteCurLingpianXishu(double flingpian,double fxishu); //д������ƫ��ϵ��
int		SendWriteVolBianbi(double fbianbi); //д��ѹ���

int		SendWriteNGData(int data1, int data2); //д���ϸ�
