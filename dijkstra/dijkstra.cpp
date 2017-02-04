#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "graph.hpp"
#include "heap.hpp"

#define INFINITY 999999999



void Dijkstra(graph *G, int s, int *dist)
{
  int n, m, i, j;
  heap *H;
  heapdata *A, v, w;
  dlobj *edge;
  n = G->n;
  m = G->m;
  for (i=1; i<=n; i++) dist[i] = INFINITY; // s からの距離を無限大にする
  dist[s] = 0; // s までの距離は 0
  A = (heapdata*)malloc((m+1)*sizeof(heapdata*)); // ヒープに m 個格納するので,m+1 個分の領域を確保
  H = heapify_build(0, A, m); // 最大要素数 m, 現在の要素数 0 でヒープを構築
  v.value = s;
  v.priority = 0;
  heap_insert(H, v); // (s, 0) をヒープに入れる


  while (H->size > 0)
  {
   v = extract_max(H); // d(v) が最小である (v, d(v)) を取り出す
   if (dist[v.value] < v.priority) continue; // すでに, より短いパスが見つかっているときは何もしない
   edge = graph_firstedge(G, v.value); // v から出ている最初の枝
   while (edge != NULL)
    { // v から出ている各枝 (v, j) に対し処理を行う
     j = edge->key; // edge->v には現在注目している枝の端点の番号 j が入っている
     if (dist[j]>dist[v.value]+edge->weight)
      {
       dist[j]=dist[v.value]+edge->weight;
       w.value = j;
       w.priority = dist[j];
       heap_insert(H, w); // w = (j, dist[j]) をヒープに入れる
      }
     edge = graph_nextedge(G, v.value, edge); // v から出ている次の枝
    }
  }
  free(A);
}


int main()
{
  int i;
  int *dist;
  graph *G=graph_input();
  int n=G->n;
  dist=(int*)malloc((n+1)*sizeof(int));


  printf("%d\n",n);
  for(i=1;i<=n;i++)
  {
   Dijkstra(G,i,dist);
   for (size_t j = 1; j <= n; j++) {
       printf("%d ",dist[j]);
   }
   printf("\n");
  }

 return 0;
}
