#pragma once
#include "afxcmn.h"


// COrigDlg �Ի���

class COrigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COrigDlg)

public:
	COrigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COrigDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ORIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int m_ix;
	int m_iy;
	virtual BOOL OnInitDialog();
	CListCtrl m_list;
	CProgressCtrl m_progress;
};
