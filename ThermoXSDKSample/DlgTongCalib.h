#pragma once
#include "afxcmn.h"


// CDlgTongCalib 对话框

class CDlgTongCalib : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTongCalib)

public:
	CDlgTongCalib(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTongCalib();

	int m_iTongNo;
	int m_iNum;
	void freshList();
// 对话框数据
	enum { IDD = IDD_DIALOG_TONG_CALIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
