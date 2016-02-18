// DlgData.cpp : 实现文件
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "DlgData.h"
#include "afxdialogex.h"


// CDlgData 对话框

IMPLEMENT_DYNAMIC(CDlgData, CDialog)

CDlgData::CDlgData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgData::IDD, pParent)
{

}

CDlgData::~CDlgData()
{
}

void CDlgData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgData, CDialog)
END_MESSAGE_MAP()


// CDlgData 消息处理程序
