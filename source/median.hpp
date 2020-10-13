#pragma once

/* --------------------------------------------------------- System includes */

#include <opencv2/opencv.hpp>

/* -------------------------------------------------------------- Namespaces */

using namespace cv;

/* -------------------------------------------------------------- Exceptions */
/* --------------------------------------------------------------------- API */

namespace Median {

	void Rectangle(const Mat &input, Mat &output, int width, int height);
	void Square(const Mat &input, Mat &output, int side);
};
