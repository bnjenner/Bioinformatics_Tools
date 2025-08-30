#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utils.h>

void process_gtf_line(std::string &line, std::vector<std::string> &columns) {
	int i = 0;
	std::string col;
	std::istringstream column_stream(line);
	while (std::getline(column_stream, col, '\t')) {
		columns.at(i) = col;
		i += 1;
	}
}

void get_region(std::string &region, std::string &chrom, int &start, int &stop) {
	int colon = region.find(':');
	int dash = region.find('-');
	chrom = region.substr(0, colon);
	start = std::stoi(region.substr(colon + 1, dash - colon - 1));
	stop = std::stoi(region.substr(dash + 1));
}

int main(int argc, const char **argv) {

	int start, stop;
	std::string chrom, region;
	std::string input, line;
	std::vector<std::string> columns{9, ""};

    // If streaming
	if (argc < 3) {

		input = argv[1];
		if (input == "-h" || input == "--help") {
			std::cerr << "USAGE:\n"
					  << "    gtfget \"chr:start-stop\" gtf|stdin\n\n"
					  << "Retrieves entries of GTf file by coordinatines (chrom:start-stop)\n" << std::endl;
			return 0;
		}

		get_region(input, chrom, start, stop);
		while (std::getline(std::cin, line)) {
			
			if (line[0] == '#') {
    			std::cout << line << "\n";
    			continue;
    		}
	    	
	    	process_gtf_line(line, columns);
	    	if (columns[0] == chrom) {
    			if (std::stoi(columns[3]) > stop) { break; } // if past region stop
    			if (std::stoi(columns[4]) >= start && std::stoi(columns[3]) <= stop) {
    				std::cout << line << "\n";
    			}
    		} 
        }

	} else {

		// Open file
		region = argv[1];
		input = argv[2];
		std::ifstream gtf(input);
	    if (gtf.is_open()) {

	    	get_region(region, chrom, start, stop);
	    	while (std::getline(gtf, line)) {
	    		
	    		if (line[0] == '#') {
	    			std::cout << line << "\n";
	    			continue;
	    		}
	    		
	    		process_gtf_line(line, columns);
	    		if (columns[0] == chrom) {
	    			if (std::stoi(columns[3]) > stop) { break; } // if past region stop
	    			if (std::stoi(columns[4]) >= start && std::stoi(columns[3]) <= stop) {
	    				std::cout << line << "\n";
	    			}
	    		} 
			}

	    } else {
	    	std::cerr << "ERROR: Could not read GTF file: " << input << "\n";
			throw "ERROR: Make sure file exists.";
	    }

		gtf.close(); // Close the fil

	}

}