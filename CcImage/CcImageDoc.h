
// CcImageDoc.h : interface of the CCcImageDoc class
//


#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "CvvImage.h"

class CCcImageDoc : public CDocument
{
protected: // create from serialization only
	CCcImageDoc();
	DECLARE_DYNCREATE(CCcImageDoc)

// Attributes
public:
	bool loaded;
	cv::Mat m_image;
// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CCcImageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void DoFileOpen();
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnEdgeCanny();
	afx_msg void OnColorBinary();
//	afx_msg void OnEdgeScobel();
	afx_msg void OnEdgeScobel();
	afx_msg void OnHoughLine();
	afx_msg void OnHoughCircle();
	afx_msg void OnColorGray();
	afx_msg void OnColorOtsu();
	afx_msg void OnMorphErode();
	afx_msg void OnMorphDilate();
	afx_msg void OnMorphClosing();
	afx_msg void OnMorphOpening();
	afx_msg void OnMorphGardient();
	afx_msg void OnMorphGrabcut();
	afx_msg void OnFilterAverage();
	afx_msg void OnFilterGaussianblur();
	afx_msg void OnFilterMedianblur();
	afx_msg void OnEdgeLaplace();
//	afx_msg void OnMorphMygrabcut();
	afx_msg void OnTestTt();
	afx_msg void OnMorphWateres();
	afx_msg void OnEdgeCorners();
	afx_msg void OnEdgeDiff();
	afx_msg void OnEdgeDiff2();
	afx_msg void OnColorInvert();
};
