#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "utils.h"

////////////////////////////////////////////////////////////////////
/// Item in Dynamic Programming Matrix
struct State {
	
public:

	int M;
	int D;
	int I;
	int max;

	/*
	States:
		M : Mis/Match
		D : Delete (gap in seq1, L)
		I : Insert (gap in seq2, U)
	*/ 

	// Constructor	
	State() : M(0), D(0), I(0) {}
	State(int _a) : M(_a), D(_a), I(_a), max(_a) {}
	State(int _m, int _d, int _i) : M(_m), D(_d), I(_i) {}

	void set_max() {
		max = std::max(std::max(M, D), I);
	}

	// Get Possible Paths
	std::vector<char> get_trace() {
		std::vector<char> max_states;
		if (M == max) { max_states.push_back('M'); }
		if (D == max) { max_states.push_back('D'); }
		if (I == max) { max_states.push_back('I'); }
		return max_states;
	}

	// Overload output operator for debugging
	friend std::ostream& operator<<(std::ostream& os, const State& state) {
		os << state.max;
		return os;
	}

};


////////////////////////////////////////////////////////////////////
// Alignment Class
class Alignment {

private:

	std::string seq1, seq2;
	std::string seq1_id, seq2_id;

	int n, m;
	int score = 0;

	std::vector<std::string> tb;
	std::vector<std::vector<State>> dp;

	// void traceback();
	void traceback(const int &i, const int &j, const int x, std::vector<std::string> &alns);
	int align_aux(const int &match, const int &mismatch, const int &gap);
	
public:

	Alignment() {}

	Alignment(const std::string &s1, const std::string &s2, const std::string &s1_id="", const std::string &s2_id="") 
	: seq1(s1), seq2(s2), seq1_id(s1_id), seq2_id(s2_id) {
		n = seq1.size();
		m = seq2.size();
		dp = std::vector<std::vector<State>>(n + 1, std::vector<State>(m + 1));
	}

	void reset() {
		score = 0;
		tb.clear();
		dp = std::vector<std::vector<State>>(n + 1, std::vector<State>(m + 1));
	}

	int get_score() const { return score; }
	int set_max(const int &a, const int &b, const int &c) {
		return std::max(std::max(a, b), c);
	}

	int align(const std::string &method);
	void print_alignment() const;
	void print_dp_matrix() const;
};