#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <queue>
#include <cassert>

using namespace std;
using Permutation = vector<int>;
using Position = pair<int,int>;


class Tableau{ 
	public:
		//表示
		void show();

		//コンストラクタ
		Tableau(vector< vector<int> > tab);
		Tableau(vector<int> partition);

		//統計量
		vector<int> sh();
		int size();
		int sum();
		int abssum();
		int max();

		//ゲッター，セッター
		int get_entry(Position pos);
		void set_entry(Position pos, int var);
		void set_entries(vector<int> word);

		//ほぼNPSのための関数
		Position ordering(int);
		Position slide(Position);

		//ほぼSYTのための関数
		vector<Position> insertable_positions();
		vector<int> read_by_row();
		vector<int> read_by_order();

	private:
		int INFTY = 100;
		vector< vector<int> > entry;
		Position slide_step(Position);
		void swap(Position a, Position b);
};

void Tableau::set_entries(vector<int> word){
	assert(this->size() == word.size());
	int count = 0;
	for(int i=0; i<this->entry.size(); i++){
		for(int j=0; j<this->entry[i].size(); j++){
			this->entry[i][j] = word[count];
			count++;
		}
	}
	return;
}

vector<int> Tableau::read_by_order(){
	vector<int> vec;
	for(int i=0; i<this->size(); i++){
		vec.push_back(this->get_entry(this->ordering(i+1)));
	}
	return vec;
}

vector<int> Tableau::read_by_row(){
	vector<int> vec;
	for(int i=0; i<this->entry.size(); i++){
		for(int j=0; j<this->entry[i].size(); j++){
			vec.push_back(this->entry[i][j]);
		}
	}
	return vec;
}

vector<Position> Tableau::insertable_positions(){
	vector<Position> result;
	for(int i=0; i<this->entry.size(); i++){
		for(int j=0; j<this->entry[i].size(); j++){
			if(entry[i][j] == 0){
				Position pos = make_pair(i,j);
				if(	(i==0 && j==0) || 
					(i==0 && j>0 && this->entry[i][j-1]>0) ||
					(i>0 && j==0 && this->entry[i-1][j]>0) ||
					(i>0 && j>0 && this->entry[i][j-1]>0 && this->entry[i-1][j]>0)
				){
					result.push_back(pos);
				}
			}
		}
	}
	return result;
}

int Tableau::max(void){
	int num = -(this->INFTY);
	for(int i=0; i<this->entry.size(); i++){
		for(int j=0; j<this->entry[i].size(); j++){
			if(num < this->entry[i][j]){
				num = this->entry[i][j];
			}
		}
	}
	return num;
}

Tableau::Tableau(vector<int> partition){
	for(int i=0; i<partition.size(); i++){
		vector<int> hoge(partition[i]);
		this->entry.push_back(hoge);
	}
}

Position Tableau::ordering(int n){
	int counter = 1;
	int max_length = 0;
	for(int i=0; i<this->entry.size(); i++){
		if(this->entry[i].size() > max_length){
			max_length = this->entry[i].size();
		}
	}
	for(int j=max_length-1; j>=0; j--){
		for(int i=(int)(this->entry.size())-1; i>=0; i--){
			if(this->entry[i].size() > j){
				if(counter == n){
					return make_pair(i,j);
				}
				counter++;
			}
		}
	}
	return make_pair(-1,-1);
}

void Tableau::swap(Position a, Position b){
	int kari = this->get_entry(a);
	this->set_entry(a, this->get_entry(b));
	this->set_entry(b, kari);
}

int Tableau::get_entry(Position pos){
	//壁の中を読むとINFTYを返すようにしてある
	if(pos.first >= this->entry.size() || pos.second >= this->entry[pos.first].size()){
		return this->INFTY;
	}
	return this->entry[pos.first][pos.second];
}
void Tableau::set_entry(Position pos, int var){
	this->entry[pos.first][pos.second] = var;
}

int Tableau::size(){
	int sm = 0;
	for(int i=0; i<this->entry.size(); i++){
		sm += this->entry[i].size();
	}
	return sm;
}

int Tableau::sum(){
	int sm = 0;
	for(int i=0; i<this->entry.size(); i++){
		for(int j=0; j<this->entry[i].size(); j++){
			sm += this->entry[i][j];
		}
	}
	return sm;
}

int Tableau::abssum(){
	int sm = 0;
	for(int i=0; i<this->entry.size(); i++){
		for(int j=0; j<this->entry[i].size(); j++){
			int x = this->entry[i][j];
			sm += (x>=0)?x:(-x);
		}
	}
	return sm;
}

Tableau::Tableau(vector< vector<int> > tab){
	this->entry = tab;
}

vector<int> Tableau::sh(){
	vector<int> shape(this->entry.size());
	for(int i=0; i<this->entry.size(); i++){
		shape[i] = this->entry[i].size();
	}
	return shape;
}

void Tableau::show(){
	for(int i=0; i<this->entry.size(); i++){
		for(int j=0; j<this->entry[i].size(); j++){
			printf("%3d", this->entry[i][j]);
		}
		printf("\n");
	}
	return;
}

Position Tableau::slide(Position initial_pos){
	Position pos = initial_pos;
	Position next_pos = initial_pos;
	do{
		pos = next_pos;
		next_pos = this->slide_step(pos);
	}while(next_pos != pos);
	return pos;
}

Position Tableau::slide_step(Position pos){
	Position right  = make_pair(pos.first, pos.second+1);
	Position down   = make_pair(pos.first+1, pos.second);
	int p = this->get_entry(pos);
	int r = this->get_entry(right);
	int d = this->get_entry(down);
	if(p < r && p < d){
		return pos;
	}
	if(p > r && p < d){
		this->swap(pos, right);
		return right;
	}
	if(p < r && p > d){
		this->swap(pos, down);
		return down;
	}
	if(p > r && p > d){
		if(d <= r){
			this->swap(pos, down);
			return down;
		}else{
			this->swap(pos, right);
			return right;
		}
	}
	return make_pair(-1,-1);
}

void NPS_step(int n, Tableau &Y, Tableau &H){
	Position oldpos = Y.ordering(n);
	Position newpos = Y.slide(oldpos);
	for(int i=oldpos.first; i<newpos.first; i++){
		H.set_entry(make_pair(i, oldpos.second), H.get_entry(make_pair(i+1, oldpos.second))-1);
	}
	H.set_entry(make_pair(newpos.first, oldpos.second), newpos.second - oldpos.second);
	return;
}

pair<Tableau, Tableau> NPS(Tableau tab, int step_visible){	
	int size = tab.size();
	Tableau T = tab;
	Tableau H(tab.sh());
	for(int i=1; i<=size; i++){
		NPS_step(i, T, H);
		if(step_visible){
			printf("---step %d-----\n", i);
			T.show();
			printf("\n");
			H.show();
		}
	}
	return make_pair(T, H);
}

void build_SYT(queue<Tableau> &building_SYT, vector<Tableau> &completed_building_SYT){
	Tableau tab = building_SYT.front();
	building_SYT.pop();
	vector<Position> insertable_positions = tab.insertable_positions();
	if(insertable_positions.empty()){
		completed_building_SYT.push_back(tab);
		return;
	}
	int max = tab.max();
	for(int i=0; i<insertable_positions.size(); i++){
		Tableau kari = tab;
		kari.set_entry(insertable_positions[i], max+1);
		building_SYT.push(kari);
	}
	return;
}

vector<Tableau> enumerate_SYT(vector<int> partition){
	queue<Tableau> building_SYT;
	vector<Tableau> completed_building_SYT;
	Tableau zero(partition);
	building_SYT.push(zero);
	while(building_SYT.size()){
		build_SYT(building_SYT, completed_building_SYT);
	}
	return completed_building_SYT;
}



/*
int main(){
	vector<int> lambda{3,3,1};
	vector<Tableau> syts = enumerate_SYT(lambda);
	cout << syts.size() << endl;
	for(int i=0; i<syts.size(); i++){
		syts[i].show();
		vector<int> hoge = syts[i].read_by_row();
		for(int i=0; i<hoge.size(); i++){
			printf("%d", hoge[i]);
		}
		printf("------\n");
	}
	return 0;
}

*/
