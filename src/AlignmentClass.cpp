#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "AlignmentClass.h"
#include "utils.h"


/////////////////////////////////////////////////////////////
// Get Max of Vector Utility
int Alignment::get_max(const std::vector<int> vec, const std::string &method) {
    int res = vec[0];
    if (method == "sw") { res = 0; } // Maximum Score in SW is 0.
    for (const auto &v: vec) { res = std::max(res, v); }
    return res;
}


/////////////////////////////////////////////////////////////
// F Matrix Scoring Function
int Alignment::score_funct(const int &i, const int &j, const char c, const std::string &method) {
    int score, s;
    if (c == 'M') {
        s = match;
        if (seq1[i - 1] != seq2[j - 1]) { s = mismatch; }   
        score = Alignment::get_max({
                                    dp[i-1][j-1].M + s,
                                    dp[i-1][j-1].D + s,
                                    dp[i-1][j-1].I + s
                                    },
                                   method);
    } else if (c == 'D') {
        score = Alignment::get_max({dp[i][j-1].max + gap}, method);
    } else {
        score = Alignment::get_max({dp[i-1][j].max + gap}, method);
    }
    return score;
}



/////////////////////////////////////////////////////////////
// Perform Traceback
void Alignment::traceback(const int i, const int j, const int x, 
                          std::vector<std::string> &alns, const std::string &method) {

    // Initilize Alignments Vector
    if (alns.empty()) { alns.push_back(""); }

    // Termination / Special Traceback Cases
    if (i <= 0 && j <= 0) {
        return; 
    } else if (i == 0 && j > 0) {
        alns[x] += "D";
        traceback(i, j - 1, x, alns, method);
        return;
    } else if (i > 0 && j == 0 || (max_i < i && method == "sg")) {
        if (method == "sw") { return; }
        alns[x] += "I";
        traceback(i - 1, j, x, alns, method);
        return;
    }

    int t;
    std::string t_str = alns[x]; 
    std::vector<char> states = dp[i][j].get_trace();

    // Recurse for all maximum states (possible paths)
    for (int y = 0; y < states.size(); y++) {

        // If multiple paths
        t = x;  
        if (y > 0) {
            alns.push_back(t_str);
            t = alns.size() - 1; // New aln string index
        }

        // Traceback other routes        
        if (states[y] == 'M') {
            alns[t] += "M";
            traceback(i - 1, j - 1, t, alns, method);
        } else if (states[y] == 'I') {
            alns[t] += "I";
            traceback(i - 1, j, t, alns, method);
        } else if (states[y] == 'D') {
            alns[t] += "D";
            traceback(i, j - 1, t, alns, method);
        }

    }   
}


/////////////////////////////////////////////////////////////
// Iterative Dynamic Programming Implementation
void Alignment::align_itr(const std::string &method) {

    for (int i = 1; i <= n; i++) {    
        for (int j = 1; j <= m; j++) {

            dp[i][j].M = Alignment::score_funct(i, j, 'M', method);
            dp[i][j].D = Alignment::score_funct(i, j, 'D', method);
            dp[i][j].I = Alignment::score_funct(i, j, 'I', method);    
           
            dp[i][j].set_max();
            Alignment::record_max(i, j, method);
        }
    }
}


/////////////////////////////////////////////////////////////
// Recursive Dynamic Programming Implementation
void Alignment::align_rec(const int i, const int j, const std::string &method) {

    int s;
    if (dp[i][j].visited == true) { return; }

    align_rec(i-1, j-1, method);
    align_rec(i, j-1, method);
    align_rec(i-1, j, method);

    dp[i][j].M = Alignment::score_funct(i, j, 'M', method);
    dp[i][j].D = Alignment::score_funct(i, j, 'D', method);
    dp[i][j].I = Alignment::score_funct(i, j, 'I', method);

    dp[i][j].set_max();
    Alignment::record_max(i, j, method);
}


/////////////////////////////////////////////////////////////
// General Alignment Call
int Alignment::align(const std::string &method, const std::string &algo) {
        
    tb_i = n;
    tb_j = m;

    // Initialize Scoring Penalities and Boundary Conditions
    if (method == "nw") {
        for (int i = 0; i <= n; i++) { dp[i][0] = State(gap * i); }
        for (int j = 0; j <= m; j++) { dp[0][j] = State(gap * j); }
        max_i = n;
        max_j = m;
    } else if (method == "sg") {
        for (int i = 0; i <= n; i++) { dp[i][0] = State(0); }
        for (int j = 0; j <= m; j++) { dp[0][j] = State(gap * j); }
        max_j = m;
    } else if (method == "sw") {
        for (int i = 0; i <= n; i++) { dp[i][0] = State(0); }
        for (int j = 0; j <= m; j++) { dp[0][j] = State(0); }
    } else {
        std::cerr << "Error: Unknown alignment method " << method << "\n";
        exit(1);
    }

    // Perform Alignment and get score
    if (algo == "rec") {
        align_rec(n, m, method);
    } else {
        align_itr(method);
    }
    max_score = dp[max_i][max_j].max;

    // Adjust Traceback Starts if needed
    if (method == "sw") { tb_i = max_i; tb_j = max_j; }
    if (method == "sg") { tb_i = n; }

    // Traceback Alignments
    int num_alignments = 0;
    traceback(tb_i, tb_j, num_alignments, this -> tb, method);

    return Alignment::get_score();
}

/////////////////////////////////////////////////////////////
// Print Alignment
void Alignment::print_alignment() const {

    int i, j;
    int x = 0;
    std::string cons;
    std::string aln_seq1;
    std::string aln_seq2;

    // For each possible alignment
    for (auto &t : tb) {
        
        cons = "";
        aln_seq1 = "";
        aln_seq2 = "";
        i = tb_i; j = tb_j;

        // Iterate through traceback strings
        for (const auto &c : t) {

            if (c == 'M') {
                aln_seq1 = seq1[i - 1] + aln_seq1;
                aln_seq2 = seq2[j - 1] + aln_seq2;
                if (seq1[i - 1] == seq2[j - 1]) {
                    cons = '.' + cons;
                } else {
                    cons = seq2[j - 1] + cons;
                    if (flip) { cons[0] = seq1[i - 1]; }
                }
                i -= 1; j -= 1;

            } else if (c == 'I') {
                aln_seq1 = seq1[i - 1] + aln_seq1;
                aln_seq2 = "_" + aln_seq2;
                cons = '-' + cons;    
                if (flip) { cons[0] = seq1[i - 1]; } // Replace is output is flipped
                i -= 1;

            } else if (c == 'D') {
                aln_seq1 = "_" + aln_seq1;
                aln_seq2 = seq2[j - 1] + aln_seq2;
                cons = seq2[j - 1] + cons;
                if (flip) { cons[0] = '-'; } // Replace is output is flipped
                j -= 1;
            }
        }

        // Report Score and Alignments
        std::cout << "Alignment #" << x + 1 << " (Score: " << this -> max_score << "):\n";
        if (flip) {
            std::cout << aln_seq2 << "\n" << aln_seq1 << "\n" << cons << "\n\n";
        } else {
            std::cout << aln_seq1 << "\n" << aln_seq2 << "\n" << cons << "\n\n";
        }
        x += 1;
    }
}