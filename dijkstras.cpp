//Grant Showalter (gds2wz)
//prelab11 topological.cpp


#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <stdlib.h>
using namespace std;


class vertex{
public:
    vector<vertex*> adj; //adj list
    int minCost;
    int name;
    int bestNode;

    vertex(){
       minCost = -1;
       bestNode = 0;
    }

    vertex(int id){
        name = id;
        minCost = -1;
        bestNode = 0;
    }

private:
};

//vector<vertex*> globalGraph;
vector<vertex*> map;
int map2[15][15] = 
{
	{0,135,0,0,0,0,0,0,0,0,0,0,0,0,123},
	{135, 0,106,0,0,0,0,0,0,0,0,0,0,0,156},
	{0,106,0,88,0,0,0,113,0,0,0,0,0,0,0},
	{0,0,88,0,59,0,64,0,0,0,0,0,0,0,0},
	{0,0,0,59,0,68,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,68,0,68,0,0,0,0,0,0,0,0},
	{0,0,0,64,0,68,0,0,0,118,0,0,0,0,0},
	{0,0,113,0,0,0,0,0,59,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,59,0,120,0,0,0,97,0},
	{0,0,0,0,0,0,118,0,120,0,72,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,72,0,105,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,105,0,110,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,110,0,105,0},
	{0,0,0,0,0,0,0,0,97,0,0,0,105,0,102},
	{123,156,0,0,0,0,0,0,0,0,0,0,0,102,0}
};

int main (int argc, char **argv) {
	vertex *A,*B,*C,*D,*E,*F,*G,*H,*I,*J,*K,*L,*M,*N,*O;
	A->name = 1;
	B->name = 2;
	C->name = 3;
	D->name = 4;
	E->name = 5;
	F->name = 6;
	G->name = 7;
	H->name = 8;
	I->name = 9;
	J->name = 10;
	K->name = 11;
	L->name = 12;
	M->name = 13;
	N->name = 14;
	O->name = 15;
	vertex* arr[] = {A,B,C,D,E,F,G,H,I,J,K,L,M,N,O};
	map.insert(map.end(),arr,arr+15);
	A->adj.push_back(B);
	A->adj.push_back(O);
	B->adj.push_back(A);
	B->adj.push_back(C);
	B->adj.push_back(O);
	C->adj.push_back(B);
	C->adj.push_back(D);
	C->adj.push_back(H);
	D->adj.push_back(C);
	D->adj.push_back(E);
	D->adj.push_back(G);
	E->adj.push_back(D);
	E->adj.push_back(F);
	F->adj.push_back(E);
	F->adj.push_back(G);
	H->adj.push_back(C);
	H->adj.push_back(I);
	I->adj.push_back(H);
	I->adj.push_back(N);
	I->adj.push_back(J);
	J->adj.push_back(G);
	J->adj.push_back(I);
	J->adj.push_back(K);
	K->adj.push_back(J);
	K->adj.push_back(L);
	L->adj.push_back(K);
	L->adj.push_back(M);
	M->adj.push_back(L);
	M->adj.push_back(N);
	N->adj.push_back(M);
	N->adj.push_back(I);
	N->adj.push_back(O);
	O->adj.push_back(A);
	O->adj.push_back(B);
	O->adj.push_back(N);
	
	
	
	
	


    return 0;
}
