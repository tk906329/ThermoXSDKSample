#pragma once


// CDlgData 对话框

class CDlgData : public CDialog
{
	DECLARE_DYNAMIC(CDlgData)

public:
	CDlgData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgData();

// 对话框数据
	enum { IDD = IDD_DIALOG_RUNSTATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
