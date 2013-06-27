#pragma once

#include <opencv2/opencv.hpp>
#include <omp.h>

int otsu(cv::Mat &src);
void ToBinary( cv::Mat & im, uchar val);
