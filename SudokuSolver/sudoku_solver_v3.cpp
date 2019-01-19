#include <bits/stdc++.h>
#include <chrono>
#define fi first
#define sec second
#define pii pair<int,int>
using namespace std;

int data[9][9];
int data_reset[9][9];
int memo1[10][9];
int memo2[10][9];
int memo3[10][9];



int getData(int i, int j, bool reset_data = false){
    if (reset_data) return data_reset[i][j];
    return data[i][j];
}

void setData(int i, int j, int value, bool ps_dr){
    data[i][j] = value;
    if(ps_dr) data_reset[i][j] = value;
}

void clearData(){
    memset(data,0,data[0][0]*9*9);
}


//attribute
void input(){
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
			cin>>data[i][j];
			data_reset[i][j] = data[i][j];
		}
	}
}

void print(int data_base[9][9] = data){
    for(int i=0; i<9; i++){
		cout<<"s"<<i<<" ";
        for(int j=0; j<9; j++){
			cout<<data_base[i][j]<<" ";
		}
		cout<<endl;
	}
}

void print_t_f(bool result){
	cout<<"c ";
	if(result) cout<<"TRUE";
	else cout<<"FALSE";
	cout<<endl;
}

bool isit_finish(int data_base[9][9] = data){
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
			if (data_base[i][j] == 0) return false;
		}
	}
	return true;
}

bool re_check(int i, int j, bool un_memo, int (&data_base)[9][9] = data){
	int a = (i/3)*3 + j/3;
	if(memo1[data_base[i][j]][i] == 0 && memo2[data_base[i][j]][j] == 0 && memo3[data_base[i][j]][a] == 0){
		if (un_memo){
			memo1[data_base[i][j]][i] = 1;
			memo2[data_base[i][j]][j] = 1;
			memo3[data_base[i][j]][a] = 1;
		}
		return true;
	}else return false;
}

bool check(bool printed = false, int (&data_base)[9][9] = data){
	bool cond = true;
	memset(memo1, 0, sizeof(memo1[0][0])*10*9);
	memset(memo2, 0, sizeof(memo2[0][0])*10*9);
	memset(memo3, 0, sizeof(memo3[0][0])*10*9);
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
			if (data_base[i][j] == 0){
			}else {
				if (!re_check(i,j,true, data_base)){
					cond = false;
					if (printed) cout<<"w "<<i<<" "<<j<<endl;
				}
				
			}
		}
	}
	return cond;
}

bool check_data_reset(){
    return check(false, data_reset);
}

void reset_sudoku(){
    copy(&data_reset[0][0], &data_reset[0][0]+9*9,&data[0][0]);
}



//for backtracking

bool rightAns(int row, int col, int ans, int data_base[9][9] = data){
	if (data_base[row][col] != 0)return false;
    for(int i = 0; i<9; i++){
		if (data_base[row][i] == ans)return false;
	}
    for(int i = 0; i<9; i++){
		if (data_base[i][col] == ans)return false;
	}
	int x = row % 3;
	int y = col % 3;
	for(int i = 0; i<3; i++){
		for(int j = 0; j<3; j++){
			if(data_base[row + i - x][col + j - y] == ans) return false;
		}
	}
	
	return true;
}


void solve_exact_cover_dancinglinks(){

}

list<int> d_p[9][9];

void eliminated_d_p(list<int> &l_pn,int dt_rm){
	l_pn.erase(remove(begin(l_pn),end(l_pn),dt_rm),end(l_pn));
}

void eliminate_number(list<int> &l_pn, queue<pii> &data_left_1, list<pii> &data_left_2, int dt_rm, int dt_num, pii loc){
	if(dt_num == 0){
		eliminated_d_p(l_pn,dt_rm);
		if (l_pn.size() == 1) data_left_1.push(loc);
		else if(l_pn.size() == 2) data_left_2.push_back(loc);
	}
}

void eliminate_surrounding(int f, int g, int (&data_base)[9][9]){
    for (int a=0;a<9;a++){
		if (data_base[f][a] == 0) eliminated_d_p(d_p[f][a], data_base[f][g]);
		if (data_base[a][g] == 0) eliminated_d_p(d_p[a][g], data_base[f][g]);
	}
	int l = f - f%3;
	int m = g - g%3;
	for (int x=l;x<l+3;x++){
		for(int y=m;y<m+3;y++){
			if (data_base[x][y] == 0) eliminated_d_p(d_p[x][y],data_base[f][g]);
		}
	}
}

bool all_possible(int (&data_base)[9][9] = data){
	if (check(false,data_base)){
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
				if (data_base[i][j] == 0){
					d_p[i][j].clear();
                    for (int k=1;k<=9;k++){
						data_base[i][j] = k;
						if (re_check(i,j,false,data_base)) d_p[i][j].push_back(k);
						data_base[i][j] = 0;
					}
				}
			}
		}
		return true;
	}else return false;
}

void reduce_puzzel(int (&data_base)[9][9] = data){
	queue<pair<pii,pii>> naked_twins;
	queue<pii> data_left_1;
	list<pii> data_left_2;
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
			int f = i;
			int g = j;
			//solving naked twins problem
			if (data_base[f][g] == 0 && d_p[f][g].size() == 2){
				list<pii>::iterator ptr;
				bool nt = false;
				pii data_pii;
				for(ptr = data_left_2.begin(); ptr != data_left_2.end(); ptr++){
					data_pii = *ptr;
					if (!(data_pii.fi == f && data_pii.sec == g)){
						if (data_pii.fi == f || data_pii.sec == g){
							if(d_p[data_pii.fi][data_pii.sec] == d_p[f][g]){
								naked_twins.push({{data_pii.fi,data_pii.sec},{f,g}});
								nt = true;
								break;
							}
						}
					}
				}
				if (!nt) data_left_2.push_back({f,g});
				else data_left_2.remove(data_pii);
			}
			while(naked_twins.size() > 0){
				if (naked_twins.front().fi.fi == naked_twins.front().sec.fi){
                    for (int a=0;a<9;a++){
						if (!(a == naked_twins.front().fi.sec || a == naked_twins.front().sec.sec)){
							eliminate_number(d_p[naked_twins.front().fi.fi][a], data_left_1,data_left_2, d_p[naked_twins.front().fi.fi][naked_twins.front().fi.sec].front(),data_base[naked_twins.front().fi.fi][a] , {naked_twins.front().fi.fi,a});
							eliminate_number(d_p[naked_twins.front().fi.fi][a], data_left_1,data_left_2, d_p[naked_twins.front().fi.fi][naked_twins.front().fi.sec].back(),data_base[naked_twins.front().fi.fi][a] , {naked_twins.front().fi.fi,a});
						}
					}
				}else{
                    for (int a=0;a<9;a++){
						if (!(a == naked_twins.front().fi.fi || a == naked_twins.front().sec.fi)){
							eliminate_number(d_p[a][naked_twins.front().fi.sec], data_left_1,data_left_2, d_p[naked_twins.front().fi.fi][naked_twins.front().fi.sec].front(),data_base[a][naked_twins.front().fi.sec] , {a,naked_twins.front().fi.sec});
							eliminate_number(d_p[a][naked_twins.front().fi.sec], data_left_1,data_left_2, d_p[naked_twins.front().fi.fi][naked_twins.front().fi.sec].back(),data_base[a][naked_twins.front().fi.sec] , {a,naked_twins.front().fi.sec});
						}
					}
				}
				naked_twins.pop();
			}
			while(true){
				// if data size is only one
				if (data_base[f][g] == 0 &&  d_p[f][g].size() == 1){
					data_base[f][g] = d_p[f][g].front();
                    for (int a=0;a<9;a++){
						eliminate_number(d_p[f][a], data_left_1, data_left_2, data_base[f][g], data_base[f][a], {f,a});
						eliminate_number(d_p[a][g], data_left_1, data_left_2, data_base[f][g], data_base[a][g], {a,g});
					}
					int l = f - f%3;
					int m = g - g%3;
					for (int x=l;x<l+3;x++){
						for(int y=m;y<m+3;y++){
							eliminate_number(d_p[x][y], data_left_1, data_left_2, data_base[f][g], data_base[x][y], {x,y});
						}
					}
				}
				if (data_left_1.size() > 0){
					f = data_left_1.front().fi;
					g = data_left_1.front().sec;
					data_left_1.pop();
				}else break;
			}
		}
	}
}

bool solve_math(int (&data_base)[9][9] = data){
	if (all_possible(data)){
		reduce_puzzel(data);
		return true;
	}else return false;
};

bool findBlank(int &row, int &col, int data_base[9][9] = data){
    for(row = 0; row < 9; row++){
        for (col = 0; col< 9;col++){
			if(data_base[row][col] == 0) return true;
		}
	}
	return false;
}

bool sudoku_solved = false;
bool SudokuBacktrack(){
	if (!sudoku_solved){
		int row, col;
		if(!findBlank(row, col)){
			sudoku_solved = true;
			return true;
		}
        for(int ans = 1; ans<=9; ans++){
			data[row][col]= ans;
			if (re_check(row,col,false)){
				re_check(row,col,true);
				if(SudokuBacktrack()) return true;
				data[row][col] = 0;
				check();
			}else{
				data[row][col] = 0;
			}
		}
	}
	return false;
}

void solve_backtracking(){
	check();
	if(!SudokuBacktrack()){	
		cout << "e The problem has no answer!" << endl;	
	}
}

bool solution_found = false;
bool solve_back_plus(int (&data_base)[9][9]){
	if (!solution_found){
		int row, col;
		int datum[9][9];
		copy(&data_base[0][0], &data_base[0][0]+9*9, &datum[0][0]);
		reduce_puzzel(datum);
		if (!findBlank(row,col,datum)) {
			copy(&datum[0][0], &datum[0][0]+9*9,&data[0][0]);
			solution_found = true;
			return true;
		}
		list<int> listData(d_p[row][col]);
		list<int>::iterator ptr;
		for (ptr = listData.begin(); ptr != listData.end(); ++ptr){
			if (*ptr > 9) continue;
			datum[row][col] = *ptr;
			eliminate_surrounding(row,col, datum);
			if(solve_back_plus(datum)) return true;
			datum[row][col] = 0;
			if(!all_possible(datum)) return false;
		}
	}
	return false;	
}

void solve_backtracking_plus(){
	solution_found = false;
	if(!all_possible(data)){
		cout << "e The problem is wrong!" << endl;	
	}else{
		if(!solve_back_plus(data)){	
			cout << "e The problem has no answer!" << endl;	
		}
	}
}

void create_hole(int amt_of_holes){
    int i = 0;
    int j = rand()%3;
    while(i<9 && j<9){
        if(data[i][j] != 0) amt_of_holes -= 1;
        data[i][j] = 0;
        if(amt_of_holes == 0) break;
        j += rand()%9;
        if(j>=9){
            i++;
            j = 0;
            j += rand()%9;
        }
    }
    if (amt_of_holes > 0) create_hole(amt_of_holes);
}

vector<int> num;
bool GeneratorBacktrack(){
	int row, col;
	if(!findBlank(row, col)) return true;
	for(int ans = 0; ans<9; ans++){
		if(ans==0){
			num = {1, 2, 3, 4, 5, 6, 7, 8, 9};
			random_shuffle(num.begin(), num.end());
		}
		if(rightAns(row, col, num[ans])){
			data[row][col]= num[ans];
			if(GeneratorBacktrack()) return true;
			data[row][col] = 0;
		}
	}
	return false;
}


void generate_sudoku(int lvl){
	srand(time(0));
	memset(data,0,data[0][0]*9*9);
	if(GeneratorBacktrack()){
		create_hole(30+(3.3)*lvl);
	}else{
		generate_sudoku(lvl);
	}
	cout<<endl;
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
			data_reset[i][j] = data[i][j];
		}
	}
}

int main(){
	int condition = 0;
	int lvl_gen;
	int cond = true;
	while(cond){
		cin>>condition;
		auto start = chrono::high_resolution_clock::now();
		try{
			switch(condition){
				case 0:
					cond = false;
					break;
				case 1:
					//for input the data
					input();
					break;
				case 2:
					//for check the current solution is it true
					print_t_f(check(true));
					break;
				case 3:
					print_t_f(check(false));
					break;
				case 4:
					//solve using math, but the solution only can be 1
					solve_math();
					break;
				case 5:
					solve_backtracking();
					break;
				case 6:
					solve_backtracking_plus();
					break;
				case 7:
					solve_exact_cover_dancinglinks();
					break;
				case 8:
					cin>>lvl_gen;
					generate_sudoku(lvl_gen);
					break;
				case -1:
					print();
					break;
				case -2:
					print_t_f(isit_finish());
					break;
				case -3:
					copy(&data_reset[0][0], &data_reset[0][0]+9*9,&data[0][0]);
					break;
			}
		}catch(int e){
			cout<<"ERROR : "<<e<<endl;
		}
		auto finish = chrono::high_resolution_clock::now();
		cout <<"t "<< chrono::duration_cast<chrono::nanoseconds>(finish-start).count()/1000 << " ns"<<endl;
	}
	return 0;
}
