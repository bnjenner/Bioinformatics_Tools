#pragma once

#include <iostream>
#include <getopt.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>


////////////////////////////////////////////////////////////////////
// Object in Dynamic Programming F Matrix
struct State {
    
public:

    int M;           // Mis/Match
    int D;           // Delete (gap in seq1, L)
    int I;           // Insert (gap in seq2, U)
    int max;         // Max Value
    bool visited;    // Visited

    // Constructors 
    State() : M(0), D(0), I(0), visited(false) {}
    State(int _a) : M(_a), D(_a), I(_a), max(_a), visited(true) {}
    State(int _m, int _d, int _i) : M(_m), D(_d), I(_i), visited(false) {}

    // Make State Decision
    void set_max() {
        max = std::max(std::max(M, D), I);
        visited = true;
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

    int n, m;                                  // Length of sequences

    int match = 1;                             // Match Reward
    int mismatch = -1;                         // Mismatch Penality
    int gap = -2;                              // Gap Penality

    int max_score = -1;                        // Max Alignment Score
    int max_i = -1, max_j = -1;                // Max Position
    int tb_i = -1, tb_j = -1;                  // Traceback Start Position

    bool flip = false;                         // Flip Sequences in Output (not necessary)

    std::string seq1, seq2;                    // Input Sequences
    std::vector<std::string> tb;               // Traceback Paths
    std::vector<std::vector<State>> dp;        // F Matrix

    //////////////////////////////////
    // Private Alignment Methods
    void align_itr(const std::string &method);
    void align_rec(const int i, const int j, const std::string &method);
    void traceback(const int i, const int j, const int x, std::vector<std::string> &alns, const std::string &method);
    
    // Private Scoring Utilities
    int get_max(const std::vector<int> vec, const std::string &method);
    int score_funct(const int &i, const int &j, const char c, const std::string &method);
    void record_max(const int &i, const int &j, const std::string &method) {
        // Keep track of max scores for sw and sg
        if (method == "sw" && max_score < dp[i][j].max) {
            max_score = dp[i][j].max;
            max_i = i, max_j = j;   
        } else if (method == "sg" && j == this -> m && max_score < dp[i][j].max) {
            max_score = dp[i][j].max;
            max_i = i;
        }
    }

public:

    //////////////////////////////////
    // Constructors 
    Alignment() {}
    Alignment(const std::string &s1, const std::string &s2) : seq1(s1), seq2(s2) {
        n = seq1.size(); m = seq2.size();
        if (m > n) { // Flip to keep seq2 smallest
            n = seq2.size(); m = seq1.size();
            seq1 = s2; seq2 = s1;
            flip = true;
        }
        dp = std::vector<std::vector<State>>(n + 1, std::vector<State>(m + 1));
    }

    //////////////////////////////////
    // Public Methods
    int align(const std::string &method, const std::string &algo);
    int get_score() const { return max_score; }
    void print_alignment() const;
    void reset() {
        max_score = -1;
        max_i = -1, max_j = -1;
        tb_i = -1, tb_j = -1;
        tb.clear();
        dp = std::vector<std::vector<State>>(n + 1, std::vector<State>(m + 1));
    }
};