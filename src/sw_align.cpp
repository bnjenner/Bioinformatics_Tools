#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iterator>
#include "AlignmentClass.h"
#include "utils.h"


int main(int argc, const char **argv) {

	std::string input = argv[1];
	if (input == "-h" || input == "--help" || argc < 3) {
		std::cerr << "Program:\n\tsw_align\n\n"
				  << "Description:\n\tImplementation of the Smith Waterman local alignment algorithm.\n\n"
				  << "Usage:\n\tsw_align seq1.fasta seq2.fasta\n" << std::endl;
		return 0;
	}

	// Store Sequences
	std::unordered_map<std::string, std::string> map1 = read_fastx(argv[1], "fasta");
	std::unordered_map<std::string, std::string> map2 = read_fastx(argv[2], "fasta");

    if (!map1.empty() && !map2.empty()) {
        
        // Get an iterator to the "first" element
        auto it1 = map1.begin();
        auto it2 = map2.begin();


        // Access the key using the iterator's first member
        std::string seq1 = it1->second;
        std::string seq2 = it2->second;

	    // Create Alignment Object./
	    Alignment aln(seq1, seq2);

	    // Align and Print Alignments
	    int score = aln.align("sw", "itr"); 
	    aln.print_alignment();
    } 
	
    return 0;

}