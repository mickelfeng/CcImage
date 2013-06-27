
// CcImageView.h : interface of the CCcImageView class
//

#pragma once


class CCcImageView : public CScrollView
{
protected: // create from serialization only
	CCcImageView();
	DECLARE_DYNCREATE(CCcImageView)

// Attributes
public:
	CCcImageDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CCcImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
};

#ifndef _DEBUG  // debug version in CcImageView.cpp
inline CCcImageDoc* CCcImageView::GetDocument() const
   { return reinterpret_cast<CCcImageDoc*>(m_pDocument); }
#endif

