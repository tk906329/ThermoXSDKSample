// TIDoc.h : interface of the CTIDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIDOC_H__17DB0367_A7A2_4B52_8BDC_DAE226DEE8B8__INCLUDED_)
#define AFX_TIDOC_H__17DB0367_A7A2_4B52_8BDC_DAE226DEE8B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTIDoc : public CDocument
{
protected: // create from serialization only
	CTIDoc();
	DECLARE_DYNCREATE(CTIDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTIDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	float m_fDefaultEmissivity;
	BOOL m_bCorrectTemperature;
	virtual ~CTIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTIDoc)
	afx_msg void OnFileSaveddt();
	afx_msg void OnFileLoadddt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIDOC_H__17DB0367_A7A2_4B52_8BDC_DAE226DEE8B8__INCLUDED_)
