#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "utils.h"


// Alignment Class
class Alignment {;

private:

	std::string seq1, seq2;
	std::string seq1_id, seq2_id;

	int n, m;
	int score = 0;
	std::string middle_line = "";
	std::string aligned_seq1 = "";
	std::string aligned_seq2 = "";

	std::vector<std::vector<int>> dp;
	std::vector<std::vector<char>> tb;

	void traceback();
	int align_aux(const int &match, const int &mismatch, const int &gap);
	
public:

	Alignment() {}

	Alignment(const std::string &s1, const std::string &s2, const std::string &s1_id="", const std::string &s2_id="") 
	: seq1(s1), seq2(s2), seq1_id(s1_id), seq2_id(s2_id) {
		n = seq1.size();
		m = seq2.size();
		dp = std::vector<std::vector<int>>(n + 1, std::vector<int>(m + 1, 0));
		tb = std::vector<std::vector<char>>(n + 1, std::vector<char>(m + 1, ' '));
	}

	void reset() {
		score = 0;
		middle_line = "";
		aligned_seq1 = "";
		aligned_seq2 = "";
		dp = std::vector<std::vector<int>>(n + 1, std::vector<int>(m + 1, 0));
		tb = std::vector<std::vector<char>>(n + 1, std::vector<char>(m + 1, ' '));
	}

	int get_score() const { return score; }
	std::string get_aligned_seq1() const { return aligned_seq1; }
	std::string get_aligned_seq2() const { return aligned_seq2; }

	int align(const std::string &method);
	void print_alignment() const;
};