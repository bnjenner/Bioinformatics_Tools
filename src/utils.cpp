#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include "utils.h"

const std::unordered_map<char, char> COMP_MAP = {
										         {'A', 'T'}, {'a', 't'},
										         {'T', 'A'}, {'t', 'a'},
										         {'G', 'C'}, {'g', 'c'},
										         {'C', 'G'}, {'c', 'g'},
										         {'U', 'A'}, {'u', 'a'},
											    };

const std::unordered_map<char, char> TRANS_MAP = {
										         {'A', 'A'}, {'a', 'a'},
										         {'T', 'U'}, {'t', 'u'},
										         {'G', 'G'}, {'g', 'g'},
										         {'C', 'C'}, {'c', 'c'},
										         {'U', 'U'}, {'u', 'u'}, // you never know
											    };


const std::unordered_map<std::string, char> CODON_MAP = {
														 {"UUU", 'F'}, {"UUC", 'F'},
														 {"UUA", 'L'}, {"UUG", 'L'},
														 {"CUU", 'L'}, {"CUC", 'L'}, {"CUA", 'L'}, {"CUG", 'L'},
														 {"AUU", 'I'}, {"AUC", 'I'}, {"AUA", 'I'},
														 {"AUG", 'M'},  // Start codon
														 {"GUU", 'V'}, {"GUC", 'V'}, {"GUA", 'V'}, {"GUG", 'V'},
														   
														 {"UCU", 'S'}, {"UCC", 'S'}, {"UCA", 'S'}, {"UCG", 'S'},
														 {"AGU", 'S'}, {"AGC", 'S'},
														 {"CCU", 'P'}, {"CCC", 'P'}, {"CCA", 'P'}, {"CCG", 'P'},
														 {"ACU", 'T'}, {"ACC", 'T'}, {"ACA", 'T'}, {"ACG", 'T'},
														 {"GCU", 'A'}, {"GCC", 'A'}, {"GCA", 'A'}, {"GCG", 'A'},
														   
														 {"UAU", 'Y'}, {"UAC", 'Y'},
														 {"UAA", '*'}, {"UAG", '*'}, {"UGA", '*'},  // Stop codons
														 {"CAU", 'H'}, {"CAC", 'H'},
														 {"CAA", 'Q'}, {"CAG", 'Q'},
														 {"AAU", 'N'}, {"AAC", 'N'},
														 {"AAA", 'K'}, {"AAG", 'K'},
														 {"GAU", 'D'}, {"GAC", 'D'},
														 {"GAA", 'E'}, {"GAG", 'E'},
														   
														 {"UGU", 'C'}, {"UGC", 'C'},
														 {"UGG", 'W'},
														 {"CGU", 'R'}, {"CGC", 'R'}, {"CGA", 'R'}, {"CGG", 'R'},
														 {"AGA", 'R'}, {"AGG", 'R'},
														 {"GGU", 'G'}, {"GGC", 'G'}, {"GGA", 'G'}, {"GGG", 'G'}
														};


void print_help() { std::cerr << "It does what it says it does..." << std::endl; }

// Print Dynamic Programming Matrix
template <typename T>
void print_dp_matrix(const std::vector<std::vector<T>> &dp, const std::string &seq1, const std::string &seq2) {
	std::cerr << "X\tX\t";
	for (int x = 0; x < seq2.size(); x++) { std::cerr << seq2[x] << "\t"; }
	std::cerr << "\n";
	for (int i = 0; i <= seq1.size(); i++) {
		if (i == 0) {
			std::cerr << "X\t";
		} else {
			std::cerr << seq1[i - 1] << "\t";
		}
		for (int j = 0; j <= seq2.size(); j++) {
			std::cerr << dp[i][j] << "\t";
		}
		std::cerr << "\n";
	}
}
template void print_dp_matrix<char>(const std::vector<std::vector<char>> &dp, const std::string &seq1, const std::string &seq2);
template void print_dp_matrix<int>(const std::vector<std::vector<int>> &dp, const std::string &seq1, const std::string &seq2);


// Reverse Complement Sequence
//		And Complement is gonna be different functions... deal with it.
std::string revcomp(const std::string &seq) {
	std::string rc_seq = "";
	for (int i = seq.length() - 1; i > -1; i--) { rc_seq += COMP_MAP.at(seq[i]); }
	return rc_seq; 
}

// Complement Sequence
std::string comp(const std::string &seq) {
	std::string c_seq = "";
	for (int i = 0; i < seq.length(); i++) { c_seq += COMP_MAP.at(seq[i]); }
	return c_seq; 
}

// Transcribe DNA into RNA
std::string transcribe(const std::string &seq) {
	std::string t_seq = "";
	for (int i = 0; i < seq.length(); i++) { t_seq += TRANS_MAP.at(seq[i]); }
	return t_seq; 
}

// Translate mRNA into proteins
std::string translate(const std::string &seq, const int &orf = 0) {
	
	char peptide;
	std::string p_seq, codon;
	for (int i = 0; i < (seq.length() - 2); i += 3) { 
		
		// This is so cursed 
		codon = std::string(1, toupper(seq[i])) + std::string(1, toupper(seq[i + 2])) + std::string(1, toupper(seq[i + 2]));
		peptide = CODON_MAP.at(codon);
		
		if (peptide == '*') { break; }
		
		p_seq += CODON_MAP.at(codon);
	}
	return p_seq; 
}

std::string modify_seq(const std::string &seq, const std::string &method) {
	if (method == "complement") {
		return comp(seq);
	} else if (method == "revcomplement") {
		return revcomp(seq);
	} else if (method == "transcribe") {
		return transcribe(seq);
	} else if (method == "translate") {
		return translate(seq);
	} else {
		std::cerr << "ERROR: INVALID METHOD\n";
		throw "ERROR: INVALID METHOD";
	}
}

// Process FASTA entries
void modify_fasta_entry(const std::string &entry, const std::string &method) {
	std::string line, seq;
	std::istringstream stream(entry);
	while (std::getline(stream, line, '\n')) {
		if (line[0] == '>') {
    		std::cout << line << "\n";
    		seq = "";
    	} else {
    		seq += line;
    	}
    }
    std::cout << modify_seq(seq, method) << std::endl;
}

// Process FASTQ entries
void modify_fastq_entry(const std::string &entry, const std::string &method) {
	int i = 0;
	std::string line, seq;
	std::istringstream stream(entry);
	while (std::getline(stream, line, '\n')) {
    	if (i % 4 == 0) {
    		std::cout << line << "\n";
    	} else if (i % 4 == 1) {
    		std::cout << modify_seq(seq, method) << "\n";
    	} else if (i % 4 == 2) {
    		std::cout << "+" << "\n";
    	} else {
    		std::cout << line << "\n";
    	}
    	i += 1;
    }
}

// Process FASTA/FASTQ file into unordered map
std::unordered_map<std::string, std::string> read_fastx(const std::string &file, const std::string &filetype) {

	std::string seq, id, line;
	std::unordered_map<std::string, std::string> fasta_map;

	std::ifstream fa(file);
    if (fa.is_open()) {

    	if (filetype == "fastq") {
    		int i = 0;
    		while (std::getline(fa, line, '\n')) {
		    	if (i % 4 == 0) {
		    		id = line;
		    	} else if (i % 4 == 1) {
		    		fasta_map[id] = line;
		    	}
		    	i += 1;
		    }
		
		} else if (filetype == "fasta") {

			while (std::getline(fa, line)) {
		    	if (line[0] == '>') {
		    		if (id != "") {
		    			fasta_map[id] = seq;
		    			seq = "";
		    		}
	    			id = line;
	    		} else {
	    			seq += line;
	    		}
		    }
		    fasta_map[id] = seq;
	        fa.close(); // Close the file

		} else {
			std::cerr << "ERROR: Invalid filetype. Need fasta/fastq\n";
			throw "ERROR: Invalid filetype. Need fasta/fastq";
		}

    } else {
    	std::cerr << "ERROR: Could not read fasta file: " << file << "\n";
		throw "ERROR: Make sure fasta file exists.";
    }

    return fasta_map;
}
