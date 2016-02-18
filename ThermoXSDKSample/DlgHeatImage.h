#pragma once


// CDlgHeatImage 对话框

class CDlgHeatImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHeatImage)

public:
	CDlgHeatImage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHeatImage();

	int m_ecl_x, m_ecl_y;
	float m_radia;
	float m_ass;

	CRgn m_rgn;
	CPoint m_mousepoint;
// 对话框数据
	enum { IDD = IDD_DIALOG_REALIMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
