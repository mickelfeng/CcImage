#include "StdAfx.h"
#include "cvalorithms.h"

int otsu(cv::Mat &src) // must graylevel image
{
	long nl = src.rows; // the total lines of the image
	long nc = src.cols;
	long NO = nc * nl;
	int h[256];
	double p[256];
#pragma omp parallel
	{
	#pragma omp sections
		{
		#pragma omp section
			memset(h, 0, sizeof(h));
		#pragma omp section
			memset(p, 0.0f, sizeof(p));
		} /*-- End of sections block --*/
	} 
	long i, j, k;
#pragma omp parallel for  shared( h,src) private(i, j)
	for( i = 0; i < nl; i++)
	{
		uchar * data = src.ptr<uchar>(i);
		for(j = 0; j < nc; j ++)
			h[data[j]]++;				
	}
	int ThresoldVal = 0;
	double MG = 0.0f, P1, M ;

#pragma omp parallel for  shared( p,h, NO) private(i)
	for(i = 0; i < 256; i++)
		p[i] = h[i] / double(NO);	
#pragma omp parallel for reduction(+:MG) shared( p) private(k)
		for( k = 0; k < 256; k++)
			MG += k * p[k];

	double fmax, fmax_max = -1;
	P1 =  M = 0.0f; // P1 = 0.0f, M = 0.0f;
	for(k = 0; k < 256; k++)
	{
		P1 += p[k], M += k*p[k];
		fmax = ((MG * P1 - M) * (MG * P1 - M))/((1-P1) * P1 + .5 );
		if(fmax > fmax_max)
			(fmax_max = fmax, ThresoldVal = k);
	}
	return ThresoldVal;
}


void ToBinary( cv::Mat & im, uchar val)
{
	long i, j;
	#pragma omp parallel for shared(im) private(i, j)
	for(i = 0; i < im.rows; i++)
	{
		uchar * data = im.ptr<uchar>(i);
		for(j = 0; j < im.cols * im.channels(); j ++ )
			data[j] = (data[j] > val) ? 0xFF : 0x0 ;	
	}
}