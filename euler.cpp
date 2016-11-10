//euler.cpp
#ifndef STACK_H
#define EULER_H
#include "stack.cpp"
#endif
#ifndef EDGE_H
#include "Edge.cpp"
#endif

#define MAX_INF 99999999

int ** mod_mst; //Modified MST after adding perfect matches
int all_ecount; // number of edges in MST

//O(V^2)
int count_reachable(int total, int start_node){
	make_stack(total);
	int curr = start_node;
	st_push(curr);
	int count = 0;
	bool marked[total]; for (int i = 0; i < total; i++){marked[i] = false;}
	while (!isEmpty() && count < total){
		curr = st_pop();
		marked[curr] = true;
		count++;
		for (int i = 0; i < total; i++){
			if (curr != i && mod_mst[curr][i] < MAX_INF && !marked[curr]){
				st_push(i);
			}
		}
	}
	return count;
}

// O(2*N^2)
bool check_edge(Edge ed, int total_vert){
	int estart = ed._start;
	int eend = ed._end;
	int eval = ed._val;
	
	int prev_count = count_reachable(total_vert, estart);
	mod_mst[estart][eend] = MAX_INF; mod_mst[eend][estart] = MAX_INF;
	int new_count = count_reachable(total_vert, estart);
	mod_mst[estart][eend] = eval; mod_mst[eend][estart] = eval;
	
	if(prev_count == new_count) return true;
	else return false;
}

// O(V^2 + V + V/2 + 2V(V + V^2)) = O(V^3)
Edge* get_euler_cir(int graph[][N], int * mst, int ** PM, int NUM, int odd, int start_node){
	Edge * _euler = new Edge[NUM + odd/2]; 

	/************************  Get the new MST after PM additions  *****************************/
	mod_mst = new int*[NUM];
	for (int i = 0; i < NUM; i++){
		mod_mst[i] = new int[NUM];
		for (int j = 0; j < NUM; j++){
			mod_mst[i][j] = MAX_INF;
		}
	}

	Edge * all_edges = new Edge[NUM + odd/2 + 1];
	int eit = 1;
	all_ecount = 0; // Track Number of edges in new MST
	while (eit < NUM){
		all_edges[all_ecount]._start = mst[eit];
		all_edges[all_ecount]._end = eit;
		all_edges[all_ecount]._val = graph[mst[eit]][eit];
		all_ecount++;
		mod_mst[mst[eit]][eit] = graph[mst[eit]][eit];
		mod_mst[eit][mst[eit]] = graph[mst[eit]][eit];

		eit++;
	}

	while (eit < NUM + odd/2){
		if (mod_mst[PM[eit - NUM][0]][PM[eit - NUM][1]] == MAX_INF){
			all_edges[all_ecount]._start = PM[eit - NUM][0];
			all_edges[all_ecount]._end = PM[eit - NUM][1];
			all_edges[all_ecount]._val = graph[PM[eit - NUM][0]][PM[eit - NUM][1]];
			all_ecount++;
			mod_mst[PM[eit - NUM][0]][PM[eit - NUM][1]] = graph[PM[eit - NUM][0]][PM[eit - NUM][1]];
			mod_mst[PM[eit - NUM][1]][PM[eit - NUM][0]] = graph[PM[eit - NUM][0]][PM[eit - NUM][1]];
		}
		eit++;
	}
	all_edges[all_ecount]._start = -1; all_edges[all_ecount]._end = -1; all_edges[all_ecount]._val = -1;

	/****************************************************************************/
	/************************************** Print Modified MST **************************************/
	/*cout << "mod mst" << endl;
	for (int x = 0; x < NUM; x++){
		for (int y = 0; y < NUM; y++){
			cout << mod_mst[x][y] << ", ";
		}
		cout << endl;
	}*/

	/*************************************** Euler Circuit ****************************************/
	// 1. Add (u-v) if its the only connecting edge to u && 2. Add (u-v) if it's nt a bridge
	Edge current_edge; //current_edge._start = mst[1]; current_edge._end = 1; current_edge._val = _ori_graph[mst[1]][1];
	int current_vert = start_node;
	int eg_it = 0, edges, eg_count = 0;
	while (eg_count < all_ecount){
		while (eg_it < NUM){
			if (mod_mst[current_vert][eg_it] < MAX_INF){
				current_edge._start = current_vert; current_edge._end = eg_it; current_edge._val = mod_mst[current_vert][eg_it];
				eg_it++;
				break;
			}
			eg_it++;
		}
		//check if its the only edge
		edges = 0;
		for (int i = 0; i < NUM; i++){
			if (mod_mst[current_vert][i] != MAX_INF) edges++;
			if (edges > 1) break;
		}
		if (edges == 1){
			_euler[eg_count] = current_edge;// cout << "> " << current_edge._start << " " << current_edge._end << endl;
			//remove edge
			mod_mst[current_edge._start][current_edge._end] = MAX_INF; mod_mst[current_edge._end][current_edge._start] = MAX_INF;
			eg_count++;

			current_vert = current_edge._end;
			eg_it = 0;
		}
		else{
			if (check_edge(current_edge, NUM)){
				mod_mst[current_edge._start][current_edge._end] = MAX_INF;
				mod_mst[current_edge._end][current_edge._start] = MAX_INF;
				_euler[eg_count] = current_edge;// cout << ">2 " << current_edge._start << " " << current_edge._end << endl;
				eg_count++;

				current_vert = current_edge._end;
				eg_it = 0;
			}
		}
	}
	/***********************************************************************************************/
	return _euler;
}
