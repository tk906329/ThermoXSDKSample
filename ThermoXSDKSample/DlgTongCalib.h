#pragma once
#include "afxcmn.h"


// CDlgTongCalib �Ի���

class CDlgTongCalib : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTongCalib)

public:
	CDlgTongCalib(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTongCalib();

	int m_iTongNo;
	int m_iNum;
	void freshList();
// �Ի�������
	enum { IDD = IDD_DIALOG_TONG_CALIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
