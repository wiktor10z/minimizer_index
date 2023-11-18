#include <math.h>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <cstring>
#include <deque>
#include <set>
#include "krfp.h"
#include "minimizers.h"

using namespace std;
typedef uint64_t INT;  


MinimizerHeap::MinimizerHeap(uint64_t n1, uint64_t l1, uint64_t k1){
	n=n1;
	l=l1;
	k=k1;
	len=0;
	//S="";
	lefthash=0;
	righthash=0;
	letter_k=0;
	for(uint64_t i;i<k-1;++i){
		letter_k=karp_rabin_hashing::concat(0,letter_k,k-i+1);
	}
	letter_k=karp_rabin_hashing::concat(letter_k,'A',1);
}

uint64_t MinimizerHeap::top(){
	return heap.begin()->second;
}


void MinimizerHeap::left(char a){
	//S.insert(0, 1, a); // TODO maybe we can store S only here
	S.push_back(a);
	lefthash =  karp_rabin_hashing::concat( a, lefthash , min(k,S.size()-1));	
	if(S.size()>k){
		lefthash = karp_rabin_hashing::subtract(lefthash, S[S.size()-1-k] , 0 );
		lefthash = karp_rabin_hashing::leftshift(lefthash); 	
		heap.insert(make_pair(lefthash,n-S.size()));
	}else if(S.size()==k){
		heap.insert(make_pair(lefthash,n-S.size()));
	}
	if(S.size()> l){

		heap.erase(heap.find(make_pair(righthash,n-S.size()+l-k+1)));
		//righthash = karp_rabin_hashing::concat(S[S.size()-1-l+k], righthash, k);
		righthash = karp_rabin_hashing::concat_k(S[S.size()-1-l+k], righthash);
		righthash = karp_rabin_hashing::subtract(righthash, S[S.size()-1-l] , 0 );
		righthash = karp_rabin_hashing::leftshift(righthash);
	}
	if(S.size()<=k){
		righthash=lefthash;
	}
}

void MinimizerHeap::right(){
	if(heap.size()>0){
		heap.erase(heap.find(make_pair(lefthash,n-S.size())));
	}
	if(S.size()>k){
		lefthash = karp_rabin_hashing::concat(lefthash,S[S.size()-1-k],1);
		//lefthash = karp_rabin_hashing::subtract(lefthash, S[S.size()-1] , k);		
		lefthash = karp_rabin_hashing::subtract_k(lefthash, S[S.size()-1]);		
	}else{
		lefthash = karp_rabin_hashing::concat(lefthash,' ',0);
		//lefthash = karp_rabin_hashing::subtract(lefthash, S[S.size()-1] , k );
		lefthash = karp_rabin_hashing::subtract_k(lefthash, S[S.size()-1]);
		righthash = lefthash;
	}
	if(S.size()>l){
		righthash = karp_rabin_hashing::concat(righthash,S[S.size()-1-l],1);
		//righthash = karp_rabin_hashing::subtract(righthash, S[S.size()-1-l+k] , k );
		righthash = karp_rabin_hashing::subtract_k(righthash, S[S.size()-1-l+k]);
		heap.insert(make_pair(righthash,n-S.size()+l-k+1));		
	}
	//S = S.substr(1);
	S.pop_back();
}



uint64_t linear_minimizer(vector<char>& S, uint64_t l, uint64_t k){
	
	INT fp = 0;
	for(INT j = 0; j<k; j++)
		fp =  karp_rabin_hashing::concat( fp, S[S.size()-1-j] , 1 );
	INT pos = 1;
	INT min_fp = fp;
	INT minimizers = 0;
	for(INT j = 1; j<=l-k; j++)
	{
		fp = karp_rabin_hashing::concat( fp, S[S.size()-j-k] , 1 );
		fp = karp_rabin_hashing::subtract_k( fp, S[S.size()-j]);
		if(fp < min_fp){
			min_fp = fp;
			minimizers = j;
		}
	}
	return minimizers;
}

/*
INT compute_minimizers(  string& text, INT w, INT k, unordered_set<uint64_t> &minimizers )
{
	INT n = text.length();
	INT fp = 0;
	INT smallest_fp = fp;
	
	INT * FP = ( INT * ) malloc( ( n - k + 1  ) *  sizeof( INT ) );
	
	for(INT j = 0; j<k; j++)
		fp =  karp_rabin_hashing::concat( fp, text[j] , 1 );
		
	FP[0] = fp;
	INT pos = 1;
	//cout<<fp<<endl;
	deque<pair<INT,INT>> min_fp = {};
	
	// find all fps for all k substrings
	for(INT j = 1; j<=n-k; j++)
	{
		fp = karp_rabin_hashing::concat( fp, text[j+k-1] , 1 );
		fp = karp_rabin_hashing::subtract( fp, text[j-1] , k );
		
		//cout<<fp<<endl;
		FP[pos] = fp;
		pos++;
	}	
	
		
	// minimum fp in first window
   	for (INT j = 0; j <= w - k ; j++) 
   	{
 		while ( !min_fp.empty() && FP[j] < min_fp.back().first )
 			min_fp.pop_back();
				
		min_fp.push_back(std::make_pair(FP[j], j));
		
		
		
    	}
	minimizers.insert( min_fp.at(0).second );
	
	// minium fp in remaining windows
	for( INT i = w-k+1; i<=n-k; i++ )
	{
		while (!min_fp.empty() && min_fp.back().first > FP[i])
			min_fp.pop_back();

				
		min_fp.push_back(std::make_pair(FP[i], i));
		
	
		while( !min_fp.empty() && min_fp.front().second <= i - w + k)
		{
			min_fp.pop_front();
		}	
		
		minimizers.insert( min_fp.at(0).second );

	}
	
	free( FP );
	return 0;
}

*/

INT pattern_minimizers(string& text,INT k)
{
	INT n = text.length();
	
	
	INT fp = 0;
		
	for(INT j = 0; j<k; j++)
		fp =  karp_rabin_hashing::concat( fp, text[j] , 1 );
		
	//FP[0] = fp;
	//cout<<fp<<endl;
	INT min_fp = fp;
	INT minimizers = 0;
	
	// find all fps for all k substrings
	for(INT j = 1; j<=n-k; j++)
	{
		fp = karp_rabin_hashing::concat( fp, text[j+k-1] , 1 );
		fp = karp_rabin_hashing::subtract_k( fp, text[j-1]);
		
		if(fp < min_fp){
			min_fp = fp;
			minimizers = j;
		}		
	}
	/*
	INT fp=0;
	for(INT j = n-k; j<n; j++)
		fp =  karp_rabin_hashing::concat( fp, text[j] , 1 );	
	INT min_fp = fp;
	INT minimizers = n-k;	
	for(int j=n-k-1;j>=0;j--){
		fp = karp_rabin_hashing::concat_k(text[j], fp);
		fp = karp_rabin_hashing::subtract(fp, text[j+k] , 0 );
		fp = karp_rabin_hashing::leftshift(fp);
		if(fp <= min_fp){
			min_fp = fp;
			minimizers = j;
		}
	}
	*/
	return minimizers;
}
