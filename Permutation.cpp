#include <iostream>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <set>
#include <cassert>
#include <utility>

using namespace std;

class Permutation{
	//要素は1,2,..,nとする．
	public:
		//コンストラクタ
		Permutation(vector<int>);
		Permutation(initializer_list<int>);
		Permutation(int size, int i, int j);

		//置換への全単射
		Permutation reverse() const;
		Permutation inverse() const;
		Permutation complement() const;
		Permutation kari() const;

		//code
		vector<int> Lehmer() const;

		//統計量
		int inv() const;
		int maj() const;
		int comaj() const;

		//演算子
		bool operator==(const Permutation&) const;
		//Bruhat順序での被覆
		bool operator<<(const Permutation&) const;
		//弱順序での被覆
		bool operator<(const Permutation&) const;
		Permutation operator*(const Permutation&) const;

		//特定の性質を満たすかどうか判定する系
		bool avoiding(const Permutation&) const;
		bool is_vincular_pattern_avoiding(string pattern) const;

		//べんり関数
		void show() const;
		int size() const;
		void show_findstat(int) const;
		void show_findmap(const Permutation &) const;

		//ゲッター・セッター
		vector<int> get_entry(void) const;
		int get_entry(int position) const;

	private:
		vector<int> entry;
		bool check_vector_is_permutation(vector<int>) const;
		vector<int> calculate_relative_rank(const vector<int> &) const;
};

//calculate_relative_rankは，列を正規化する関数
// example input: 5 3 2 6 3 9
// output       : 3 2 1 4 2 5
vector<int> Permutation::calculate_relative_rank(const vector<int> & vec) const{
	assert(not vec.empty());
	vector< pair<int,int> > indexed_nums(vec.size());
	for(int i=0; i<vec.size(); i++){
		indexed_nums[i] = {vec[i], i};
	}
	sort(indexed_nums.begin(), indexed_nums.end());
	int last = indexed_nums[indexed_nums.size()-1].first;
	int rank = 1;
	for(int i=0; i<indexed_nums.size(); i++){
		int kari = indexed_nums[i].first;
		indexed_nums[i].first = rank;
		if(i < indexed_nums.size()-1 && kari < indexed_nums[i+1].first){
			rank++;
		}
	}
	sort(indexed_nums.begin(), indexed_nums.end(), 
			[](pair<int,int> a, pair<int,int> b){return a.second < b.second;});
	vector<int> result(vec.size());
	for(int i=0; i<vec.size(); i++){
		result[i] = indexed_nums[i].first;
	}
	return result;
}
bool Permutation::check_vector_is_permutation(vector<int> vec) const{
	vector<int> sorted = vec;
	sort(sorted.begin(), sorted.end());
	for(int i=0; i<sorted.size(); i++){
		if(sorted[i] != i+1){
			return false;
		}
	}
	return true;
}

//特定の性質を満たすかどうか判定する系
bool Permutation::avoiding(const Permutation& pattern) const{
	if(this->size() < pattern.size()){
		return true;
	}
	vector<int> choice(this->size());
	for(int i=0; i<pattern.size(); i++){
		choice[choice.size()-1-i] = 1;
	}
	do{
		vector<int> subperm_entry;
		for(int i=0; i<choice.size(); i++){
			if(choice[i]){
				subperm_entry.push_back(this->entry[i]);
			}
		}
		subperm_entry = calculate_relative_rank(subperm_entry);
		if(pattern.get_entry() == subperm_entry){
			return false;
		}
	}while(next_permutation(choice.begin(), choice.end()));
	return true;
}

//演算子
bool Permutation::operator==(const Permutation& other) const{
	return this->entry == other.entry;
}
Permutation Permutation::operator*(const Permutation& right) const{
	assert(this->size() == right.size());
	vector<int> new_entry(this->size());
	for(int i=0; i<this->size(); i++){
		new_entry[i] = this->get_entry(right.get_entry(i)-1);
	}
	Permutation product(new_entry);
	return product;
}
//Bruhat順序での被覆
bool Permutation::operator<<(const Permutation& other) const{
	if(this->size() != other.size()){
		return false;
	}
	for(int a=1; a<=(int)(this->size())-1; a++){
		for(int b=a+1; b<=(int)(this->size()); b++){
			Permutation transposition(this->size(), a, b);
			if(not (other == (*this) * transposition) ){
				continue;
			}
			if(this->entry[a-1] >= this->entry[b-1]){
				continue;
			}
			int check_c_flag = 0;
			for(int c=a+1; c<b; c++){
				if(this->entry[a-1] < this->entry[c-1] && this->entry[c-1] < this->entry[b-1]){
					check_c_flag = 1;
					break;
				}
			}
			if(check_c_flag == 0){
				return true;
			}
		}
	}
	return false;
}
//弱順序での被覆
bool Permutation::operator<(const Permutation& other) const{
	if(this->size() != other.size()){
		return false;
	}
	for(int i=0; i<this->size()-1; i++){
		if(this->entry[i] < this->entry[i+1]){
			Permutation tau(this->size(), this->entry[i], this->entry[i+1]);
			if(tau*(*this) == other){
				return true;
			}
		}
	}
	return false;
	
}

//べんり関数
int Permutation::size() const{
	return entry.size();
}
void Permutation::show() const{
	for(int i=0; i<entry.size(); i++){
		printf("%2d", entry[i]);
	}
	printf("\n");
	return;
}
void Permutation::show_findstat(int n) const{
	printf("[");
	for(int i=0; i<this->entry.size(); i++){
		printf("%d", this->entry[i]);
		if(i < this->entry.size()-1){
			printf(",");
		}
	}
	printf("] => %d\n", n);
	return;
}

void Permutation::show_findmap(const Permutation &other) const{
	printf("[");
	for(int i=0; i<this->size(); i++){
		printf("%d", this->entry[i]);
		if(i < this->size()-1){
			printf(",");
		}
	}
	printf("] => [");
	for(int i=0; i<other.size(); i++){
		printf("%d", other.get_entry(i));
		if(i < other.size()-1){
			printf(",");
		}
	}
	printf("]\n");
	return;
}

//ゲッター
vector<int> Permutation::get_entry() const{
	return entry;
}
int Permutation::get_entry(int position) const{
	return entry[position];
}

//code
vector<int> Permutation::Lehmer() const{
	vector<int> code(entry.size());
	for(int i=0; i<code.size(); i++){
		int sm = 0;
		for(int j=0; j<i; j++){
			sm += (entry[j] > entry[i]);
		}
		code[i] = sm;
	}
	return code;
}

//コンストラクタ
Permutation::Permutation(vector<int> init_entry){
	assert(check_vector_is_permutation(init_entry));
	entry = init_entry;
}
Permutation::Permutation(initializer_list<int> init_list) {
	vector<int> init_entry(init_list);
	assert(check_vector_is_permutation(init_entry));
	entry = init_entry;
}
Permutation::Permutation(int size, int i, int j){
	assert(1 <= i && i <= size);
	assert(1 <= j && j <= size);
	vector<int> init_entry(size);
	for(int i=0; i<size; i++){
		init_entry[i] = i+1;
	}
	init_entry[i-1] = j;
	init_entry[j-1] = i;
	entry = init_entry;
}

//統計量
int Permutation::maj() const{
	int sum = 0;
	for(int i=0; i<entry.size()-1; i++){
		if(entry[i] > entry[i+1]){
			sum += i+1;
		}
	}
	return sum;
}
int Permutation::inv() const{
	int sum = 0;
	for(int i=0; i<entry.size()-1; i++){
		for(int j=i+1; j<entry.size(); j++){
			if(entry[i]>entry[j]){
				sum++;
			}
		}
	}
	return sum;
}
int Permutation::comaj() const{
	int sum = 0;
	for(int i=0; i<this->size()-1; i++){
		if(this->entry[i] > this->entry[i+1]){
			sum += this->size()-(i+1);
		}
	}
	return sum;
}

//置換への全単射
Permutation Permutation::kari() const{
	vector<int> perm_entry = entry;
	for(int i=0; i<perm_entry.size(); i++){
		if(perm_entry[i] == perm_entry.size()){
			perm_entry[i] = 1;
		}else{
			perm_entry[i]++;
		}
	}
	Permutation karied(perm_entry);
	return karied;
}
Permutation Permutation::reverse() const{
	vector<int> perm_entry = entry;
	std::reverse(perm_entry.begin(), perm_entry.end());
	Permutation reversed(perm_entry);
	return reversed;
}
Permutation Permutation::inverse() const{
	vector<int> perm = entry;
	vector<int> inv_perm;
	for(int i=0; i<perm.size(); i++){
		for(int j=0; j<perm.size(); j++){
			if(perm[j] == i+1){
				inv_perm.push_back(j+1);
				break;
			}
		}
	}
	Permutation inversed(inv_perm);
	return inversed;
}
Permutation Permutation::complement() const{
	int size = entry.size();
	vector<int> comp = entry;
	for(int i=0; i<comp.size(); i++){
		comp[i] = size - comp[i] + 1;
	}
	Permutation complemented(comp);
	return complemented;
}


vector<Permutation> enum_permutation(int size){
	vector<int> entry(size);
	vector<Permutation> result;
	for(int i=0; i<size; i++){
		entry[i] = i+1;
	}
	do{
		Permutation perm(entry);
		result.push_back(perm);
	}while(next_permutation(entry.begin(), entry.end()));
	return result;
}

void show(vector<int> vec){
	for(int i=0; i<vec.size(); i++){
		printf("%2d", vec[i]);
	}
	printf("\n");
	return;
}

int xmain(){
	//Permutation pattern{2,3,1};
	Permutation sigma{4,2,1,3};
	vector<Permutation> perms = enum_permutation(4);
	for(Permutation perm: perms){
		if(sigma < perm){
			printf("\\draw (");
			for(int i=0; i<4; i++){
				printf("%d", sigma.get_entry(i));
			}
			printf(")--(");
			for(int i=0; i<4; i++){
				printf("%d", perm.get_entry(i));
			}
			printf(");\n");
		}
	}
	return 0;
}
int main(){
	Permutation pattern{3,1,2};
	vector<Permutation> perms = enum_permutation(4);
	for(Permutation &perm: perms){
		if(perm.avoiding(pattern)){
			perm.show();
		}
	}

	return 0;
}
