#ifndef HEAVY_STRING_H 
#define HEAVY_STRING_H

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <stdexcept> 
#include <iostream>

class HeavyString{
	std::string H;
	std::map<size_t, char> _alt;
	std::map<size_t, double> delta_pi;
	//std::unordered_map<int, std::vector<int>> alt_pos;
	std::map<int, std::pair<int, int>> alt_ext;
	std::vector<double> pi_suf;
	int n;
	int N;

	public:
	HeavyString(): n(0), N(0){};
	
	HeavyString(std::vector<std::vector<double>>& P, std::string const& S, std::string& A) 
		: n(P.size()), N(S.size()) {
			if (n == 0 || N == 0) {
				throw std::invalid_argument("P and S cannot be empty.");
			}

			for(size_t i = 0; i < n; i++){
				int which_max = max_element(P[i].begin(), P[i].end()) - P[i].begin();
				H+=(A[which_max]);
			}		
			for(size_t i = 0; i < N; i++){
				if(S[i] != H[i%n]){
					if (A.find(S[i]) == std::string::npos) {
						throw std::invalid_argument("S contains a character not in A.");
					}
					_alt[i] = S[i];
				}
			}
		}	
		
	HeavyString(std::vector<std::vector<double>>& P,  std::string const& S, std::string& A, std::unordered_set<int> min_pos, std::vector<int> le, std::vector<int> re, bool create_pi){
		n = P.size();
		N = S.size();
		std::vector<int> min_pos2(min_pos.begin(),min_pos.end());
		std::sort(min_pos2.begin(),min_pos2.end());
		if (n == 0 || N == 0) {
			throw std::invalid_argument("P and S cannot be empty.");
		}
		
		std::vector<double> pi_arr;
		
		for(size_t i = 0; i < n; i++){
			int which_max = max_element(P[i].begin(), P[i].end()) - P[i].begin();
			H+=(A[which_max]);
			double pi = log2(P[i][which_max]);
			pi_arr.push_back(pi);
		}
		
		if(create_pi){				
			pi_suf.assign(pi_arr.begin(), pi_arr.end());
			for(int i = n-2; i >= 0; i--){
				pi_suf[i] += pi_suf[i+1];
			}
		}
		
		for(int m : min_pos2){
			alt_ext[m]=std::make_pair(le[m],re[m]);
		}
		
		std::map<int,std::pair<int,int>>::iterator iter=alt_ext.begin();
		int i=iter->first-iter->second.first;
		while(true){
			if(i>iter->first+iter->second.second){
				++iter;
				while((iter!=alt_ext.end())&&(i>iter->first+iter->second.second)){
					++iter;
				}
				if(iter==alt_ext.end()) return;
				if(iter->first-iter->second.first>i) i=iter->first-iter->second.first;
			}
			int h = i%n;
			if(H[h]!=S[i]){
				double this_pi = log2(P[h][A.find(S[i])]);
				_alt[i] = S[i];
				delta_pi[i] =  this_pi - pi_arr[h];
			}
			++i;
		}
	}
	
	HeavyString(const HeavyString& other): H(other.H), _alt(other._alt), n(other.n), N(other.N) {}

	HeavyString& operator=(const HeavyString& other) {
		if (this != &other) {
			H = other.H;
			n = other.n;
			N = other.N;
			std::map<size_t, char> temp(other._alt);
			std::swap(_alt, temp);
		}
		return *this;
	}

	char& operator[](size_t i){ 
		if (i >= N) {
			throw std::out_of_range("Index out of range.");
		}

		if(_alt.count(i)){
			return _alt.at(i);
		}else{
			return H[i%n];
		}
	}
	
	char& at(size_t i){ 
		if (i >= N) {
			throw std::out_of_range("Index out of range.");
		}

		if(_alt.count(i)){
			return _alt.at(i);
		}else{
			return H[i%n];
		}
	}
	
	std::string substr(size_t pos){
		if(pos >= N){
			return "";
		}
		std::string substring = H.substr(pos%n);
		std::map<size_t,char>::iterator alt_iter = _alt.lower_bound(pos);
		while((alt_iter!=_alt.end()) && (alt_iter->first<pos+substring.size())){
			substring[alt_iter->first-pos]=alt_iter->second;
			++alt_iter;
		}
		return substring;			
	}

	std::string substr(size_t pos, size_t len = 0){
		if (pos >= N || pos%n + len > n) {
			return "";
		}
		if(len == 0){	//if no length is given, print to the end
			len = n- pos%n;
		}
		std::string substring = H.substr(pos%n, len);
		std::map<size_t,char>::iterator alt_iter = _alt.lower_bound(pos);
		while((alt_iter!=_alt.end()) && (alt_iter->first<pos+len)){
			substring[alt_iter->first-pos]=alt_iter->second;
			++alt_iter;
		}

		return substring;
	}
	
	double get_pi(int i, int begin, int length){		
		if(begin%n > i%n)			return 0;
		if(begin%n + length > n)	return 0;
		if( i - alt_ext[i].first > begin ) return 0;
		if( i + alt_ext[i].second < begin + length - 1 ) return 0;
		
		int end = begin + length;
		double cum_pi = pi_suf[begin%n] - pi_suf[end%n];	
		std::map<size_t,double>::iterator alt_iter = delta_pi.lower_bound(begin);
		while((alt_iter!=delta_pi.end()) && (alt_iter->first<end)){
			cum_pi += alt_iter->second;
			++alt_iter;
		}
		return pow(2,cum_pi);
		
	}
	
	double check_pi(std::string& pat, size_t pat_begin, size_t txt_begin, size_t length, size_t min_pos){
		for(auto i = 0; i < length; i++){
			if(pat[pat_begin + i] != this->at(txt_begin+i)){
				return 0;
			}
		}
		return this->get_pi(min_pos,txt_begin, length);
	}
	
	size_t le(size_t i){
		return alt_ext[i].first;
	}
	
	size_t re(size_t i){
		return alt_ext[i].second;
	}
	
	size_t length() const {return N;}
	size_t size() const {return N;}
	size_t heavy_length() const {return n;}

	class Iterator : public std::iterator<std::random_access_iterator_tag, char> {
		HeavyString* hs; 
		size_t index;
		public:
		Iterator(HeavyString* hs, size_t index) : hs(hs), index(index) {}

		char& operator*() {
			return (*hs)[index];
		}

		Iterator& operator++() { 
			++index;
			return *this;
		}

		Iterator operator++(int) {
			Iterator temp = *this;
			++index;
			return temp;
		}

		Iterator& operator--() {
			--index;
			return *this;
		}

		Iterator operator--(int) {
			Iterator temp = *this;
			--index;
			return temp;
		}

		Iterator operator+(size_t n) const {
			return Iterator(hs, index + n);
		}

		friend Iterator operator+(size_t n, const Iterator& it) {
			return it + n;
		}

		Iterator& operator+=(size_t n) {
			index += n;
			return *this;
		}

		Iterator operator-(size_t n) const {
			return (*this) + (-n);
		}

		friend ptrdiff_t operator-(const Iterator& lhs, const Iterator& rhs) {
			return lhs.index - rhs.index; 
		}

		Iterator& operator-=(size_t n) {
			(*this) += (-n);
			return *this;
		}

		bool operator<(const Iterator& other) const {
			return index < other.index; 
		}

		bool operator>(const Iterator& other) const {
			return other < (*this); 
		}

		bool operator<=(const Iterator& other) const {
			return !((*this) > other); 
		}

		bool operator>=(const Iterator& other) const {
			return !((*this) < other); 
		}

		bool operator==(const Iterator& other) const {
			return index == other.index; 
		}

		bool operator!=(const Iterator& other) const {
			return !((*this)==other); 
		}   
	};


	Iterator begin() {return Iterator(this, 0);}
	Iterator end() {return Iterator(this,N);}
};


std::ostream& operator<<(std::ostream& os, HeavyString& hs);

#endif
