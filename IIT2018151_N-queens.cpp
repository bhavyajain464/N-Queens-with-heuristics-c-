#include<bits/stdc++.h>
using namespace std;

int N;

bool under_attack(int col,vector<int>queens){
	if(find(queens.begin(),queens.end(),col)!=queens.end()) return true;
	else{
		for (int i=0;i<queens.size();i++){
			if(abs(col-queens[i])==queens.size()-i) return true;
		}
	}
	return false;
}

bool is_under_attack(int row,int col,vector<int>queens){
	if(find(queens.begin(),queens.end(),col)!=queens.end()) return true;
	for(int i=0;i<queens.size();i++){
		if(abs(col-queens[i])==row-i) return true;
	}
	return false;
}

bool sq_under_attack(int x,int y,int qx,int qy){
	if(y==qy) return true;
	if(abs(y-qy)==x-qx) return true;
	return false;
}

bool revise(map<int,vector<int>>&queens_dict,int q1,int q2){
	bool revised = false;
	for(int i=0;i<queens_dict[q1].size();i++){
		bool satisfied = false;
		for(int j=0;j<queens_dict[q2].size();j++){
			if(!sq_under_attack(q2,queens_dict[q2][j],q1,queens_dict[q1][i])){
				satisfied = true;
			}
		}
		if(satisfied==false){
			queens_dict[q1].erase(queens_dict[q1].begin()+i);
			// queens_dict[q1].remove(queens_dict[q1][i]);
			revised = true;
		}
	}
	return revised;
}

bool ac3(map<int,vector<int>>queens_dict,vector<int>&queens){
	queue<pair<int,int>>arcs_queue;
	int q=queens.size()-1;
	for(auto it:queens_dict){
		if(it.first>q){
			for(auto it2:queens_dict){
				if(it2.first>q && it.first!=it2.first){
					arcs_queue.push(make_pair(it.first,it2.first));
				}
			}
		}
	}
	while(arcs_queue.size()>0){
		pair<int,int>arc = arcs_queue.front(); 
		arcs_queue.pop();
		int queen1 = arc.first;
		int queen2 = arc.second;
		if(revise(queens_dict,queen1,queen2)){
			if(queens_dict[queen1].size()==0){
				return false;
			}
			for(auto it:queens_dict){
				if(find(queens.begin(),queens.end(),it.first)==queens.end() && it.first>q){
					arcs_queue.push(make_pair(queen1,it.first));
				}
			}
		}
	}
	return true;
}



void update_domain(map<int,vector<int>>&queens_dict,vector<int>&queens){
	int queen = queens.size()-1;
	int col = queens[queens.size()-1];
	for(auto it:queens_dict){
		if(it.first!=queen){
			vector<int> to_del;
			for(int i=0;i<it.second.size();i++){
				if(sq_under_attack(it.first,it.second[i],queen,col)){
					to_del.push_back(it.second[i]);
				}
			}
			vector<int>temp;
			for(int i=0;i<it.second.size();i++){
				if(find(to_del.begin(),to_del.end(),it.second[i])==to_del.end()){
					temp.push_back(it.second[i]);
				}
			}
			queens_dict[it.first]=temp;
		}
	}
}

vector<int> order_by_lcv(vector<int>queens,int n){
	int new_restrictions = 0;
	vector<int>col_restrictions;
	map<int,int>col_rest;
	for(int i=0;i<N;i++){
		if(find(queens.begin(),queens.end(),i)==queens.end()){
			if(!(under_attack(i,queens))){
				int bottom_range = N-queens.size();
				int left_range = min(i-0,bottom_range)+1;
				int right_range = min(N-left_range,bottom_range)+1;
				for(int j=1;j<right_range;j++){
					bool is_restricted = is_under_attack(queens.size()+i,i+j,queens);
					if(!is_restricted){
						if(queens.size()+j<N && queens.size()+j>=0 && i+j<N && i+j>=0){
							new_restrictions++;
						}
					}
				}
				for(int j=1;j<left_range;j++){
					bool is_restricted = is_under_attack(queens.size()+i,i-j,queens);
					if(!is_restricted){
						if(queens.size()+j<N && queens.size()+j>=0 && i-j<N && i-j>=0){
							new_restrictions++;
						}
					}
				}
				for(int j=1;j<bottom_range;j++){
					bool is_restricted = is_under_attack(queens.size()+i,i,queens);
					if(!is_restricted){
						if(queens.size()+j<N && queens.size()+j>=0 && i<N && i>=0){
							new_restrictions++;
						}
					}
				}
				col_rest[i] = new_restrictions;
				new_restrictions=0;

			}
		}
	}
	set<pair<int,int>>s;
	for(auto it:col_rest){
		s.insert(make_pair(it.second,it.first));
	}
	std::vector<int> x;
	for(auto it:s){
		x.push_back(it.second);
	}
	return x;

}

vector<int> rsolve(map<int,vector<int>>m,vector<int>v,int n){
	if(n==v.size()){
		return v;
	}
	else{
		int queen = v.size();
		m[queen] = order_by_lcv(v,N);
		map<int,vector<int>>local_queens = m;
		for(int i=0;i<m[queen].size();i++){
			vector<int>temp=v;
			temp.push_back(m[queen][i]);
			update_domain(m,temp);
			bool inference = ac3(m,temp);

			if(inference){
				vector<int>newqueens = rsolve(m,temp,N);
				if(newqueens.size()!=0){
					return newqueens;
				}
			}
			m = local_queens;
		}
		vector<int> fail;
		return fail;
	}
}

int main(){

	clock_t start;
	start = clock();
	int n=20;
	N=n;
	map<int,vector<int>>m;
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			m[i].push_back(j);
		}
	}
	vector<int>v;
	vector<int>ans = rsolve(m,v,n);

	for(int i=0;i<ans.size();i++){
		for(int j=0;j<ans[i];j++){
			cout<<". ";
		}
		cout<<"Q ";
		for(int j=0;j<n-1-ans[i];j++){
			cout<<". ";
		}
		cout<<endl;
	}
	start = clock()-start;
	cout<<"Time Taken:"<<(double)start/CLOCKS_PER_SEC<<" Seconds"<<endl;
}