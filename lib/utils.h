#pragma once

#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>

void print_help();

// Reverse Complement Sequence
//		And Complement is gonna be different functions... deal with it.
std::string revcomp(const std::string &seq);

// Complement Sequence
std::string comp(const std::string &seq);

// Transcribe DNA into RNA
std::string transcribe(const std::string &seq);

// Translate mRNA into proteins
std::string translate(const std::string &seq, const int orf);

std::string modify_seq(const std::string &seq, const std::string &method);

// Process FASTA entries
void modify_fasta_entry(const std::string &entry, const std::string &method);

// Process FASTQ entries
void modify_fastq_entry(const std::string &entry, const std::string &method);

// Process FASTA/FASTQ file into unordered map
std::unordered_map<std::string, std::string> read_fastx(const std::string &file, const std::string &filetype);
