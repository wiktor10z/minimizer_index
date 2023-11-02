#ifndef MST_SET_H
#define MST_SET_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <list>
#include "heavy_string.h"
#include "PST.h"


using namespace std;

class MinimizerIndex{
	string alph;
	int N;
	vector<vector<double>> fP;
	
	string H;
	vector<double> pi_prefix; // TODO we can pass it to the heavy string constructor, so that we do not have to compute pi_suf there once again
	
	friend std::istream & operator >> (std::istream& input, MinimizerIndex &M);
	int find_minimzer_index(string& s, int k);
	int pruning(int first_pos,double p1);
	PropertySuffixTree * forward_index;
	
public:
	MinimizerIndex(): alph(), fP(), forward_index(NULL){}
	//MinimizerIndex(vector<vector<double>> &P, string &A, int ell, int k, double z);
	void build_index(double z,int ell);

	vector<int> occurrences(string const &P, int ell, double z, ostream& result) const;
};

#endif //MST_SET_H  
