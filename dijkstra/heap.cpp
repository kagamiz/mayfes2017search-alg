#include <stdio.h>
#include <stdlib.h>
#include "heap.hpp"

#define PARENT(i) (i/2)
#define GREATER(x,y) ((x).priority < (y).priority) // 最小値を求めるので不等号が逆になっている

void heap_swap(heap *H,int p,int q)
{
 heapdata tmp;
 tmp =H->A[p];
 H->A[p]=H->A[q];
 H->A[q]=tmp;
}

void heapify(heap *H, int i)
{
 int l, r, smallest, size;
 heapdata *A;
 A = H->A;
 size = H->size;
 l =2*i ;
 r =2*i+1 ;
 if (l <= size && A[l].priority<A[i].priority) smallest = l;

 else smallest= i;

 if (r <= size && A[r].priority<A[smallest].priority) smallest = r;

 if (smallest != i)
  {
   heap_swap(H, i, smallest);
   heapify(H, smallest);
  }
}


heap *heapify_build(int size, heapdata *A, int max_size)
{
 int i;
 heap *H;
 H=(heap*)malloc(sizeof(heap));
 H->max_size = max_size;
 H->size = size;
 H->A = A;


 for (i = size/2; i >= 1; i--)
  {
   heapify(H,i);
  }

 return H;
}

heapdata extract_max(heap *H)
{
 heapdata p=H->A[1];
 heap_swap(H,1,H->size);
 H->size=H->size-1;
 heapify(H,1);
 return p;
}

void heap_insert(heap *H, heapdata x)
{
 heapdata *A;
 int i;
 A = H->A;
 H->size = H->size + 1;
 if (H->size > H->max_size)
  {
   printf("ERROR ヒープのオーバーフロー¥n");
   exit(1);
  }
 i = H->size;
 A[i] = x; // ヒープの最後に入れる (注: x はポインタでは無く実体)
 while (i > 1 && GREATER(A[i],A[PARENT(i)]))
  {
   heap_swap(H, i, PARENT(i));
   i = PARENT(i);
  }
}
