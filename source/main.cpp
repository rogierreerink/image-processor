/* ---------------------------------------------------------- Local Includes */

#include "command.hpp"
#include "convolution.hpp"
#include "demosaic.hpp"
#include "median.hpp"
#include "pixel.hpp"

/* --------------------------------------------------------- System Includes */

#include <iostream>
#include <fstream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

/* -------------------------------------------------------------- Namespaces */

using namespace std;
using namespace cv;

/* -------------------------------------------------------- Static functions */

static bool fileExists(const string &path) {
	ifstream f(path);
	return f.good();
}

/* ------------------------------------------------------------- Application */

int main(int argc, char **argv) {

	Option optionPathIn = {'i', "input-image",
		"Path to the image file to be processed.", true, true};
	Option optionPathOut = {'o', "output-image",
		"Path to write the processed image file to.", false, true};
	Option optionAdjustBrightness = {"adjust-brightness",
		"Adjust the brightness of the image.", false, true};
	Option optionAdjustContrast = {"adjust-contrast",
		"Adjust the contrast of the image.", false, true};
	Option optionFilterBayer = {"filter-bayer",
		"Apply a Bayer filter to the image.", false, false};
	Option optionFilterBoxBlur = {"filter-box-blur",
		"Apply a box blur filter of the given size to the image.", false, true};
	Option optionFilterMedian = {"filter-median",
		"Apply a median filter of the given size to the image.", false, true};
	Option optionViewInOut = {"view-images",
		"View the input and output images in a window.", false, false};
	Option optionViewIn = {"view-input",
		"View the input image in a window.", false, false};
	Option optionViewOut = {"view-output",
		"View the output image in a window.", false, false};
	Option optionViewHistogram = {"view-histogram",
		"View the the histogram of the displayed image(s) in a window.", false, false};
	Option optionEfficient = {'e', "efficient",
		"Prefer the use of OpenCV built-in image processing routines.", false, false};
	Option optionVerbose = {'v', "verbose",
		"Print process status.", false, false};
	Option optionHelp = {'h', "help",
		"Print this help menu.", false, false};

	Command cmd("Image Processor", "1.0");
	cmd.AddOption(optionPathIn);
	cmd.AddOption(optionPathOut);
	cmd.AddOption(optionAdjustBrightness);
	cmd.AddOption(optionAdjustContrast);
	cmd.AddOption(optionFilterBayer);
	cmd.AddOption(optionFilterBoxBlur);
	cmd.AddOption(optionFilterMedian);
	cmd.AddOption(optionViewInOut);
	cmd.AddOption(optionViewIn);
	cmd.AddOption(optionViewOut);
	cmd.AddOption(optionViewHistogram);
	cmd.AddOption(optionEfficient);
	cmd.AddOption(optionVerbose);
	cmd.SetHelpOption(optionHelp);
	
	try {
		cmd.ParseOptions(argc, argv);

	} catch (NoArguments &e) {
		cmd.PrintHelp(cout);
		return EXIT_FAILURE;

	} catch (InvalidArgument &e) {
		cout << e.what() << endl;
		return EXIT_FAILURE;
	}

	if (optionHelp.IsOptionSupplied()) {
		cmd.PrintOptions(cout);
		return EXIT_SUCCESS;
	}

	string inputPath;
	string outputPath;
	bool viewInput = false;
	bool viewOutput = false;
	bool viewHistogram = false;
	bool efficient = false;
	bool verbose = false;

	list<Option*> suppliedProcessingOptions;
	int adjustBrightnessShift = 0;
	float adjustContrastFactor = 1;
	int filterBoxBlurSize = 1;
	int filterMedianSize = 1;

	/* Process command line options. */
	for (auto &option: cmd.GetSuppliedOptions()) {

		if (option == &optionPathIn) {
			if (fileExists(optionPathIn.GetInput())) {
				inputPath = optionPathIn.GetInput();
			} else {
				cout << "Input image '" << optionPathIn.GetInput()
					<< "' doesn't exist." << endl;
				return EXIT_FAILURE;
			}

		} else if (option == &optionPathOut) {
			outputPath = optionPathOut.GetInput();

		} else if (option == &optionAdjustBrightness) {
			suppliedProcessingOptions.push_back(&optionAdjustBrightness);
			sscanf(optionAdjustBrightness.GetInput().c_str(),
				"%i", &adjustBrightnessShift);

		} else if (option == &optionAdjustContrast) {
			suppliedProcessingOptions.push_back(&optionAdjustContrast);
			sscanf(optionAdjustContrast.GetInput().c_str(),
				"%f", &adjustContrastFactor);
			if (adjustContrastFactor < 0.0) {
				cout << "The contrast factor should be a positive, real number."
					<< endl;
				return EXIT_FAILURE;
			}

		} else if (option == &optionFilterBayer) {
			suppliedProcessingOptions.push_back(&optionFilterBayer);
		
		} else if (option == &optionFilterBoxBlur) {
			suppliedProcessingOptions.push_back(&optionFilterBoxBlur);
			sscanf(optionFilterBoxBlur.GetInput().c_str(),
				"%i", &filterBoxBlurSize);
			if (filterBoxBlurSize < 1 || !(filterBoxBlurSize % 2)) {
				cout << "The box blur filter size should be a natural, odd number."
					<< endl;
				return EXIT_FAILURE;
			}

		} else if (option == &optionFilterMedian) {
			suppliedProcessingOptions.push_back(&optionFilterMedian);
			sscanf(optionFilterMedian.GetInput().c_str(),
				"%i", &filterMedianSize);
			if (filterMedianSize < 1 || !(filterMedianSize % 2)) {
				cout << "The median filter size should be a natural, odd number."
					<< endl;
				return EXIT_FAILURE;
			}

		} else if (option == &optionViewInOut) {
			viewInput = true;
			viewOutput = true;

		} else if (option == &optionViewIn) {
			viewInput = true;

		} else if (option == &optionViewOut) {
			viewOutput = true;

		} else if (option == &optionViewHistogram) {
			viewHistogram = true;

		} else if (option == &optionEfficient) {
			efficient = true;

		} else if (option == &optionVerbose) {
			verbose = true;
		}
	}

	/* Load the input image (copy to the output image). */
	Mat inputImage = imread(inputPath, IMREAD_COLOR);
	Mat outputImage = inputImage.clone();

	/* Process the output image in the order in which the options were supplied. */
	for (auto &option: suppliedProcessingOptions) {

		if (option == &optionAdjustBrightness) {
			
			if (verbose) {
				cout << "Adjusting brightness... ";
			}

			if (adjustBrightnessShift != 0) {
				/* My implementation. */
				Pixel::Brightness(outputImage, adjustBrightnessShift);
			}

			if (verbose) {
				cout << "Done." << endl;
			}

		} else if (option == &optionAdjustContrast) {

			if (verbose) {
				cout << "Adjusting contrast... ";
			}

			if (adjustContrastFactor != 1.0) {
				/* My implementation. */
				Pixel::Contrast(outputImage, adjustContrastFactor);
			}

			if (verbose) {
				cout << "Done." << endl;
			}

		} else if (option == &optionFilterBayer) {

			if (verbose) {
				cout << "Applying Bayer filter... ";
			}

			/* My implementation. */
			Demosaic::Bayer(outputImage, outputImage);

			if (verbose) {
				cout << "Done." << endl;
			}

		} else if (option == &optionFilterBoxBlur) {

			if (verbose) {
				cout << "Applying box blur filter... ";
			}

			if (filterBoxBlurSize == 1) {
				if (efficient) {
					/* OpenCV implementation. */
					blur(outputImage, outputImage, 
						Size_<int>(Point_<int>(filterBoxBlurSize, filterBoxBlurSize)));
				} else {
					/* My implementation. */
					Convolution::BoxBlur(outputImage, outputImage, filterBoxBlurSize);
				}
			}

			if (verbose) {
				cout << "Done." << endl;
			}

		} else if (option == &optionFilterMedian) {

			if (verbose) {
				cout << "Applying median filter... ";
			}

			if (filterMedianSize == 1) {
				if (efficient) {
					/* OpenCV implementation. */
					medianBlur(outputImage, outputImage, filterMedianSize);
				} else {
					/* My implementation. */
					Median::Square(outputImage, outputImage, filterMedianSize);
				}
			}

			if (verbose) {
				cout << "Done." << endl;
			}
		}
	}
	
	/* View image(s). */
	if (viewInput || viewOutput || viewHistogram) {

		const string inputImageWindow = "Input Image";
		const string inputHistogramWindow = "Input Histogram";
		const string outputImageWindow = "Output Image";
		const string outputHistogramWindow = "Output Histogram";

		if (viewInput) {
			imshow(inputImageWindow, inputImage);
			if (viewHistogram) {
				//
			}
		}

		if (viewOutput) {
			imshow(outputImageWindow, outputImage);
			if (viewHistogram) {
				//
			}
		}

		char key;
		bool exit = false;
		while (!exit && (key = waitKey(2000))) {
			
			switch (key) {

				/* Exit the loop when 'ESC' or 'Q' is pressed. */
				case 0x1B:
				case 'q':
					exit = true;
					break;
			}

			/* Exit the loop when all windows are closed. */
			if (!getWindowProperty(inputImageWindow, WND_PROP_VISIBLE) 
				&& !getWindowProperty(inputHistogramWindow, WND_PROP_VISIBLE) 
				&& !getWindowProperty(outputImageWindow, WND_PROP_VISIBLE) 
				&& !getWindowProperty(outputHistogramWindow, WND_PROP_VISIBLE)) {
					exit = true;
			}
		}

		destroyAllWindows();
	}

	return EXIT_SUCCESS;
}
