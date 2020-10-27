/* ---------------------------------------------------------- Local includes */

#include "pixel.hpp"

/* --------------------------------------------------------- System includes */

#include <cmath>

/* -------------------------------------------------------------- Namespaces */

using namespace std;

/* -------------------------------------------------------------- Exceptions */
/* ------------------------------------------------------------- Private API */
/* -------------------------------------------------------------- Public API */

void Pixel::Brightness(Mat &image, int shift) {

	for (int channel = 0; channel < image.channels(); channel++) {
		for (int row = 0; row < image.rows; row++) {
			for (int col = 0; col < image.cols; col++) {

				const int newPixel 
					= (float)image.at<Vec3b>(row, col)[channel] + shift;

				if (newPixel > 255) {
					image.at<Vec3b>(row, col)[channel] = 255;
				} else if (newPixel < 0) {
					image.at<Vec3b>(row, col)[channel] = 0;
				} else {
					image.at<Vec3b>(row, col)[channel] = (uchar)newPixel;
				}
			}
		}
	}
}

void Pixel::Contrast(Mat &image, float factor) {

	for (int channel = 0; channel < image.channels(); channel++) {
		for (int row = 0; row < image.rows; row++) {
			for (int col = 0; col < image.cols; col++) {

				const float newPixel 
					= ((float)image.at<Vec3b>(row, col)[channel] - 127) * factor + 127;

				if (newPixel > 255) {
					image.at<Vec3b>(row, col)[channel] = 255;
				} else if (newPixel < 0) {
					image.at<Vec3b>(row, col)[channel] = 0;
				} else {
					image.at<Vec3b>(row, col)[channel] = (uchar)newPixel;
				}
			}
		}
	}
}

void Pixel::Gamma(Mat &image, float power) {

	uint8_t gammaLUT[256];
	for (int i = 0; i < 256; i++) {
		gammaLUT[i] = pow(i / 255.0, power) * 255;
	}

	for (int channel = 0; channel < image.channels(); channel++) {
		for (int row = 0; row < image.rows; row++) {
			for (int col = 0; col < image.cols; col++) {
				image.at<Vec3b>(row, col)[channel] =
					gammaLUT[image.at<Vec3b>(row, col)[channel]];
			}
		}
	}
}
