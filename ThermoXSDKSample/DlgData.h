#pragma once


// CDlgData �Ի���

class CDlgData : public CDialog
{
	DECLARE_DYNAMIC(CDlgData)

public:
	CDlgData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgData();

// �Ի�������
	enum { IDD = IDD_DIALOG_RUNSTATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
