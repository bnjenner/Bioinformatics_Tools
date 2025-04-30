#include <iostream>
#include <fstream>
#include <utils.h>


int main(int argc, const char **argv) {

	int i = 0;
	std::string entry, line;
	std::string method = "transcribe";
	
	// If streaming
	if (argc < 2) {

	    while (std::getline(std::cin, line)) {
	    	entry += line + '\n';
	    	if (i % 2 == 1) { 
	    		if (entry[0] == '>') {
	    			process_fasta_entry(entry, method);
	    		} else {
	    			process_fastq_entry(entry, method);
	    		}
	    		entry = "";
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

		std::ifstream file(input);
	    if (file.is_open()) {

		    while (std::getline(file, line)) {
		    	entry += line + '\n';
		    	if (i % 2 == 1) { 
		    		if (entry[0] == '>') {
		    			process_fasta_entry(entry, method);
		    		} else {
		    			process_fastq_entry(entry, method);
		    		}
		    		entry = "";
		    	}
		        i += 1;
		    }

	        file.close(); // Close the file
	   
	    } else {
	    	std::cerr << "ERROR: Could not read fasta/fastq file: " << input << "\n";
			throw "ERROR: Make sure fasta/fastq file exists.";
	    }
	}


	
    return 0;

}