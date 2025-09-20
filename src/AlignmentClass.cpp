#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "AlignmentClass.h"
#include "utils.h"


////////////////////////////////////////////////////////////////////
// Perform Traceback
void Alignment::traceback(const int &i, const int &j, const int x, std::vector<std::string> &alns) {

	// Initilize Alignments Vector
	if (alns.empty()) {
		alns.push_back("");
	}

	// End of Alignment Cases
	if (i <= 0 && j <= 0) {
		return;
	
	} else if (i == 0 && j > 0) {
		alns[x] += "D";
		traceback(i, j - 1, x, alns);
		return;
	
	} else if (i > 0 && j == 0) {
		alns[x] += "I";
		traceback(i - 1, j, x, alns);
		return;
	} 


	int t;
	std::string t_str = alns[x]; 
	std::vector<char> states = dp[i][j].get_trace();

	// Recurse for all maximum states
	for (int y = 0; y < states.size(); y++) {

		t = x;	

		// If multiple paths
		if (y > 0) {
			alns.push_back(t_str);
			t = alns.size() - 1;
		}
		
		if (states[y] == 'M') {
			alns[t] += "M";
			traceback(i - 1, j - 1, t, alns);
		
		} else if (states[y] == 'I') {
			alns[t] += "I";
			traceback(i - 1, j, t, alns);
		
		} else if (states[y] == 'D') {
			alns[t] += "D";
			traceback(i, j - 1, t, alns);
		}

	}	
}


////////////////////////////////////////////////////////////////////
// Generic Dynamic Programming Implementation
int Alignment::align_aux(const int &match, const int &mismatch, const int &gap) {

	int m;

	for (int i = 1; i <= seq1.size(); i++) {	
		for (int j = 1; j <= seq2.size(); j++) {

			m = match;
			if (seq1[i - 1] != seq2[j - 1]) {
				m = mismatch;
			}

			dp[i][j].M = Alignment::set_max(
							dp[i-1][j-1].M + m,
							dp[i-1][j-1].D + m,
							dp[i-1][j-1].I + m
						  );
			dp[i][j].D = dp[i][j-1].max + gap;
			dp[i][j].I = dp[i-1][j].max + gap;
			dp[i][j].set_max();
		}
	}

	return dp[seq1.size()][seq2.size()].max;
}


////////////////////////////////////////////////////////////////////
// General Alignment Call
int Alignment::align(const std::string &method) {
		
	int match;
	int mismatch;
	int gap;

	// Initialize Scoring Penalities and Boundary Conditions
	if (method == "nw") {

		// Needleman Wunsch
		match = 1;
		mismatch = -1;
		gap = -2;

		for (int i = 0; i <= n; i++) { dp[i][0] = State(gap * i); }
		for (int j = 0; j <= m; j++) { dp[0][j] = State(gap * j); }

	} else if (method == "semi") {

		// Semi-global
		match = 1;
		mismatch = -1;
		gap = -2;

		// TO DO
		if (seq1.size() > seq2.size()) {
			for (int j = 0; j <= seq2.size(); j++) { dp[0][j] = State(gap * j); }
		} else {
			for (int i = 0; i <= seq1.size(); i++) { dp[i][0] = State(gap * i); } 
		}

	} else {
		std::cerr << "Error: Unknown alignment method " << method << "\n";
		exit(1);
	}

	// Perform Alignment
	score = align_aux(match, mismatch, gap);

	// print_dp_matrix();

	// Traceback Alignments
	int num_alignments = 0;
	traceback(n, m, num_alignments, this -> tb);

	return score;
}


//////////////////////////////////////////////////////////////////
// Print Dynamic Programming Matrix
void Alignment::print_dp_matrix() const {
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
			if (i == 0) {
				std::cerr << dp[i][j].D << "\t";
			} else if (j == 0) {
				std::cerr << dp[i][j].I << "\t";
			} else {
				std::cerr << dp[i][j] << "\t";
			}
		}
		std::cerr << "\n";
	}
}


//////////////////////////////////////////////////////////////////
// Print Alignment
void Alignment::print_alignment() const {

	int i, j;
	int x = 0;
	std::string aln_seq1;
	std::string aln_seq2;

	for (auto &t : tb) {
		
		i = 0; j = 0;
		aln_seq1 = ""; aln_seq2 = "";

		std::string str = t;
		std::reverse(str.begin(), str.end());

		for (const auto &c : str) {

			if (c == 'M') {
				aln_seq1 += seq1[i];
				aln_seq2 += seq2[j];
				i += 1; j += 1;

			} else if (c == 'I') {
				aln_seq1 += seq1[i];
				aln_seq2 += "_";
				i += 1;

			} else if (c == 'D') {
				aln_seq1 += "_";
				aln_seq2 += seq2[j];
				j += 1;

			}

		}

		std::cerr << "Alignment #" << x + 1 << " (Score: " << this -> score << "):\n";
		std::cerr << aln_seq1 << "\n" << aln_seq2 << "\n\n";
		x += 1;
	}
}
