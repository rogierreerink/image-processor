/* ---------------------------------------------------------- Local Includes */

#include "command.hpp"
#include "convolution.hpp"
#include "median.hpp"

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

	Option optionHelp = {'h', "help",
		"Print this help menu.", false, false};
	Option optionPathIn = {'i', "input-image",
		"Path to the image file to be processed.", true, true};
	Option optionPathOut = {'o', "output-image",
		"Path to write the processed image file to.", false, true};
	Option optionFilterMedian = {'m', "filter-median",
		"Apply a median filter of the given size to the image.", false, true};
	Option optionViewInOut = {'v', "view-images",
		"View the input and output images in a window.", false, false};
	Option optionViewIn = {"view-input",
		"View the input image in a window.", false, false};
	Option optionViewOut = {"view-output",
		"View the output image in a window.", false, false};
	Option optionViewHistogram = {"view-histogram",
		"View the the histogram of the displayed image(s) in a window.", false, false};

	Command cmd("Image Processor", "1.0");
	cmd.AddOption(optionPathIn);
	cmd.AddOption(optionPathOut);
	cmd.AddOption(optionFilterMedian);
	cmd.AddOption(optionViewInOut);
	cmd.AddOption(optionViewIn);
	cmd.AddOption(optionViewOut);
	cmd.AddOption(optionViewHistogram);
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
	int filterMedianSize;
	bool filterMedian = false;
	bool viewInput = false;
	bool viewOutput = false;
	bool viewHistogram = false;

	for (auto &suppliedOption: cmd.GetSuppliedOptions()) {

		if (suppliedOption == &optionPathIn) {
			if (fileExists(optionPathIn.GetInput())) {
				inputPath = optionPathIn.GetInput();
			} else {
				cout << "Input image '" << optionPathIn.GetInput()
					<< "' doesn't exist." << endl;
				return EXIT_FAILURE;
			}

		} else if (suppliedOption == &optionPathOut) {
			outputPath = optionPathOut.GetInput();

		} else if (suppliedOption == &optionFilterMedian) {
			filterMedian = true;
			sscanf(optionFilterMedian.GetInput().c_str(),
				"%i", &filterMedianSize);
			if (filterMedianSize < 1 || !(filterMedianSize % 2)) {
				cout << "The median filter size should be a natural, odd number."
					<< endl;
				return EXIT_FAILURE;
			}

		} else if (suppliedOption == &optionViewInOut) {
			viewInput = true;
			viewOutput = true;

		} else if (suppliedOption == &optionViewIn) {
			viewInput = true;

		} else if (suppliedOption == &optionViewOut) {
			viewOutput = true;

		} else if (suppliedOption == &optionViewHistogram) {
			viewHistogram = true;
		}
	}

	Mat inputImage = imread(inputPath, IMREAD_COLOR);
	Mat outputImage(inputImage.rows, inputImage.cols, inputImage.type());

	if (filterMedian) {
		Median::Square(inputImage, outputImage, filterMedianSize);
	}
	
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

				/* Do something */
				case 'x':
					break;

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
