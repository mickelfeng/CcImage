#ifndef CVTHREAD_H
#define CVTHREAD_H
#include <windows.h>
#include <opencv2/opencv.hpp>

enum  {CV_MY_GRAY, CV_MY_BINARY, CV_MY_HIS, CV_MORH_DILATE , GRABT_CUT};

unsigned int __stdcall doGray(void * arg)
{
	cv::Mat  *img = (cv::Mat*) arg;
	if(img->channels() == 3)
	{
		cv::Mat gray ;
		cv::cvtColor(*img,  gray, CV_BGR2GRAY);
		gray.copyTo(*img);
	}
	return 0;
} 

unsigned int __stdcall doGrabcut(void *arg)
{
	cv::Mat  *m_image = (cv::Mat*) arg;
	cv::Rect rectangle(50,100,m_image->cols-80,m_image->rows-190);

	cv::Mat result; // segmentation result (4 possible values)
	cv::Mat bgModel,fgModel; // the models (internally used)
	// GrabCut segmentation
	cv::grabCut(*m_image,    // input image
		result,   // segmentation result
		rectangle,// rectangle containing foreground 
		bgModel,fgModel, // models
		1,        // number of iterations
		cv::GC_INIT_WITH_RECT); // use rectangle

	// Get the pixels marked as likely foreground
	cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
	// Generate output image
	cv::Mat foreground(m_image->size(),CV_8UC3,cv::Scalar(255,255,255));
	m_image->copyTo(foreground,result); // bg pixels not copied
	cv::namedWindow("Segmented Image");
	cv::imshow("Segmented Image",foreground);
	cv::waitKey(0);
	return 0;
}

bool DispatchDo(cv::Mat & src,int what)
{
	bool exitcode = false;
	HANDLE hthread = INVALID_HANDLE_VALUE;
	switch(what)
	{
	case CV_MY_GRAY:
		hthread = (HANDLE)_beginthreadex(NULL, 0, &doGray, (void*)&src, 0, NULL);
		break;	
	case CV_MORH_DILATE:
		{
			cv::dilate(src, src, cv::Mat());
		}
		break;
	case GRABT_CUT:
		hthread = (HANDLE)_beginthreadex(NULL, 0, &doGrabcut, (void*)&src, 0, NULL);
		break;	
	}
	if(hthread != INVALID_HANDLE_VALUE)
	{
		WaitForSingleObject(hthread, INFINITE);
		CloseHandle(hthread);
		exitcode = true;
	}
	return exitcode;
}




#endif