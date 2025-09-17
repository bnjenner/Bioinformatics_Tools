#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "AlignmentClass.h"
#include "utils.h"

// Perform Traceback
void Alignment::traceback() {
	int i = n;
	int j = m;
	while (i > 0 || j > 0) {
		if (tb[i][j] == 'D') {
			aligned_seq1 = seq1[i - 1] + aligned_seq1;
			aligned_seq2 = seq2[j - 1] + aligned_seq2;
			middle_line = "|"+ middle_line;
			i -= 1; j -= 1;
		} else if (tb[i][j] == 'U') {
			aligned_seq1 = seq1[i - 1] + aligned_seq1;
			aligned_seq2 = "-" + aligned_seq2;
			middle_line = " " + middle_line;
			i -= 1;
		} else if (tb[i][j] == 'L') {
			aligned_seq1 = "-" + aligned_seq1;
			aligned_seq2 = seq2[j - 1] + aligned_seq2;
			middle_line = " " + middle_line;
			j -= 1;
		}
	}
}

// Generic Dynamic Programming Implementation
int Alignment::align_aux(const int &match, const int &mismatch, const int &gap) {

	// Temporary variables
	int tmp, t;
	std::vector<int> tmp_vec;

	for (int i = 1; i <= seq1.size(); i++) {	
		for (int j = 1; j <= seq2.size(); j++) {

			if (seq1[i - 1] == seq2[j - 1]) {
				tmp = match;
			} else { 
				tmp = mismatch;
			}
		
			tmp_vec = {dp[i-1][j-1] + tmp,      // Match/Mismatch
				       dp[i-1][j] + gap,        // Gap in seq2
				       dp[i][j-1] + gap};       // Gap in seq1


			auto max_it = std::max_element(tmp_vec.begin(), tmp_vec.end());
			dp[i][j] = *max_it;

			// Store Traceback (Greedy)
			t = std::distance(tmp_vec.begin(), max_it);
			if (t == 0) {
				tb[i][j] = 'D';
			} else if (t == 1) {
				tb[i][j] = 'U';
			} else {
				tb[i][j] = 'L';
			}
		
		}
	}

	return dp[seq1.size()][seq2.size()];
}

// General Alignment Call
int Alignment::align(const std::string &method) {
		
	int match;
	int mismatch;
	int gap;

	// Initialize Directions of Traceback
	for (int i = 0; i <= n; i++) { dp[i][0] = gap * i; tb[i][0] = 'U'; }
	for (int j = 0; j <= m; j++) { dp[0][j] = gap * j; tb[0][j] = 'L'; }

	// Initialize Scoring Penalities and Boundary Conditions
	if (method == "nw") {
		match = 1;
		mismatch = -1;
		gap = -2;

		for (int i = 0; i <= n; i++) { dp[i][0] = gap * i; }
		for (int j = 0; j <= m; j++) { dp[0][j] = gap * j; }

	} else if (method == "semi") {
		match = 1;
		mismatch = -1;
		gap = -2;

		if (seq1.size() > seq2.size()) {
			for (int j = 0; j <= seq2.size(); j++) { dp[0][j] = gap * j; }
		} else {
			for (int i = 0; i <= seq1.size(); i++) { dp[i][0] = gap * i; } 
		}

	} else {
		std::cerr << "Error: Unknown alignment method " << method << "\n";
		exit(1);
	}

	// Perform Alignment
	score = align_aux(match, mismatch, gap);
	traceback();

	return score;
}

// Print Alignment
void Alignment::print_alignment() const {
	std::cout << "Score: " << score << "\n";
	if (!seq1_id.empty()) {
		std::cout << seq1_id << "\n";
	}
	std::cout << aligned_seq1 << "\n";
	std::cout << middle_line << "\n";
	std::cout << aligned_seq2 << "\n";
	if (!seq2_id.empty()) {
		std::cout << seq2_id << "\n";
	}
}