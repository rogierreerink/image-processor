/* ---------------------------------------------------------- Local Includes */

#include "command.hpp"

/* --------------------------------------------------------- System Includes */

#include <iostream>

/* -------------------------------------------------------------- Namespaces */

using namespace std;

/* ------------------------------------------------------------- Application */

int main(int argc, char **argv) {

	Option optionHelp = {'h', "help",
		"Print this help menu.", false, false};
	Option optionPathIn = {'i', "input-image",
		"Path to the image file to be processed.", true, true};
	Option optionPathOut = {'o', "output-image",
		"Path to write the processed image file to.", false, true};
	Option optionViewIn = {'v', "view-images",
		"View the the image(s) in a window.", false, false};
	
	Command cmd("Image Processor", "1.0");
	cmd.AddOption(optionPathIn);
	cmd.AddOption(optionPathOut);
	cmd.AddOption(optionViewIn);
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

	auto suppliedOptions = cmd.GetSuppliedOptions();
	for (auto &suppliedOption: suppliedOptions) {
		auto shortId = suppliedOption->GetShortId();

		if (shortId == optionPathIn.GetShortId()) {
			cout << "Input image: " << suppliedOption->GetInput() << endl;

		} else if (shortId == optionPathOut.GetShortId()) {
			cout << "Output image: " << suppliedOption->GetInput() << endl;

		} else if (shortId == optionViewIn.GetShortId()) {
			cout << "View mode enabled." << endl;
		}
	}

	return EXIT_SUCCESS;
}
