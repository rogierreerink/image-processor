/* ---------------------------------------------------------- Local includes */

#include "median.hpp"

/* --------------------------------------------------------- System includes */

#include <vector>

/* -------------------------------------------------------------- Namespaces */

using namespace std;

/* -------------------------------------------------------------- Exceptions */
/* ------------------------------------------------------------- Private API */

/**
 * @brief For solving the border problem.
 */
template <typename T>
static Mat BorderCopy(Mat &input) {

	Mat temp = Mat::zeros(input.rows + 2, input.cols + 2, input.type());

	/* Copy the left and right sides. */
	for (int row = 0; row < input.rows; row++) {
		temp.at<T>(row + 1, 0) = 
			input.at<T>(row, 0);
		temp.at<T>(row + 1, temp.cols - 1) = 
			input.at<T>(row, input.cols - 1);
	}

	/* Copy the top and bottom sides. */
	for (int col = 0; col < input.cols; col++) {
		temp.at<T>(0, col + 1) = 
			input.at<T>(0, col);
		temp.at<T>(temp.rows - 1, col + 1) = 
			input.at<T>(input.rows - 1, col);
	}

	/* Copy the corners. */
	temp.at<T>(0, 0) = 
		input.at<T>(0, 0);
	temp.at<T>(temp.rows - 1, 0) = 
		input.at<T>(input.rows - 1, 0);
	temp.at<T>(0, temp.cols - 1) = 
		input.at<T>(0, input.cols - 1);
	temp.at<T>(temp.rows - 1, temp.cols - 1) = 
		input.at<T>(input.rows - 1, input.cols - 1);

	/* Copy the rest of the image. */
	for (int row = 0; row < input.rows; row++) {
		for (int col = 0; col < input.cols; col++) {
			temp.at<T>(row + 1, col + 1) = 
				input.at<T>(row, col);
		}
	}

	return temp;
}

/**
 * @brief Sort the content of a vector of vectors on each's summed value.
 * @typename T Type of each vector element of the vector.
 * @typename N Number of elements per vector element.
 * @typename S Type should be able to hold the sum of each vector's elements.
 */
template <typename T, unsigned N, typename S>
static void BrightnessSort(vector<Vec<T,N>> &input, vector<Vec<T,N>> &output) {

	output.clear();
	while (input.size()) {

		S brightnessMax = 0;
		auto it = input.begin();
		auto itMax = it;
		while (it != input.end()) {

			S brightness = 0;
			for (int channel = 0; channel < N; channel++) {
				brightness += (*it)[channel];
			}

			if (brightness >= brightnessMax) {
				brightnessMax = brightness;
				itMax = it;
			}
			
			it++;
		}
		
		output.push_back(*itMax);
		input.erase(itMax);
	}
}

/**
 * @brief Sort the content of a vector.
 * @typename T Type of the vector elements.
 */
template <typename T>
static void NormalSort(vector<T> &input, vector<T> &output) {

	while (input.size()) {

		auto it = input.begin();
		auto itMax = it;
		while (it != input.end()) {

			if (*it >= *itMax) {
				itMax = it;
			}
			
			it++;
		}
		
		output.push_back(*itMax);
		input.erase(itMax);
	}
}

/* -------------------------------------------------------------- Public API */

void Median::Rectangle(const Mat &input, Mat &output, int width, int height) {

	int border = max((height - 1) / 2, (width - 1) / 2);
	
	/* Expand borders to solve border problem. */
	Mat temp = input;
	for (int i = 0; i < border; i++) {
		temp = BorderCopy<Vec3b>(temp);
	}

	/* Preallocate vector memory. */
	vector<uchar> area;
	vector<uchar> areaSorted;
	area.reserve(width * height);
	areaSorted.reserve(width * height);

	/* Apply median filter. */
	for (int channel = 0; channel < 3; channel++) {
		for (int row = 0; row < output.rows; row++) {
			for (int col = 0; col < output.cols; col++) {

				area.clear();
				int tempRow = row + border;
				int tempCol = col + border;
				for (int areaRow = - border; areaRow < height - border; areaRow++) {
					for (int areaCol = - border; areaCol < width - border; areaCol++) {
						area.push_back(temp.at<Vec3b>
							(tempRow + areaRow, tempCol + areaCol)[channel]);
					}
				}

				areaSorted.clear();
				NormalSort<uchar>(area, areaSorted);
				output.at<Vec3b>(row, col)[channel]
					= areaSorted[areaSorted.size() / 2];
			}
		}
	}

	/* Apply median filter. */
	// Mat result(input.rows, input.cols, input.type());
	// for (int row = border; row < temp.rows - border; row++) {
	// 	for (int col = border; col < temp.cols - border; col++) {
			
	// 		vector<Vec3b> area;
	// 		for (int areaRow = - border; areaRow < height - border; areaRow++) {
	// 			for (int areaCol = - border; areaCol < width - border; areaCol++) {
	// 				area.push_back(
	// 					temp.at<Vec3b>(row + areaRow, col + areaCol));
	// 			}
	// 		}

	// 		vector<Vec3b> areaSorted;
	// 		BrightnessSort<uchar, 3, int>(area, areaSorted);
	// 		result.at<Vec3b>(row - border, col - border)
	// 			= areaSorted[areaSorted.size() / 2];
	// 	}
	// }
}

void Median::Square(const Mat &input, Mat &output, int side) {
	Median::Rectangle(input, output, side, side);
}
