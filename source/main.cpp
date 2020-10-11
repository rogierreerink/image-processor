/* ---------------------------------------------------------- Local Includes */

#include "command.hpp"

/* --------------------------------------------------------- System Includes */

#include <iostream>
#include <exception>

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
	} catch (invalid_argument &e) {

		if (argc > 1) {
			cout << e.what() << endl;
			return EXIT_FAILURE;

		} else {
			cmd.PrintHelp(cout);
			return EXIT_FAILURE;
		}
	}

	if (optionHelp.IsOptionSupplied()) {
		cmd.PrintOptions(cout);
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}
