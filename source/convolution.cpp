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

void Convolution::EdgeDetect(const Mat &input, Mat &output) {

	const int border = 3;
	// const int kernel[7][7] = {
	// 	{-1, -2, -3, -4, -3, -2, -1},
	// 	{-2, -3, -4, -5, -4, -3, -2},
	// 	{-3, -4, -5, -6, -5, -4, -3},
	// 	{ 0,  0,  0,  0,  0,  0,  0},
	// 	{ 3,  4,  5,  6,  5,  4,  3},
	// 	{ 2,  3,  4,  5,  4,  3,  2},
	// 	{ 1,  2,  3,  4,  3,  2,  1},
	// };
	// const int kernel[7][7] = {
	// 	{  0,  0,  0, -1,  0,  0,  0},
	// 	{  0,  0, -1, -2, -1,  0,  0},
	// 	{  0, -1, -2, -3, -2, -1,  0},
	// 	{ -1, -2, -3, 40, -3, -2, -1},
	// 	{  0, -1, -2, -3, -2, -1,  0},
	// 	{  0,  0, -1, -2, -1,  0,  0},
	// 	{  0,  0,  0, -1,  0,  0,  0},
	// };
	const int kernel[7][7] = {
		{ -1, -1, -1, -1, -1, -1, -1},
		{ -1, -2, -2, -2, -2, -2, -1},
		{ -1, -2, -3, -3, -3, -2, -1},
		{ -1, -2, -3, 71, -3, -2, -1},
		{ -1, -2, -3, -3, -3, -2, -1},
		{ -1, -2, -2, -2, -2, -2, -1},
		{ -1, -1, -1, -1, -1, -1, -1},
	};

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
				for (int areaRow = - border; areaRow < border + 1; areaRow++) {
					for (int areaCol = - border; areaCol < border + 1; areaCol++) {
						sum += temp.at<Vec3b>(tempRow + areaRow, tempCol + areaCol)[channel]
							* kernel[areaRow + border][areaCol + border];
					}
				}

				if (sum < 0) {
					output.at<Vec3b>(row, col)[channel] = 0;
				} else if (sum > 255) {
					output.at<Vec3b>(row, col)[channel] = 255;
				} else {
					output.at<Vec3b>(row, col)[channel] = sum;
				}
			}
		}
	}
}
