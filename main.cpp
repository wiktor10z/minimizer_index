	/**
    Weighted Suffix Array
    Copyright (C) 2017 Panagiotis.Charalampopoulos, Costas.Iliopoulos, Chang.Liu, Solon.Pissis
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include <iostream>
#include <cstdio>
#include <string>
#include <chrono>
#include <list>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <malloc.h>
#include <cmath>

#include "input.h"
#include "minimizer_index.h"
#include "krfp.h"
#include "heavy_string.h"


using namespace std;
using get_time = chrono::steady_clock;

int main (int argc, char ** argv )
{
    Settings st = decode_switches(argc, argv);
	istream& text_file = st.text.is_open()?st.text:cin;
	ostream& output_file = st.output.is_open()?st.output:cout;
	ofstream result;
	
	auto begin = get_time::now();
	// struct mallinfo2 mi;
    // mi = mallinfo2();
	// double begin_ram = mi.hblkhd + mi.uordblks;
	
	double z = st.z;
	int ell = st.ell;

	string alphabet;
	vector<vector<double>> text;

		karp_rabin_hashing::init();
	int   N;
	text_file >> N;
	text_file >> alphabet;
	int   A = alphabet.size();
	for (int   i = 0; i < N; ++i) {
        double sum = 0;
        vector<double> symbol(A, 0);
        for (int   j = 0; j < A; ++j) {
            text_file >> symbol[j];
            sum += symbol[j];
        }
        if (abs(sum-1) > 1e9) {
            cerr << "Probabilities at position " << i << " do not sum up to 1" << endl;
            throw 1;
        }
        text.emplace_back(symbol);
    }
	
	int k = ceil(log2(ell) / log2(alphabet.size())); //TODO multiply by 4, but this multiplication forces large l
	int w = ell - k + 1;
	
	cout << "finish reading" << endl;
	MinimizerIndex SET(text, alphabet, k, ell, z);
	cout << "Minimizer Index build finish" << endl;
	
	//SET.bfs();
	//cout << endl;
	//SET.dfs();
	//SET.compacteddfs();


	// mi = mallinfo2();
	
	// double end_ram = mi.hblkhd + mi.uordblks;
	auto end = get_time::now();
	auto diff2 = end - begin;
	output_file << "CT "<< chrono::duration_cast<chrono::milliseconds>(diff2).count()<<endl;	
	// output_file << "CS " << (end_ram-begin_ram)/1000000 << endl;

	return 0;
}

