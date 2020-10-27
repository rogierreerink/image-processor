#pragma once

/* --------------------------------------------------------- System includes */

#include <opencv2/opencv.hpp>

/* -------------------------------------------------------------- Namespaces */

using namespace cv;

/* -------------------------------------------------------------- Exceptions */
/* --------------------------------------------------------------------- API */

namespace Pixel {

	void Brightness(Mat &image, int shift);
	void Contrast(Mat &image, float factor);
	void Gamma(Mat &image, float power);
};
