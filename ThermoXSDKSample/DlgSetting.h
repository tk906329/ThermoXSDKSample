#pragma once
#include "afxwin.h"


// CDlgSetting �Ի���

class CDlgSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetting)

public:
	CDlgSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetting();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_combo;
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnBnClickedButton2();
	int m_ix;
	int m_iWidth;
	int m_iy;
	int m_iHeight;
	float m_Tmax;
	float m_Tmin;
	float m_TRobot;
	float m_iHeightStep1;
	int m_iHeightMax;
	float m_TEmergency;
	afx_msg void OnBnClickedButton3();
	int m_iHeightMax2;
	int m_iFlag1;
	afx_msg void OnBnClickedCheckVision();
};
