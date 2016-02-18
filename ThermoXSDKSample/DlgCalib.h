#pragma once
#include "afxwin.h"


// CDlgCalib 对话框

class CDlgCalib : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCalib)

public:
	CDlgCalib(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalib();

// 对话框数据
	enum { IDD = IDD_DIALOG_CALIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
//	CEdit m_svalue1;
//	CEdit m_svalue2;
//	CEdit m_svalue3;
//	CEdit m_svalue4;
//	CEdit m_svalue5;
//	CEdit m_svalue6;
	CString m_svalue1;
	CString m_svalue2;
	CString m_svalue3;
	CString m_svalue4;
	CString m_svalue5;
	CString m_svalue6;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton5();
	CString m_svalue7;
	afx_msg void OnBnClickedButton6();
	CString m_svalue8;
	CString m_svalue9;
//	int m_fRadia1;
	CString m_svalue10;
//	int m_fRadia2;
//	int m_fHeight;
//	int m_fRadia3;
//	int m_fRadia4;
	double m_fa3_1;
	double m_fa2_1;
	double m_fa1_1;
	double m_fa0_1;
	double m_fa3_2;
	double m_fa2_2;
	double m_fa1_2;
	double m_fa0_2;
	int m_pix_min;
	int m_distance_min;
	float m_fMinus;
	int m_iRobotLen;
	float m_fa1_distance;
	float m_fa0_distance;
};
