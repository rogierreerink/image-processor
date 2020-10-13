/* ---------------------------------------------------------- Local includes */

#include "command.hpp"

/* --------------------------------------------------------- System includes */

#include <iostream>
#include <exception>

/* -------------------------------------------------------------- Namespaces */
/* -------------------------------------------------------------- Exceptions */

NoArguments::NoArguments(): exception() {
}

InvalidArgument::InvalidArgument(const string &what): invalid_argument(what) {
}

/* ------------------------------------------------------------- Private API */
/* -------------------------------------------------------------- Public API */

Option::Option(const string &longId, const string &description,
		bool optionRequired, bool inputRequired): ShortId(0),
		LongId(longId),	Description(description), 
		OptionRequired(optionRequired),	InputRequired(inputRequired) {
}

Option::Option(char shortId, const string &longId, const string &description,
		bool optionRequired, bool inputRequired): ShortId(shortId),
		LongId(longId),	Description(description), 
		OptionRequired(optionRequired),	InputRequired(inputRequired) {
}

char Option::GetShortId() const {
	return ShortId;
}

const string &Option::GetLongId() const {
	return LongId;
}

const string &Option::GetDescription() const {
	return Description;
}

const string &Option::GetInput() const {
	return Input;
}

void Option::SetInput(const string &input) {
	Input = input;
}

void Option::SetOptionSupplied() {
	Supplied = true;
}

bool Option::IsOptionSupplied() const {
	return Supplied;
}

bool Option::IsOptionRequired() const {
	return OptionRequired;
}

bool Option::IsInputRequired() const {
	return InputRequired;
}

Command::Command(const string &identifier, const string &version):
	Identifier(identifier), Version(version) {
}

void Command::AddOption(Option &option) {
	Options.push_back(&option);
}

void Command::SetHelpOption(Option &option) {
	
	/* HelpOption is also pushed to the general option list,
	 * in order to simplify the argument parsing. */
	Options.push_back(&option);
	HelpOption = &option;
}

void Command::ParseOptions(int argc, char **argv) {

	char character;
	bool validArgument;
	unsigned nextArgParsed = 0;

	/* Check for insufficient number of arguments. */
	if (argc == 1) {
		throw NoArguments();
	}

	/* Parse the arguments. */
	for (unsigned i = 1; i < argc; i++) {

		if (nextArgParsed > 0) {
			nextArgParsed--;
			continue;
		}

		unsigned j = 0;
		unsigned dashCounter = 0;
		unsigned skipArgument = false;

		/* Interpret each argument character. */
		while (!skipArgument && (character = argv[i][j]) != '\0') {

			validArgument = false;
			
			/* Check for short-hand or long notation. */
			if (character == '-' && j < 2) {
				dashCounter++;

			} else {

				/* Parse short-hand argument. */
				if (dashCounter == 1) {
					for (auto &option: Options) {

						if (character == option->GetShortId()) {
							SuppliedOptions.push_back(option);
							validArgument = true;

							if (option->IsInputRequired()) {

								unsigned nextArgIndex = i + 1 + nextArgParsed;
								if (nextArgIndex < argc) {
									option->SetInput(argv[nextArgIndex]);
									nextArgParsed++;

								} else {
									throw InvalidArgument(
										string("Argument for option '-")
										+ character + "' is missing.");
								}
							}
							
							option->SetOptionSupplied();
							break;
						}
					}

					if (!validArgument) {
						throw InvalidArgument(string("Argument '-")
						+ character + "' not recognized.");
					}

				/* Parse long argument. */
				} else if (dashCounter == 2) {
					string argument = string(argv[i]);
					argument.erase(0, 2);

					for (auto &option: Options) {
						
						if (!argument.compare(option->GetLongId())) {
							SuppliedOptions.push_back(option);
							validArgument = true;

							if (option->IsInputRequired()) {

								unsigned nextArgIndex = i + 1;
								if (nextArgIndex < argc) {
									option->SetInput(argv[nextArgIndex]);
									nextArgParsed++;

								} else {
									throw InvalidArgument(
										string("Argument for option '--")
										+ argument + "' is missing.");
								}
							}

							option->SetOptionSupplied();
							break;
						}
					}

					if (!validArgument) {
						throw InvalidArgument(string("Argument '--")
						+ argument + "' not recognized.");
					}

					/* Prevent the next character of this argument
					 * to be interpreted. */
					skipArgument = true;

				} else {
					throw InvalidArgument(
						string("Invalid argument '") + 
						string(argv[i]) + "'.");
				}
			}

			j++;
		}
	}

	/* Check whether all required options have been supplied. If the help
	 * option was given, the user probibly doesn't know about any required
	 * options. */
	if (HelpOption != nullptr && !HelpOption->IsOptionSupplied()) {
		
		for (const auto &option: Options) {
			if (option->IsOptionRequired() && !option->IsOptionSupplied()) {
				throw InvalidArgument(string("Option '--")
				+ option->GetLongId() + "' is required.");
			}
		}
	}
}

list<Option*> Command::GetSuppliedOptions() const {
	return SuppliedOptions;
}

void Command::PrintOptions(ostream& os) {

	os << Identifier << ", version: " << Version << "." << endl
		<< "Usage information:" << endl;

	unsigned LongestId = 0;
	for (auto &option: Options) {
		if (option->GetLongId().length() > LongestId) {
			LongestId = option->GetLongId().length();
		}
	}

	for (auto &option: Options) {

		if (option->GetShortId() == 0) {
			os << "        --" << option->GetLongId();
		} else {
			os << "    -" << option->GetShortId()
				<< ", --" << option->GetLongId();
		}
		
		unsigned numSpaces = LongestId - option->GetLongId().length();
		for (unsigned i = 0; i < numSpaces; i++) {
			os << " ";
		}

		if (option->IsOptionRequired()) {
			os << " * ";
		} else {
			os << "   ";
		}

		os << option->GetDescription() << endl;
	}
}

void Command::PrintHelp(ostream &os) {

	os << Identifier << ", version: " << Version << "." << endl;

	if (HelpOption != nullptr) {
		os << "Append '-" << HelpOption->GetShortId() 
			<< "' or '--" << HelpOption->GetLongId() 
			<< "' for usage information." << endl;
	}
}
