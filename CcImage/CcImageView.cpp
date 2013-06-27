
// CcImageView.cpp : implementation of the CCcImageView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CcImage.h"
#endif

#include "CcImageDoc.h"
#include "CcImageView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCcImageView

IMPLEMENT_DYNCREATE(CCcImageView, CScrollView)

BEGIN_MESSAGE_MAP(CCcImageView, CScrollView)
	ON_WM_ERASEBKGND()

END_MESSAGE_MAP()

// CCcImageView construction/destruction

CCcImageView::CCcImageView()
{
	// TODO: add construction code here

}

CCcImageView::~CCcImageView()
{
}

BOOL CCcImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CCcImageView drawing

void CCcImageView::OnDraw(CDC* pDC)
{
	CCcImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if(pDoc->loaded)
	{
		IplImage imgtmp = pDoc->m_image;
		CvvImage * img = new CvvImage();
		//img->Create( imgt.width,  imgt.height, (imgt.depth & 255)*imgt.nChannels, imgt.origin);
		img->CopyOf(&imgtmp, 3);
		if(img != nullptr)
		{
			CRect r;
			SetScrollSizes(MM_TEXT, CSize(img->Width() + 40, img->Height()+40));
			CRect rect;
			rect.top = 20;
			rect.left = 20;
			rect.bottom = img->Height() + 20;
			rect.right = img->Width() + 20;
			img->DrawToHDC(pDC->GetSafeHdc(), &rect);
			delete img;	
		}
	
	}

}

void CCcImageView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CCcImageView diagnostics

#ifdef _DEBUG
void CCcImageView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCcImageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCcImageDoc* CCcImageView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCcImageDoc)));
	return (CCcImageDoc*)m_pDocument;
}
#endif //_DEBUG


// CCcImageView message handlers


BOOL CCcImageView::OnEraseBkgnd(CDC* pDC)
{
	if(!CScrollView::OnEraseBkgnd(pDC))
		return FALSE;
	CRect rect;
	CBrush brush;
	brush.CreateSolidBrush(RGB(127, 127, 127));
	//brush.CreateSolidBrush(RGB(20, 20, 20));
	pDC->GetClipBox(rect);
	pDC->FillRect(rect,&brush);
	return TRUE;
}



