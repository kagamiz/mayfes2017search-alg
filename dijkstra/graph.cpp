#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "graph.hpp"
dlist *dlist_new()//空のリストを作成
{
 dlist *L;
 L=(dlist*)malloc(sizeof(dlist));
 L->nil=(dlobj*)malloc(sizeof(dlobj));
 L->nil->next=L->nil;
 L->nil->prev=L->nil;
 return L;
}

graph *graph_new(int n,int m)//新しいグラフを作る
{
 int i;
 graph *G;
 G=(graph*)malloc(sizeof(graph));
 G->n=n;
 G->m=m;
 G->E=(dlist**)malloc(n*sizeof(dlist*));
 for(i=0;i<n;i++)
  {
   G->E[i]=dlist_new();
  }
 return G;
}

void dlist_append(dlist *L, dlobj *r)//リストの最後に要素ｒを追加
{
 r->prev=L->nil->prev;
 L->nil->prev->next=r;
 L->nil->prev=r;
 r->next=L->nil;
}


dlobj *dlobj_new(int k,int w)//キーがｋである要素を作成
{
 dlobj *x;
 x=(dlobj*)malloc(sizeof(dlobj));
 x->key=k;
 x->weight=w;
 return x;
}


graph *graph_input()//グラフを標準入力から読み込む
{
 graph *G;
 int i,n,m;
 int v,u,t;
 scanf("%d %d",&n,&m);
 G=graph_new(n,m);
 for(i=0;i<G->m;i++)
  {
    scanf("%d %d %d",&v,&u,&t);
    dlist_append(G->E[v-1],dlobj_new(u,t));
  }
 return G;
}



dlobj *graph_firstedge(graph *G, int i)
{
 if(G->E[i-1]->nil->next!=G->E[i-1]->nil)
 {
  dlobj *x;
  x=G->E[i-1]->nil->next;
  return x;
 }
 else return NULL;
}

dlobj *dlist_search(dlist *L, int k)//リストにあるキーがkの要素のポインタを返す
{
 dlobj *x = L->nil->next;
 while (x != L->nil)
 {
  if (x->key == k) return x;
  x = x->next;
 }
 return NULL;
}


dlobj *graph_nextedge(graph *G, int i, dlobj *e)//点 i から出ている枝で e の次のものを返す
{
 if(e->next!=G->E[i-1]->nil)
 {
  dlobj *x;
  x=dlist_search(G->E[i-1],e->key)->next;
  return x;
 }
 else return NULL;
}
