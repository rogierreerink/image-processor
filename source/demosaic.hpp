#pragma once

/* --------------------------------------------------------- System includes */

#include <opencv2/opencv.hpp>

/* -------------------------------------------------------------- Namespaces */

using namespace cv;

/* -------------------------------------------------------------- Exceptions */
/* --------------------------------------------------------------------- API */

namespace Demosaic {

	void Bayer(const Mat &input, Mat &output);
};
