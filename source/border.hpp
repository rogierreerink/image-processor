#pragma once

/* --------------------------------------------------------- System includes */

#include <opencv2/opencv.hpp>

/* -------------------------------------------------------------- Namespaces */

using namespace cv;

/* -------------------------------------------------------------- Exceptions */
/* --------------------------------------------------------------------- API */

namespace Border {

	Mat Copy(const Mat &input);
};
