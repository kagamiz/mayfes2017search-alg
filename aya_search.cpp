/*
 * aya_search.cpp
 *
 *  Created on: 2017/02/19
 *      Author: Aya
 */

#include <iostream>
#include <algorithm>
#include "aya_header.h"

using namespace std;

#define INFINITY 99999999

/* 全順列から重み最小の並びを探す */
int *all_permutation_search(int size, double **dm, int *vertex)
{
	int i, j;
	double d, min;

	min = INFINITY;

	int *index = 0;
	index = new int[size];
	for(i = 0; i < size; i++)
	{
			index[i] = i;
	}
	// 配列0,...,n-1を作成

	int *vertex_min = 0; // 重み最小の順列を保存する配列
	vertex_min = new int[size];

	do
	{
		d = 0.0;
		for(i = 1; i < size; i++)
		{
			d += dm[index[i - 1]][index[i]];
		}
		// 2頂点間の重みを足す
		if(d < min)
		{
			min = d; // 最小値を更新
			for(j = 0; j < size; j++)
			{
				vertex_min[j] = vertex[index[j]];
			}
			// 重み最小の順列を保存
		}
	}
	while(next_permutation(index, index + size));
	// 次の順列が存在する間繰り返す

	delete[] index;

	return vertex_min;
}
