#pragma once
#include "afxcmn.h"


// CDlgHeatData �Ի���

class CDlgHeatData : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHeatData)

public:
	CDlgHeatData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHeatData();

// �Ի�������
	enum { IDD = IDD_DIALOG_HEATDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void refreshList();

	DECLARE_MESSAGE_MAP()
public:
	int m_num;
	CListCtrl m_list;
	void show(struct_Heatinfo* pinfo);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
