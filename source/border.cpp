/* ---------------------------------------------------------- Local includes */

#include "border.hpp"

/* --------------------------------------------------------- System includes */
/* -------------------------------------------------------------- Namespaces */
/* -------------------------------------------------------------- Exceptions */
/* ------------------------------------------------------------- Private API */
/* -------------------------------------------------------------- Public API */

/**
 * @brief For solving the border problem.
 */
Mat Border::Copy(const Mat &input) {

	Mat temp = Mat::zeros(
		input.rows + 2, input.cols + 2, input.type());

	/* Copy the left and right sides. */
	for (int row = 0; row < input.rows; row++) {
		temp.at<Vec3b>(row + 1, 0) = 
			input.at<Vec3b>(row, 0);
		temp.at<Vec3b>(row + 1, temp.cols - 1) = 
			input.at<Vec3b>(row, input.cols - 1);
	}

	/* Copy the top and bottom sides. */
	for (int col = 0; col < input.cols; col++) {
		temp.at<Vec3b>(0, col + 1) = 
			input.at<Vec3b>(0, col);
		temp.at<Vec3b>(temp.rows - 1, col + 1) = 
			input.at<Vec3b>(input.rows - 1, col);
	}

	/* Copy the corners. */
	temp.at<Vec3b>(0, 0) = 
		input.at<Vec3b>(0, 0);
	temp.at<Vec3b>(temp.rows - 1, 0) = 
		input.at<Vec3b>(input.rows - 1, 0);
	temp.at<Vec3b>(0, temp.cols - 1) = 
		input.at<Vec3b>(0, input.cols - 1);
	temp.at<Vec3b>(temp.rows - 1, temp.cols - 1) = 
		input.at<Vec3b>(input.rows - 1, input.cols - 1);

	/* Copy the rest of the image. */
	for (int row = 0; row < input.rows; row++) {
		for (int col = 0; col < input.cols; col++) {
			temp.at<Vec3b>(row + 1, col + 1) = 
				input.at<Vec3b>(row, col);
		}
	}

	return temp;
}
