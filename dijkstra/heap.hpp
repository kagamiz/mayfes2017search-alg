#include <stdio.h>

//構造体の定義
typedef struct {
int priority;
int value;
} heapdata;


typedef struct {
int max_size;
int size;
heapdata *A;
}heap;

//プロトタイプ宣言

void heapify(heap *H, int i);
heap *heapify_build(int n, heapdata *A, int max_size);
heapdata extract_max(heap *H);
void heap_insert(heap *H, heapdata x);
