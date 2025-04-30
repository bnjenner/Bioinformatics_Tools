#include <iostream>
#include <fstream>
#include <utils.h>

int main(int argc, const char **argv) {

	int i = 0;
	std::string entry, line;
	
	// If streaming
	if (argc < 2) {

	    while (std::getline(std::cin, line)) {
	    	if (i % 4 == 0) {
        		std::cout << line.replace(0, 1, ">") << "\n";
        	} else if (i % 4 == 1) {
        		std::cout << line << "\n";
        	}
        	i += 1;
        }

	    // If file
	} else {

		std::string input = argv[1];
		if (input == "-h" || input == "--help") {  
			print_help(); 
			return 0;
		}

		std::ifstream fastq(input);
	    if (fastq.is_open()) {

		    while (std::getline(fastq, line)) {
				if (i % 4 == 0) {
	        		std::cout << line.replace(0, 1, ">") << "\n";
	        	} else if (i % 4 == 1) {
	        		std::cout << line << "\n";
	        	}
	        	i += 1;
			}

	        fastq.close(); // Close the file
	   
	    } else {
	    	std::cerr << "ERROR: Could not read fastq file: " << input << "\n";
			throw "ERROR: Make sure fasta filq exists.";
	    }
	}
	
    return 0;


}