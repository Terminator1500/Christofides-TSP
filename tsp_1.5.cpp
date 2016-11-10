//tsp mst 1.5
#include <iostream>
#define INF 9999999
#define N 4

using namespace std;

#include "Edge.cpp"
#include "stack.cpp"
#include "odd_deg.cpp"

#ifndef EULER_H
#include "euler.cpp"
/* MST calculation */
bool in_mst[N];
int key[N];
int * parent;
void initialize(){
	for (int i = 0; i < N; i++){
		in_mst[i] = false;
		key[i] = INF;
		parent[i] = 0;
	}
}

//Prim's Method for MST
int * mst(int graph[N][N]){
	int in = 0;
	int min = INF;
	int min_id = 0;
	parent = new int[N]; // Gives the final MST with pointers to parents for each vertex
	initialize();
	while (in != N){
		if (in == 0){
			key[0] = 0;
		}
		else{
			for (int uni = 0; uni < N; uni++){
				if (!in_mst[uni] && graph[min_id][uni] < key[uni]){
					key[uni] = graph[min_id][uni];
					parent[uni] = min_id;
				}
			}
		}

		min = INF;
		min_id = 0;
		for (int i = 0; i < N; i++){
			if (!in_mst[i] && key[i] < min){
				min_id = i;
				min = key[i];
			}
		}

		in_mst[min_id] = true;
		in++;
	}
	return parent;
}

int * get_path_frm_euler(Edge * eulerc, int V, int odd, int start_node){
	bool visited[V];
	for (int i = 0; i < V; i++) {visited[i] = false;}
	
	int * path = new int[V + 1];
	int it = 0, path_c = 1, start = start_node;
	path[0] = start;

	Edge curr_eg = eulerc[it];
	while (curr_eg._start != start_node && it < all_ecount){
		curr_eg = eulerc[it];
		it = (it + 1) % all_ecount;
	}
	while (curr_eg._end != start_node && it < all_ecount){		
		if (!visited[curr_eg._end]){
			visited[curr_eg._end] = true;
			path[path_c] = curr_eg._end;
			path_c++;
		}
		it = (it + 1) % all_ecount;
		curr_eg = eulerc[it];
	}
	path[path_c] = curr_eg._end;

	return path;
}

/***********************/

int main(){
	int graph[N][N] = { {0, 1, 2, 1},
						{1, 0, 1, 2},
						{2, 1, 0, 1},
						{1, 2, 1, 0}};
	
	int i, st_node = 1;
	int * parents = mst(graph);
	int * oddd = odd_deg(parents, N);
	int ** PM = MWPM(graph, oddd);
	//cout << odd_nodes << endl;
	Edge * euler_c = get_euler_cir(graph, parents, PM, N, num_odd_nodes(), st_node);
	int * path = get_path_frm_euler(euler_c, N, num_odd_nodes(), st_node);

	//Print Path
	cout << "TSP Path : ";
	for (i = 0; i < N; i++){
		cout << path[i] << " - ";
	}
	cout << path[i] << endl;

	return 0;
}
#endif
