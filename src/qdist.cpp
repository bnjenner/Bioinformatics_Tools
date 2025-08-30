#include <iostream>
#include <fstream>
#include <vector>
#include <utils.h>

int main(int argc, const char **argv) {

	int i = 0;
	int offset = 33;
	std::string entry, line;

	std::vector<int> phred_counts(61, 0); // Phred scores from 0 to 60
	
	// If streaming
	if (argc < 2) {

	    while (std::getline(std::cin, line)) {
	    	if (i % 4 == 3) {
	    		for (const char &q : line) {
	    			phred_counts[static_cast<unsigned int>(q) - offset] += 1;
	    		}
        	}
        	i += 1;
        }

	    // If file
	} else {

		std::string input = argv[1];
		if (input == "-h" || input == "--help") {
			std::cerr << "Generates histogram of phred scores (offset 33)." << std::endl;
			return 0;
		}

		std::ifstream fastq(input);
	    if (fastq.is_open()) {

		    while (std::getline(fastq, line)) {
				if (i % 4 == 3) {
	    		for (const char &q : line) {
	    			phred_counts[static_cast<unsigned int>(q) - offset] += 1;
	    		}
        	}
	        	i += 1;
			}

	        fastq.close(); // Close the file
	   
	    } else {
	    	std::cerr << "ERROR: Could not read fastq file: " << input << "\n";
			throw "ERROR: Make sure fasta filq exists.";
	    }
	}

	// Output histogram
	std::cerr << "Score\tFreq\n";
    for (int i = 1; i < phred_counts.size(); i++) {
		std::cout << i << "\t" << phred_counts[i] << "\n";
    }
	
    return 0;


}