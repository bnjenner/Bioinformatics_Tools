#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <cctype>


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
void process_fasta_entry(const std::string &entry, const std::string &method) {
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
void process_fastq_entry(const std::string &entry, const std::string &method) {
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