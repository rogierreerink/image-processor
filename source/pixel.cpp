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

	uint8_t brightnessLUT[256];
	for (int i = 0; i < 256; i++) {
		int value = i + shift;
		if (value > 255) {
			brightnessLUT[i] = 255;
		} else if (value < 0) {
			brightnessLUT[i] = 0;
		} else {
			brightnessLUT[i] = value;
		}
	}

	for (int channel = 0; channel < image.channels(); channel++) {
		for (int row = 0; row < image.rows; row++) {
			for (int col = 0; col < image.cols; col++) {
				image.at<Vec3b>(row, col)[channel] =
					brightnessLUT[image.at<Vec3b>(row, col)[channel]];
			}
		}
	}
}

void Pixel::Contrast(Mat &image, float factor) {

	uint8_t contrastLUT[256];
	for (int i = 0; i < 256; i++) {
		float value = (i - 127.0) * factor + 127.0;
		if (value > 255) {
			contrastLUT[i] = 255;
		} else if (value < 0) {
			contrastLUT[i] = 0;
		} else {
			contrastLUT[i] = value;
		}
	}

	for (int channel = 0; channel < image.channels(); channel++) {
		for (int row = 0; row < image.rows; row++) {
			for (int col = 0; col < image.cols; col++) {
				image.at<Vec3b>(row, col)[channel] =
					contrastLUT[image.at<Vec3b>(row, col)[channel]];
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
