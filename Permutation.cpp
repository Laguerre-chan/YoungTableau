#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using Permutation = vector<int>;

vector<Permutation> enum_permutation(int size){
	Permutation perm(size);
	vector<Permutation> result;
	for(int i=0; i<size; i++){
		perm[i] = i+1;
	}
	do{
		result.push_back(perm);
	}while(next_permutation(perm.begin(), perm.end()));
	return result;
}

void pretty_show_stat(Permutation perm, int n){
	printf("[");
	for(int i=0; i<perm.size(); i++){
		printf("%d", perm[i]);
		if(i < perm.size()-1){
			printf(",");
		}
	}
	printf("] => %d\n", n);
	return;
}

Permutation inverse(Permutation input_perm){
	//要素は1,2,..,nとする．
	vector<int> perm = input_perm;
	vector<int> inv_perm;
	for(int i=0; i<perm.size(); i++){
		for(int j=0; j<perm.size(); j++){
			if(perm[j] == i+1){
				inv_perm.push_back(j+1);
				break;
			}
		}
	}
	return inv_perm;
}

int comaj(Permutation p){
	int sum = 0;
	int size = p.size();
	for(int i=0; i<p.size()-1; i++){
		if(p[i] > p[i+1]){
			sum += size-(i+1);
		}
	}
	return sum;
}

int maj(Permutation p){
	int sum = 0;
	for(int i=0; i<p.size()-1; i++){
		if(p[i] > p[i+1]){
			sum += i+1;
		}
	}
	return sum;
}

int inv(Permutation p){
	int sum = 0;
	for(int i=0; i<p.size()-1; i++){
		for(int j=i+1; j<p.size(); j++){
			if(p[i]>p[j]){
				sum++;
			}
		}
	}
	return sum;
}
