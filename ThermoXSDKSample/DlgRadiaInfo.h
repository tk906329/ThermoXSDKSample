#pragma once
#include "afxcmn.h"


// CDlgRadiaInfo �Ի���

class CDlgRadiaInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRadiaInfo)

public:
	CDlgRadiaInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRadiaInfo();

	int m_iTongNo, m_iNum;
	void freshList();
	void updateRadia(int iNo,int iHeight,int iRadia);
// �Ի�������
	enum { IDD = IDD_DIALOG_RADIAINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton7();
};
