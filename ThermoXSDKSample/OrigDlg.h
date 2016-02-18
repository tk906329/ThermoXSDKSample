#pragma once
#include "afxcmn.h"


// COrigDlg 对话框

class COrigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COrigDlg)

public:
	COrigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrigDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ORIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int m_ix;
	int m_iy;
	virtual BOOL OnInitDialog();
	CListCtrl m_list;
	CProgressCtrl m_progress;
};
