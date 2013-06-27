
// CcImageDoc.cpp : implementation of the CCcImageDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CcImage.h"
#endif

#include "CcImageDoc.h"
#include <afxwin.h>
#include <propkey.h>
#include "LineFinder.h"
#include "cvthread.h"
#include "cvalorithms.h"
#include "water.h"
#include "corns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCcImageDoc

IMPLEMENT_DYNCREATE(CCcImageDoc, CDocument)

BEGIN_MESSAGE_MAP(CCcImageDoc, CDocument)
	ON_COMMAND(ID_EDGE_CANNY, &CCcImageDoc::OnEdgeCanny)
	ON_COMMAND(ID_COLOR_BINARY, &CCcImageDoc::OnColorBinary)
	ON_COMMAND(ID_EDGE_SCOBEL, &CCcImageDoc::OnEdgeScobel)
	ON_COMMAND(ID_EDGE_SCOBEL, &CCcImageDoc::OnEdgeScobel)
	ON_COMMAND(ID_HOUGH_LINE, &CCcImageDoc::OnHoughLine)
	ON_COMMAND(ID_HOUGH_CIRCLE, &CCcImageDoc::OnHoughCircle)
	ON_COMMAND(ID_COLOR_GRAY, &CCcImageDoc::OnColorGray)
	ON_COMMAND(ID_COLOR_OTSU, &CCcImageDoc::OnColorOtsu)
	ON_COMMAND(ID_MORPH_ERODE, &CCcImageDoc::OnMorphErode)
	ON_COMMAND(ID_MORPH_DILATE, &CCcImageDoc::OnMorphDilate)
	ON_COMMAND(ID_MORPH_CLOSING, &CCcImageDoc::OnMorphClosing)
	ON_COMMAND(ID_MORPH_OPENING, &CCcImageDoc::OnMorphOpening)
	ON_COMMAND(ID_MORPH_GARDIENT, &CCcImageDoc::OnMorphGardient)
	ON_COMMAND(ID_MORPH_GRABCUT, &CCcImageDoc::OnMorphGrabcut)
	ON_COMMAND(ID_FILTER_AVERAGE, &CCcImageDoc::OnFilterAverage)
	ON_COMMAND(ID_FILTER_GAUSSIANBLUR, &CCcImageDoc::OnFilterGaussianblur)
	ON_COMMAND(ID_FILTER_MEDIANBLUR, &CCcImageDoc::OnFilterMedianblur)
	ON_COMMAND(ID_EDGE_LAPLACE, &CCcImageDoc::OnEdgeLaplace)
//	ON_COMMAND(ID_MORPH_MYGRABCUT, &CCcImageDoc::OnMorphMygrabcut)
	ON_COMMAND(ID_TEST_TT, &CCcImageDoc::OnTestTt)
	ON_COMMAND(ID_MORPH_WATERES, &CCcImageDoc::OnMorphWateres)
	ON_COMMAND(ID_EDGE_CORNERS, &CCcImageDoc::OnEdgeCorners)
	ON_COMMAND(ID_EDGE_DIFF, &CCcImageDoc::OnEdgeDiff)
	ON_COMMAND(ID_EDGE_DIFF2, &CCcImageDoc::OnEdgeDiff2)
	ON_COMMAND(ID_COLOR_INVERT, &CCcImageDoc::OnColorInvert)
END_MESSAGE_MAP()


// CCcImageDoc construction/destruction

CCcImageDoc::CCcImageDoc()
{
	loaded = false;
	// TODO: add one-time construction code here

}

CCcImageDoc::~CCcImageDoc()
{
}

 void CCcImageDoc::DoFileOpen()
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Image Files (*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "jpg";

	if(GetOpenFileName(&ofn))
	{
		m_image = cv::imread(szFileName);
		loaded = true;
		SetPathName(szFileName);
		//SetWindowText(ofn.hwndOwner, szFileName);
	}
}

BOOL CCcImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	DoFileOpen();
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CCcImageDoc serialization

void CCcImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CCcImageDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CCcImageDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CCcImageDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CCcImageDoc diagnostics

#ifdef _DEBUG
void CCcImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCcImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCcImageDoc commands


BOOL CCcImageDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	DeleteContents();

	BeginWaitCursor();
	m_image = cv::imread(lpszPathName);
	loaded = true;
	EndWaitCursor();
	return TRUE;
}


BOOL CCcImageDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	BeginWaitCursor();
	if(!CDocument::OnSaveDocument(lpszPathName))
		return FALSE;
	cv::imwrite(lpszPathName, m_image);
	EndWaitCursor();
	return TRUE;
}


void CCcImageDoc::OnEdgeCanny()
{
	BeginWaitCursor();
	cv::GaussianBlur(m_image, m_image, cv::Size(5, 5), 1.5f);
	cv::Mat gray = m_image;
	if(m_image.channels() == 3)
		cv::cvtColor(m_image, gray, CV_BGR2GRAY);

	//gray.copyTo(m_image);
	//m_image.CopyOf((IplImage*)&a);
	cv::Canny(gray, m_image, 40, 80);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
	// TODO: Add your command handler code here
}


void CCcImageDoc::OnColorBinary()
{
	BeginWaitCursor();
	cv::Mat gray = m_image;
	if(m_image.channels() == 3)
		cv::cvtColor(m_image, gray, CV_BGR2GRAY);

	cv::threshold(gray, m_image, 100, 255 , cv::THRESH_BINARY);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}


void CCcImageDoc::OnEdgeScobel()
{
	BeginWaitCursor();
	cv::Mat gray = m_image;
	if(m_image.channels() == 3)
		cv::cvtColor(m_image, gray, CV_BGR2GRAY);
	cv::Sobel(gray, m_image, CV_8UC3, 1, 0, 3);
	m_image.convertTo(gray,CV_8U,90/PI);
	gray.copyTo(m_image);


	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}

void CCcImageDoc::OnHoughLine()
{
	BeginWaitCursor();
	cv::Mat contours, image;
	if(m_image.channels() == 3)
		cv::cvtColor(m_image, contours, CV_BGR2GRAY);
	contours.copyTo(image);
	cv::Canny(image,contours,125,350);

	cv::Mat result;
	m_image.copyTo(result);
	LineFinder finder;
	// Set probabilistic Hough parameters
	finder.setLineLengthAndGap(100,20);
	finder.setMinVote(80);
	// Detect lines and draw them
	std::vector<cv::Vec4i> lines= finder.findLines(contours);
	finder.drawDetectedLines(result, cv::Scalar(0, 0, 255));
	

	cv::namedWindow("Detected Lines with Hough");
	cv::imshow("Detected Lines with Hough",result);

	UpdateAllViews(NULL);
	EndWaitCursor();
}


void CCcImageDoc::OnHoughCircle()
{
	
	BeginWaitCursor();
	cv::Mat  image, result;
	cv::GaussianBlur(m_image,image,cv::Size(5,5),1.5);
	cv::Mat gray = image;
	if(m_image.channels() == 3)
		cv::cvtColor(image, gray, CV_BGR2GRAY);

	gray.copyTo(image);
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(image, circles, CV_HOUGH_GRADIENT, 
		1,   // accumulator resolution (size of the image / 2) 
		30,  // minimum distance between two circles
		200, // Canny high threshold 
		60, // minimum number of votes 
		5, 80); // min and max radius

	//std::cout << "Circles: " << circles.size() << std::endl;

	// Draw the circles
	
	std::vector<cv::Vec3f>::const_iterator itc= circles.begin();
	m_image.copyTo(result);
	while (itc!=circles.end()) {

		cv::circle(result, 
			cv::Point((*itc)[0], (*itc)[1]), // circle centre
			(*itc)[2], // circle radius
			cv::Scalar(0, 0, 255), // color 
			2); // thickness

		++itc;	
	}
	cv::namedWindow("Detected Circles");
	cv::imshow("Detected Circles",result);
	EndWaitCursor();
}


void CCcImageDoc::OnColorGray()
{
	BeginWaitCursor();
	DispatchDo(m_image, CV_MY_GRAY);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);

	EndWaitCursor();
}


void CCcImageDoc::OnColorOtsu()
{
	BeginWaitCursor();
	cv::Mat gray;
	if(m_image.channels() == 3)
		cv::cvtColor(m_image, gray, CV_BGR2GRAY);
	gray.copyTo(m_image);
	int thresV = otsu(gray);
	ToBinary(m_image, thresV);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();	
}


void CCcImageDoc::OnMorphErode()//∏Ø ¥
{
	BeginWaitCursor();
	cv::erode(m_image, m_image, cv::Mat());
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();	
}


void CCcImageDoc::OnMorphDilate()//≈Ú’Õ
{
	BeginWaitCursor();
	DispatchDo(m_image, CV_MORH_DILATE);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}


void CCcImageDoc::OnMorphClosing()
{
	BeginWaitCursor();
	cv::Mat element(5, 5, CV_8U, cv::Scalar(1));
	cv::Mat closed;
	cv::morphologyEx(m_image, closed, CV_MOP_CLOSE, element);
	closed.copyTo(m_image);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}


void CCcImageDoc::OnMorphOpening()
{
	BeginWaitCursor();
	cv::Mat element(5, 5, CV_8U, cv::Scalar(1));
	cv::Mat opened;
	cv::morphologyEx(m_image, opened, CV_MOP_OPEN, element);
	opened.copyTo(m_image);

	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}


void CCcImageDoc::OnMorphGardient()//–ŒÃ¨—ßÃ›∂»
{
	BeginWaitCursor();
	cv::Mat element(5, 5, CV_8U, cv::Scalar(1));
	cv::Mat gradient;
	cv::morphologyEx(m_image, gradient, CV_MOP_GRADIENT, element);
	gradient.copyTo(m_image);

	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}


void CCcImageDoc::OnMorphGrabcut()//øŸÕº
{
	
	BeginWaitCursor();
	DispatchDo(m_image, GRABT_CUT);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}


void CCcImageDoc::OnFilterAverage()
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	cv::Mat ave;
	
	cv::blur(m_image,ave,cv::Size(5,5));
	ave.copyTo(m_image);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}


void CCcImageDoc::OnFilterGaussianblur()
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	cv::Mat gau;

	cv::GaussianBlur(m_image,gau,cv::Size(5,5),1.5);
	gau.copyTo(m_image);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}


void CCcImageDoc::OnFilterMedianblur()
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	cv::Mat median;
	cv::medianBlur(m_image,median,5);
	median.copyTo(m_image);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}


void CCcImageDoc::OnEdgeLaplace()
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	cv::Mat laplace;
	cv::Laplacian(m_image,laplace,CV_8UC3);
	laplace.copyTo(m_image);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}



void CCcImageDoc::OnTestTt()
{
	cv::Rect rectangle;
	BeginWaitCursor();
	cv::Mat  image, result;
	cv::GaussianBlur(m_image,image,cv::Size(5,5),1.5);
	cv::Mat gray = image;
	if(m_image.channels() == 3)
		cv::cvtColor(image, gray, CV_BGR2GRAY);

	gray.copyTo(image);
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(image, circles, CV_HOUGH_GRADIENT, 
		1,   // accumulator resolution (size of the image / 2) 
		30,  // minimum distance between two circles
		200, // Canny high threshold 
		60, // minimum number of votes 
		5, 60); // min and max radius

	//std::cout << "Circles: " << circles.size() << std::endl;

	// Draw the circles

	std::vector<cv::Vec3f>::const_iterator itc= circles.begin();
	m_image.copyTo(result);
	while (itc!=circles.end()) {

		cv::circle(result, 
			cv::Point((*itc)[0], (*itc)[1]), // circle centre
			(*itc)[2], // circle radius
			cv::Scalar(0, 0, 255), // color 
			2); // thickness

		++itc;	
	}
	itc = circles.begin();
	cv::Point s, e,center;

	while (itc!=circles.end()) {
		 s.x = (*itc)[0], s.y = (*itc)[1];
		++itc;	

		if(itc == circles.end())
			break;
		e.x = (*itc)[0], e.y = (*itc)[1];
		cv::line(result, s, e, cv::Scalar(0, 255, 20), 2);
		double dis, sum;
		sum = (s.x - e.x)*(s.x - e.x)+(s.y - e.y)*(s.y - e.y);
		center.x = (s.x + e.x)/2;
		center.y =  (e.y + s.y)/2;
		dis = std::sqrt(sum);
	    rectangle = cv::Rect(center.x - dis*0.8, center.y - dis*0.45, dis *1.6, dis * 0.6);
		//rectangle = cv::Rect(center.x - dis*0.6, center.y - dis*0.3, dis *1.0, dis * 0.4);
		cv::rectangle(result, rectangle, cv::Scalar(255, 0, 0), 2);
		
		itc++;
	}

	
	

	
	cv::Mat bgModel,fgModel; // the models (internally used)
	// GrabCut segmentation
	cv::grabCut(m_image,    // input image
		result,   // segmentation result
		rectangle,// rectangle containing foreground 
		bgModel,fgModel, // models
		1,        // number of iterations
		cv::GC_INIT_WITH_RECT); // use rectangle

	// Get the pixels marked as likely foreground
	cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
	// Generate output image
	cv::Mat foreground(m_image.size(),CV_8UC3,cv::Scalar(255,255,255));
	
	m_image.copyTo(foreground,result); // bg pixels not copied
	foreground.copyTo(m_image);
	/*cv::namedWindow("Segmented Image");
	cv::imshow("Segmented Image",foreground);
	cv::waitKey(0);*/


	cv::namedWindow("Detected Circles");
	cv::imshow("Detected Circles",result);
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
	
}

void CCcImageDoc::OnMorphWateres()
{
	cv::Mat binary;
	if(m_image.channels() == 3)
		cv::cvtColor(m_image, binary, CV_BGR2GRAY );
	cv::threshold(binary, binary, 0, 255 , cv::THRESH_BINARY | cv::THRESH_OTSU);

	// Display the binary image
	

	// Eliminate noise and smaller objects
	cv::Mat fg;
	cv::erode(binary,fg,cv::Mat(),cv::Point(-1,-1),6);

	// Display the foreground image

	// Identify image pixels without objects
	cv::Mat bg;
	cv::dilate(binary,bg,cv::Mat(),cv::Point(-1,-1),6);
	cv::threshold(bg,bg,1,128,cv::THRESH_BINARY_INV);

	// Display the background image
	

	// Show markers image
	cv::Mat markers(binary.size(),CV_8U,cv::Scalar(0));
	markers= fg+bg;
	cv::namedWindow("Markers");
	cv::imshow("Markers",markers);

	// Create watershed segmentation object
	WatershedSegmenter segmenter;

	// Set markers and process
	segmenter.setMarkers(markers);
	segmenter.process(m_image);

	// Display segmentation result
	cv::namedWindow("Segmentation");
	cv::imshow("Segmentation",segmenter.getSegmentation());

	// Display watersheds
	cv::namedWindow("Watersheds");
	cv::imshow("Watersheds",segmenter.getWatersheds());

}


void CCcImageDoc::OnEdgeCorners()
{
	MorphoFeatures morpho;
	morpho.setThreshold(40);

	// Get the edges
	cv::Mat edges;
	cv::Mat gray = m_image;
	if(m_image.channels() == 3)
		cv::cvtColor(m_image, gray, CV_BGR2GRAY);
	gray.copyTo(m_image);
	edges= morpho.getEdges(m_image);

	// Display the edge image
	cv::namedWindow("Edge Image");
	cv::imshow("Edge Image",edges);

	//// Get the corners
	//morpho.setThreshold(-1);
	//cv::Mat corners;
	//corners= morpho.getCorners(m_image);
	//cv::morphologyEx(corners,corners,cv::MORPH_TOPHAT,cv::Mat());
	//cv::threshold(corners, corners, 40, 255, cv::THRESH_BINARY_INV);

	//// Display the corner image
	//cv::namedWindow("Corner Image");
	//cv::imshow("Corner Image",corners);

	//// Display the corner on the image
	//morpho.drawOnImage(corners,m_image);
	//cv::namedWindow("Corners on Image");
	//cv::imshow("Corners on Image",m_image);

}


void CCcImageDoc::OnEdgeDiff()
{
		BeginWaitCursor();
	CString nextImName = GetPathName();
	nextImName += ".back";
	cv::Mat nextImage = cv::imread((LPCSTR)nextImName);
	cv::Mat diff = cv::Mat(m_image.rows, m_image.cols, m_image.type());
	//cv::absdiff(m_image, nextImage, diff);
	//diff = m_image - nextImage;
	//diff.copyTo(m_image);
	for(int j = 0; j < m_image.rows; j++)
		for(int i =0; i < m_image.cols; i++)
		{
			for(int k =0; k < 3; k++)
			{
				if(m_image.at<cv::Vec3b>(j, i)[k] != nextImage.at<cv::Vec3b>(j, i)[k])
				{
					diff.at<cv::Vec3b>(j, i)[0] = m_image.at<cv::Vec3b>(j, i)[0];
					diff.at<cv::Vec3b>(j, i)[1] = m_image.at<cv::Vec3b>(j, i)[1];
					diff.at<cv::Vec3b>(j, i)[2] = m_image.at<cv::Vec3b>(j, i)[2];
					break;

				}else
				{
					diff.at<cv::Vec3b>(j, i)[k] = 0;
					
				}
			}

			
		}
	diff.copyTo(m_image);
	/*cv::namedWindow("diff Image");
	cv::imshow("diff Image",diff);*/
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();

}


void CCcImageDoc::OnEdgeDiff2()
{
		BeginWaitCursor();
	CString nextImName = GetPathName();
	nextImName += ".back";
	cv::Mat nextImage = cv::imread((LPCSTR)nextImName);
	cv::Mat diff = cv::Mat(m_image.rows, m_image.cols, m_image.type());
	cv::absdiff(m_image, nextImage, diff);
	//diff = m_image - nextImage;
	diff.copyTo(m_image);
	for(int j = 0; j < m_image.rows; j++)
		for(int i =0; i < m_image.cols; i++)
		{
			for(int k =0; k < 3; k++)
			{
				if(m_image.at<cv::Vec3b>(j, i)[k] != 0)
				{
					m_image.at<cv::Vec3b>(j, i)[0] = 255;
					m_image.at<cv::Vec3b>(j, i)[1] = 255;
					m_image.at<cv::Vec3b>(j, i)[2] = 255;
					break;

				}
			}


		}
	//cv::dilate(m_image, m_image, cv::Mat());
	/*cv::namedWindow("diff Image");
	cv::imshow("diff Image",diff);*/
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}


void CCcImageDoc::OnColorInvert()
{
	cv::Mat invert;
	BeginWaitCursor();

	for(int j = 0; j < m_image.rows; j++)
		for(int i =0; i < m_image.cols; i++)
		{
			
				
					m_image.at<cv::Vec3b>(j, i)[0] = 255 - m_image.at<cv::Vec3b>(j, i)[0];
					m_image.at<cv::Vec3b>(j, i)[1] = 255 - m_image.at<cv::Vec3b>(j, i)[1];
					m_image.at<cv::Vec3b>(j, i)[2] = 255 - m_image.at<cv::Vec3b>(j, i)[2];

		}
	
	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
	EndWaitCursor();
}
	
