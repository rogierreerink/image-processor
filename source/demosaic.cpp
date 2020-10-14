/* ---------------------------------------------------------- Local includes */

#include "demosaic.hpp"

/* --------------------------------------------------------- System includes */
/* -------------------------------------------------------------- Namespaces */
/* -------------------------------------------------------------- Exceptions */
/* ------------------------------------------------------------- Private API */

/**
 * @brief Expand the border of a raw image for demosaicing.
 * @note Uses only the first channel in a matrix.
 */
static Mat RawBorderExpand(const Mat &input) {

	Mat result(input.rows + 2, input.cols + 2, input.type());

	/* Copy the left and right sides. */
	for (int row = 0; row < input.rows; row++) {
		result.at<Vec3b>(row + 1, 0)[0] 
			= input.at<Vec3b>(row, 1)[0];
		result.at<Vec3b>(row + 1, result.cols - 1)[0] 
			= input.at<Vec3b>(row, input.cols - 2)[0];
	}

	/* Copy the top and bottom sides. */
	for (int col = 0; col < input.cols; col++) {
		result.at<Vec3b>(0, col + 1)[0] 
			= input.at<Vec3b>(1, col)[0];
		result.at<Vec3b>(result.rows - 1, col + 1)[0] 
			= input.at<Vec3b>(input.rows - 2, col)[0];
	}

	/* Copy the corners. */
	result.at<Vec3b>(0, 0)[0]
		= input.at<Vec3b>(1, 1)[0];
	result.at<Vec3b>(0, result.cols - 1)[0]
		= input.at<Vec3b>(1, input.cols - 2)[0];
	result.at<Vec3b>(result.rows - 1, 0)[0]
		= input.at<Vec3b>(input.rows - 2, 1)[0];
	result.at<Vec3b>(result.rows - 1, result.cols - 1)[0]
		= input.at<Vec3b>(input.rows - 2, input.cols - 2)[0];

	/* Copy the rest of the image. */
	for (int row = 0; row < input.rows; row++) {
		for (int col = 0; col < input.cols; col++) {
			result.at<Vec3b>(row + 1, col + 1)[0]
				= input.at<Vec3b>(row, col)[0];
		}
	}

	return result;
}

/* -------------------------------------------------------------- Public API */

void Demosaic::Bayer(const Mat &input, Mat &output) {

	Mat temp = RawBorderExpand(input);

	for (int row = 0; row < output.rows; row++) {
		for (int col = 0; col < output.cols; col++) {

			const int tempRow = row + 1;
			const int tempCol = col + 1;

			int blue = 0;
			int green = 0;
			int red = 0;

			if ((col % 2) && (row % 2)) {

				blue = (temp.at<Vec3b>(tempRow - 1, tempCol - 1)[0]
					+ temp.at<Vec3b>(tempRow - 1, tempCol + 1)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol - 1)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol + 1)[0]) / 4;

				green = (temp.at<Vec3b>(tempRow - 1, tempCol)[0]
					+ temp.at<Vec3b>(tempRow, tempCol - 1)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol)[0]
					+ temp.at<Vec3b>(tempRow, tempCol + 1)[0]) / 4;
				
				red = temp.at<Vec3b>(tempRow, tempCol)[0];

			} else if ((col % 2) && !(row % 2)) {

				blue = (temp.at<Vec3b>(tempRow, tempCol - 1)[0]
					+ temp.at<Vec3b>(tempRow, tempCol + 1)[0]) / 2;

				green = (temp.at<Vec3b>(tempRow, tempCol)[0]
					+ temp.at<Vec3b>(tempRow - 1, tempCol + 1)[0]
					+ temp.at<Vec3b>(tempRow - 1, tempCol + 1)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol - 1)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol + 1)[0]) / 5;

				red = (temp.at<Vec3b>(tempRow - 1, tempCol)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol)[0]) / 2;

			} else if (!(col % 2) && (row % 2)) {

				blue = (temp.at<Vec3b>(tempRow - 1, tempCol)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol)[0]) / 2;

				green = (temp.at<Vec3b>(tempRow, tempCol)[0]
					+ temp.at<Vec3b>(tempRow - 1, tempCol + 1)[0]
					+ temp.at<Vec3b>(tempRow - 1, tempCol + 1)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol - 1)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol + 1)[0]) / 5;

				red = (temp.at<Vec3b>(tempRow, tempCol - 1)[0]
					+ temp.at<Vec3b>(tempRow, tempCol + 1)[0]) / 2;


			} else if (!(col % 2) && !(row % 2)) {
				
				blue = temp.at<Vec3b>(tempRow, tempCol)[0];

				green = (temp.at<Vec3b>(tempRow - 1, tempCol)[0]
					+ temp.at<Vec3b>(tempRow, tempCol - 1)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol)[0]
					+ temp.at<Vec3b>(tempRow, tempCol + 1)[0]) / 4;

				red = (temp.at<Vec3b>(tempRow - 1, tempCol - 1)[0]
					+ temp.at<Vec3b>(tempRow - 1, tempCol + 1)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol - 1)[0]
					+ temp.at<Vec3b>(tempRow + 1, tempCol + 1)[0]) / 4;

			}

			output.at<Vec3b>(row, col)
				= {(uchar)blue, (uchar)green, (uchar)red};
		}
	}
}
