#ifndef STACK_H
#define STACK_H

int * Stack;
int st_size;
int filled;
void make_stack(int sz){
	st_size = sz;
	Stack = new int[sz];
	filled = 0;
}
bool isEmpty(){
	return (filled == 0)?true:false;
}
void st_push(int a){
	Stack[filled] = a;
	filled++;
}
int st_pop(){
	int l = -1;
	if (!isEmpty()){
		l = Stack[filled-1];
		filled--;
	}
	return l;
}
#endif