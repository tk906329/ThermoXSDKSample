#pragma once


// CDlgHeatImage �Ի���

class CDlgHeatImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHeatImage)

public:
	CDlgHeatImage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHeatImage();

	int m_ecl_x, m_ecl_y;
	float m_radia;
	float m_ass;

	CRgn m_rgn;
	CPoint m_mousepoint;
// �Ի�������
	enum { IDD = IDD_DIALOG_REALIMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
