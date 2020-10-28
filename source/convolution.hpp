#pragma once

/* --------------------------------------------------------- System includes */

#include <opencv2/opencv.hpp>

/* -------------------------------------------------------------- Namespaces */

using namespace cv;

/* -------------------------------------------------------------- Exceptions */
/* --------------------------------------------------------------------- API */

namespace Convolution {

	void BoxBlur(const Mat &input, Mat &output, int amount);
	void EdgeDetect(const Mat &input, Mat &output);
};
