#include "Edge.cpp"
#ifndef N
#define N 4
#endif

#include <algorithm>
int * _parents;
int odd_nodes = 0;

//Find odd degree vertices in MST
int * odd_deg(int * parents, int NUM){
	_parents = parents;
	int * _odd_deg_arr = new int[NUM];
	for (int it = 0; it < NUM; it++)
		_odd_deg_arr[it] = 1;

	for (int vert = 0; vert < NUM; vert++){
		_odd_deg_arr[parents[vert]] = (_odd_deg_arr[parents[vert]] + 1) % 2;
	}
	odd_nodes = 0;
	for (int i = 0; i < NUM; i++){if(_odd_deg_arr[i] == 1) odd_nodes++;}
	return _odd_deg_arr;
}

int num_odd_nodes(){
	if (odd_nodes % 2 != 0){
		cout << "ODD number of odd degree nodes, Check the input" << endl;
		exit(0);
	}
	return odd_nodes;
}

//Greedy make minimum weight perfect matching
int ** MWPM(int graph[][N], int * odd_deg_arr){
	//Count the number of odd degree vertices in mst
	int _num_odd = odd_nodes;

	//List of all odd degree vertices
	int odd_list[_num_odd], l = 0;
	for (int it = 0; it < N; it++){
		if (odd_deg_arr[it] == 1){
			odd_list[l] = it;
			l++;		
		}
	}

	//Edge list ... will be sorted later
	int len = _num_odd*(_num_odd - 1), list_ct = 0;
	Edge list[_num_odd*(_num_odd - 1)];
	for (int i = 0; i < _num_odd; i++){
		for (int j = 0; j < _num_odd; j++){
			if (j != i){
				list[list_ct]._start = odd_list[i];
				list[list_ct]._end = odd_list[j];
				list[list_ct]._val = graph[i][j];
				list_ct++;
			}
		}
	}

	//Sort Edge list (N2)
	Edge temp[_num_odd*(_num_odd - 1)];
	int min = 9999999; int min_id = 0;
	bool marked[len];
	for (int i = 0; i < len; i++){
		min = 9999999; min_id = i;
		for (int j = 0; j < len; j++){
			if (!marked[j] && list[j]._val < min){
				min_id = j;
				min = list[j]._val;
			}
		}
		temp[i] = list[min_id];
		//cout << i << " : " << temp[i]._start << ", " << temp[i]._end << endl;
		marked[min_id] = true;
	}
	// Or
	// std::sort(list, list + _num_odd*(_num_odd - 1), [](Edge const & a, Edge const & b)-> bool{ return a._val < b._val; });

	int ** _pM = new int*[_num_odd/2];
	int count = 0, iter = 0;
	bool pick_mark[_num_odd];
	//for (int it = 0; it < _num_odd; it++){		
	while (count < _num_odd/2 && iter < len){
		if (!pick_mark[temp[iter]._start] && !pick_mark[temp[iter]._end]){
			_pM[count] = new int[2];
			_pM[count][0] = temp[iter]._start;
			_pM[count][1] = temp[iter]._end;
			pick_mark[temp[iter]._start] = true;
			pick_mark[temp[iter]._end] = true;
			count++;
		}
		iter++;
	}

	return _pM;
}
