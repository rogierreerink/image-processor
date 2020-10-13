/* ---------------------------------------------------------- Local includes */

#include "pixel.hpp"

/* --------------------------------------------------------- System includes */
/* -------------------------------------------------------------- Namespaces */
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
