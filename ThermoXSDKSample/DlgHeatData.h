#pragma once
#include "afxcmn.h"


// CDlgHeatData 对话框

class CDlgHeatData : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHeatData)

public:
	CDlgHeatData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHeatData();

// 对话框数据
	enum { IDD = IDD_DIALOG_HEATDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void refreshList();

	DECLARE_MESSAGE_MAP()
public:
	int m_num;
	CListCtrl m_list;
	void show(struct_Heatinfo* pinfo);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
