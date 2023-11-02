/**
    Weighted Index
    Copyright (C) 2017 Carl Barton, Tomasz Kociumaka, Chang Liu, Solon P. Pissis and Jakub Radoszewski.
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
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <map>
#include "property_string.h"
#include "heavy_string.h"

using namespace std;

class PropertySuffixTree {
    typedef HeavyString::const_iterator position;
    struct stNode {
        stNode* suf_link;
        map<char, stNode*> children; // TODO we only use ordered map so that the dfs output is deterministic
        //stNode* parent;	//TODO can be substituted with a stack of nodes in the construction function
        position begin, end;
        list<size_t> minimizers;
        vector<int> labels;
		pair<int,int> SAinterval;
		bool keeper = false;
        
        stNode(position const &begin, position const &end);
        ~stNode();
        
        stNode* add_leaf(position const &begin, position const &end);
        void split_edge(PropertySuffixTree::position const &split);
        
        void print(ostream &out, int d) const;
        
        stNode* trim(bool is_root = false);
		stNode* keeper_trim(bool is_root = false);
        void listel(vector<int> &l) const;
    };
    
    struct Locus {
        stNode* node;
        position begin, end;
        
        stNode* descendant() const;
        bool is_explicit() const;
        void fast_forward();
        void suf_link();
        void forward(position limit);
		void mark_forward();
        stNode* make_explicit();
        
        bool operator < (Locus const &other) const;
        
        Locus() {}
        Locus(stNode* node, position const &begin, position const &end) : node(node), begin(begin), end(end) {}
	};
    
        
    stNode* root;
    Locus find(HeavyString &P) const;
    
    void build_suffix_tree();
    void build_suffix_tree2(list<pair<size_t,size_t>> min_substrings);
    //void process_property(vector<int> const& pi,std::vector<int> const& pos);
    friend std::ostream& operator<< (std::ostream &out, PropertySuffixTree const &st);
    
public:
    HeavyString text;

   //PropertySuffixTree(vector<int> const& S, HeavyString const& H, std::vector<int> const& pos);
   PropertySuffixTree(HeavyString const& H, list<pair<size_t,size_t>> min_substrings);
   
//    PropertySuffixTree(HeavyString const& H);

    //void minimizer_trim(vector<int> const& property, vector<int> const& pos);
	
	int number_of_nodes();	
    bool contains(string const &s) const;
    vector<int> occurrences(string const &s) const;
	pair<int,int> SAoccurrences(string const& s) const;
	void clean();
	std::vector<int> toSA();
	double get_pi(int i, int begin, int length);
	double naive_check(string const & pat, int p_begin, int t_begin, int length, int c);
	void dfs();
    ~PropertySuffixTree();
};
