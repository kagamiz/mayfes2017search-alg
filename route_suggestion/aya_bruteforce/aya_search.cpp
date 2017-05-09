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
int *all_permutation_search(int size, int *start, int *timelength, int **dm)
{
	int d, i, j, min, nowplace, nextplace, nowtime, nexttime, deadline, nextstart;
	min = INFINITY;
	int *index = 0;
	index = new int[size];
	for(i = 0; i < size; i++) index[i] = i + 1;	// 配列 1,...,n を作成
	int *index_min = 0;	// 重み最小の順列を保存する配列
	index_min = new int[size];
	index_min[0] = -1;
	do
	{
		nowtime = start[0];
		nowplace = 0;
		d = 0;
		for(i = 0; i < size; i++)
		{
			nextplace = index[i];
			nexttime = nowtime + dm[nowplace][nextplace];
			deadline = start[nextplace];
			if(deadline != -1 && deadline < nexttime)
			{
				d = INFINITY;
				break;
			}
			nextstart = max(nexttime, deadline);
			d += nextstart - nowtime;	// 2頂点間の重みを足す
			nowplace = nextplace;
			nowtime = max(nexttime, deadline) + timelength[nextplace];
		}
		if(d < min && nowtime <= timelength[0])
		{
			min = d; // 最小値を更新
			for(j = 0; j < size; j++) index_min[j] = index[j];	// 重み最小の順列を保存
		}
	}
	while(next_permutation(index, index + size));	// 次の順列が存在する間繰り返す
	delete[] index;
	return index_min;
}
