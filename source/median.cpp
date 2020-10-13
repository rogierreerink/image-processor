/* ---------------------------------------------------------- Local includes */

#include "border.hpp"
#include "median.hpp"

/* --------------------------------------------------------- System includes */

#include <vector>

/* -------------------------------------------------------------- Namespaces */

using namespace std;

/* -------------------------------------------------------------- Exceptions */
/* ------------------------------------------------------------- Private API */

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

	const int border = max((height - 1) / 2, (width - 1) / 2);
	
	/* Expand borders to solve border problem. */
	Mat temp = input.clone();
	for (int i = 0; i < border; i++) {
		temp = Border::Copy(temp);
	}

	/* Preallocate vector memory. */
	vector<uchar> area;
	vector<uchar> areaSorted;
	area.reserve(width * height);
	areaSorted.reserve(width * height);

	/* Apply filter. */
	for (int channel = 0; channel < output.channels(); channel++) {
		for (int row = 0; row < output.rows; row++) {
			for (int col = 0; col < output.cols; col++) {

				area.clear();
				const int tempRow = row + border;
				const int tempCol = col + border;
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
}

void Median::Square(const Mat &input, Mat &output, int side) {
	Median::Rectangle(input, output, side, side);
}
