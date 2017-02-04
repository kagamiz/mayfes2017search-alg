#include <stdio.h>
//構造体の定義
typedef struct dlobj{
struct dlobj *next;
struct dlobj *prev;
int key;
int weight;
} dlobj;

typedef struct {
dlobj *nil;
} dlist;

typedef struct {
int n; // 節点数
int m; // 枝数
dlist **E; // 枝リストの配列
} graph;

//プロトタイプ宣言

dlist *dlist_new();
graph *graph_new(int n,int m);
graph *graph_input();
dlobj *dlobj_new(int k,int w);
void dlist_append(dlist *L, dlobj *r);
dlobj *graph_firstedge(graph *G, int i);
dlobj *dlist_search(dlist *L, int k);
dlobj *graph_nextedge(graph *G, int i, dlobj *e);
