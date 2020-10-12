#pragma once

/* --------------------------------------------------------- System Includes */

#include <ostream>
#include <list>
#include <string>
#include <exception>

/* -------------------------------------------------------------- Namespaces */

using namespace std;

/* -------------------------------------------------------------- Exceptions */

class NoArguments: public exception {
	public:
	NoArguments();
};

class InvalidArgument: public invalid_argument {
	public:
	InvalidArgument(const string &what);
};

/* ----------------------------------------------------------------- Classes */

class Option {
	public:
	Option(const string &longId, const string &description,
		bool optionRequired, bool inputRequired);
	Option(char shortId, const string &longId, const string &description,
		bool optionRequired, bool inputRequired);
	char GetShortId() const;
	const string &GetLongId() const;
	const string &GetDescription() const;
	const string &GetInput() const;
	void SetInput(const string &input);
	void SetOptionSupplied();
	bool IsOptionSupplied() const;
	bool IsOptionRequired() const;
	bool IsInputRequired() const;
	private:
	const char ShortId;
	const string LongId;
	const string Description;
	const bool OptionRequired;
	const bool InputRequired;
	string Input;
	bool Supplied = false;
};

class Command {
	public:
	Command(const string &identifier, const string &version);
	void AddOption(Option &option);
	void SetHelpOption(Option &option);
	void ParseOptions(int argc, char **argv);
	list<Option*> GetSuppliedOptions() const;
	void PrintOptions(ostream &os);
	void PrintHelp(ostream &os);
	private:
	const string Identifier;
	const string Version;
	Option *HelpOption = nullptr;
	list<Option*> Options;
};
