#include <iostream>
#include <fstream>
#include <utils.h>


int main(int argc, const char **argv) {

	int i = 0;
	std::string curr, line;

	// If streaming
	if (argc < 2) {

	    while (std::getline(std::cin, line)) {
    		if (line[0] == '>') {
    			if (curr != "") { std::cout << "\n"; }
    			std::cout << line << "\n";
    			curr = line;
    		} else {
    			std::cout << line;
    		}
	    }
	    std::cout << "\n";   

	    // If file
	} else {

		std::string input = argv[1];
		if (input == "-h" || input == "--help") {
			std::cerr << "Flattens those pesky multiline fasta files.\n\n"
					  << " Look, I get that they're more human readable, but under what circumstances \n" 
					  << " do you really need to be looking for a specific sequence with your \n"
					  << " eyes within a file or trying to find it by its position. Please, just \n"
					  << " do it with some quick python code or something." << std::endl;
			return 0;
		}

		std::ifstream file(input);
	    if (file.is_open()) {

		    while (std::getline(file, line)) {
		    	if (line[0] == '>') {
	    			if (curr != "") { std::cout << "\n"; }
	    			std::cout << line << "\n";
	    			curr = line;
	    		} else {
	    			std::cout << line;
	    		}
		    }
		    std::cout << "\n"; 

	        file.close(); // Close the file
	   
	    } else {
	    	std::cerr << "ERROR: Could not read fasta/fastq file: " << input << "\n";
			throw "ERROR: Make sure fasta/fastq file exists.";
	    }
	}


	
    return 0;

}