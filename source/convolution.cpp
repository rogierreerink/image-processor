/* ---------------------------------------------------------- Local includes */

#include "border.hpp"
#include "convolution.hpp"

/* --------------------------------------------------------- System includes */

#include <iostream>

/* -------------------------------------------------------------- Namespaces */
/* -------------------------------------------------------------- Exceptions */
/* ------------------------------------------------------------- Private API */
/* -------------------------------------------------------------- Public API */

void Convolution::BoxBlur(const Mat &input, Mat &output, int amount) {

	const int border = (amount - 1) / 2;

	/* Expand borders to solve border problem. */
	Mat temp = input.clone();
	for (int i = 0; i < border; i++) {
		temp = Border::Copy(temp);
	}

	/* Apply filter. */
	for (int channel = 0; channel < output.channels(); channel++) {
		for (int row = 0; row < output.rows; row++) {
			for (int col = 0; col < output.cols; col++) {

				int sum = 0;
				const int tempRow = row + border;
				const int tempCol = col + border;
				for (int areaRow = - border; areaRow < amount - border; areaRow++) {
					for (int areaCol = - border; areaCol < amount - border; areaCol++) {
						sum += temp.at<Vec3b>
							(tempRow + areaRow, tempCol + areaCol)[channel];
					}
				}

				output.at<Vec3b>(row, col)[channel]
					= (float)sum / (amount * amount);
			}
		}
	}
}
