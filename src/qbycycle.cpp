#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <utils.h>


void sum_scores(const std::string &line, std::vector<int> &sum, std::vector<int> &pos) {
	
	int offset = 33;
	int n = sum.size();
	int length = line.size();

	if (length > n) {
		sum.resize(line.size(), 0);
		pos.resize(line.size(), 0);
	}

	for (int i = 0; i < length; i++) {
		sum[i] += static_cast<unsigned int>(line[i]) - offset;
		pos[i] += 1;
	}
}

int main(int argc, const char **argv) {

	int i = 0;
	
	std::string entry, line;
	std::vector<int> phred_sum; // Sum of phred scores at position
	std::vector<int> pos_reads; // Reads at positions
	
	// If streaming
	if (argc < 2) {

	    while (std::getline(std::cin, line)) {
	    	if (i % 4 == 3) {
	    		sum_scores(line, phred_sum, pos_reads);
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
	    			sum_scores(line, phred_sum, pos_reads);
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
	float avg;
	std::cout << "Pos\tScore\n";
    for (int i = 0; i < phred_sum.size(); i++) {
    	avg = (float)phred_sum[i] / (float)pos_reads[i];
		std::cout << i << "\t" 
				  << std::fixed 
				  << std::setprecision(2) 
				  << avg << "\n";
    }
	
    return 0;


}