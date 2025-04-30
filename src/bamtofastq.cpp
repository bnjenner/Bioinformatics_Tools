#include <iostream>
#include "api/BamAux.h"
#include "api/BamReader.h"

int main(int argc, const char **argv) {

	std::string input = argv[1];
	if (input == "-h" || input == "--help") {
		std::cerr << "It does what it says it does..." << std::endl;
		return 0;
	}

	// Open alignment file
	BamTools::BamReader inFile;
	BamTools::BamAlignment alignment;

	if (!inFile.Open(input)) {
		std::cerr << "ERROR: Could not read alignment file: " << input << "\n";
		throw "ERROR: Make sure alignment file exists.";
	}

	while (inFile.GetNextAlignment(alignment)) {
		std::cout << "@" << alignment.Name << "\n"
				  << alignment.QueryBases << "\n+\n"
				  << alignment.Qualities << "\n";
	}

	inFile.Close();
}