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
	istream& text = st.text.is_open()?st.text:cin;
	ostream& output_file = st.output.is_open()?st.output:cout;
	ofstream result;
	
	auto begin = get_time::now();
	// struct mallinfo2 mi;
    // mi = mallinfo2();
	// double begin_ram = mi.hblkhd + mi.uordblks;
	
	karp_rabin_hashing::init();
	MinimizerIndex M;
	text >> M;
	cout << "finish reading" << endl;
	M.build_index(st.z,st.ell);
	cout << "Minimizer Index build finish" << endl;

	// mi = mallinfo2();
	
	// double end_ram = mi.hblkhd + mi.uordblks;
	auto end = get_time::now();
	auto diff2 = end - begin;
	output_file << "CT "<< chrono::duration_cast<chrono::milliseconds>(diff2).count()<<endl;	
	// output_file << "CS " << (end_ram-begin_ram)/1000000 << endl;

	return 0;
}

