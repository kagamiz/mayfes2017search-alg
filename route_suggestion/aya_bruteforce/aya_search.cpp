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
#define INFINITY (1<<20)

void swap(int *x, int *y)
{
	int tmp = *x;
	*x = *y;
	*y = tmp;
}

void insertionsort_starttime(int array[], int array_size, int *start)
{
	int i, j;
	for (i = 1; i < array_size; i++)
	{
		j = i;
		while (j > 0)
		{
			if (start[array[j - 1]] > start[array[j]]) swap(&array[j - 1], &array[j]);
			j--;
		}
	}
}

int *determined(int size, int *start, int number)
{
	int i, j;
	int *a = 0; // 開始時刻の決まっている企画の番号を保存
	a = new int[number];
	i = 1;
	j = 0;
	while (i <= size)
	{
		if (start[i] != -1)
		{
			a[j] = i;
			j++;
		}
		i++;
	}
	return a;
}

bool valid(int size, int *index, int number, int *order)
{
	int i, j;
	i = 0;
	j = 0;
	while (i < size)
	{
		if (index[i] == order[j]) j++;
		if (j == number) return true;
		i++;
	}
	return false;
}

int *shortageoftime(int size, int *start, int *end, int *timelength, int **distancematrix)
{
	int i, j, l, d_ij, d_min_max, d_min_sub, d_sum_sub, t_sum;
	t_sum = 0;
	for (i = 1; i <= size;  i++) t_sum += timelength[i];
	l = start[0] + t_sum - end[0];
	d_min_max = 0;
	int *d_min = 0;
	d_min = new int[size + 1];
	for (i = 0; i <= size; i++)
	{
		d_min_sub = INFINITY;
		for (j = 0; j < i; j++)
		{
			d_ij = distancematrix[i][j];
			if (d_ij < d_min_sub) d_min_sub = d_ij;
		}
		for (j = i + 1; j <= size; j++)
		{
			d_ij = distancematrix[i][j];
			if (d_ij < d_min_sub) d_min_sub = d_ij;
		}		
		d_min[i] = d_min_sub;
		if (d_min_max < d_min_sub) d_min_max = d_min_sub;
	}
	d_sum_sub = 0;
	for (i = 0; i <=size; i++) d_sum_sub += d_min[i];
	int *d_sum = 0;
	d_sum = new int[size + 1];
	d_sum[0] = d_sum_sub - d_min_max + l;
	for(i = 1; i <= size; i++) d_sum[i] = d_sum_sub - d_min[i] + l;
	delete[] d_min;
	return d_sum;
}

/* 全順列から重み最小の並びを探す */
int *all_permutation_search(int size, int *start, int *end, int *timelength, int **distancematrix)
{
	int d, i, j, l, m, nowplace, nextplace, nowtime, nexttime, deadline, nextstart, et, at, l_min, d_sum, d_sum_min;
	l_min = INFINITY;
  d_sum_min = INFINITY;

	int *index_min = 0;	// 重み最小の順列を保存する配列
	index_min = new int[size];
	index_min[0] = -1;

	m = 0; // 開始時刻の決まっている企画数
	for (i = 1; i <= size; i++)
	{
		if (start[i] != -1) m++;
	}

	int *order = 0;
	if(m > 0)
	{		
		order = determined(size, start, m);
		insertionsort_starttime(order, m, start);
	}

	int *st = 0;
	st = shortageoftime(size, start, end, timelength, distancematrix);

	if(st[0] <= 0)
	{
		int *index = 0; // プログラム中での頂点番号 1,...,n の配列
		index = new int[size];
		for(i = 0; i < size; i++) index[i] = i + 1; // 配列 1,...,n を作成
		do
		{
			if (st[index[size - 1]] > 0) continue;
			if (m > 0 && !valid(size, index, m, order)) continue;
			nowtime = start[0];
			nowplace = 0;
			d_sum = 0; // 移動時間の合計
			l = 0; // 移動と待ち時間の合計
			for (i = 0; i < size; i++)
			{
				nextplace = index[i]; // 行先
				d = distancematrix[nowplace][nextplace];
				d_sum += d;
				nexttime = nowtime + d; // 到着時刻
				nextstart = start[nextplace]; // 行先の開始時刻
				deadline = nextstart; // 行先に到着していなければならない時刻(1)
				et = end[nextplace]; // 行先の終了時刻
				if (et != -1)
				{
					at = et - timelength[nextplace]; // 行先に到着していなければならない時刻(2)
					if (deadline == -1 || at < deadline) deadline = at;
				}
				if (deadline != -1 && deadline < nexttime) // 全部回れそうか
				{
					l = INFINITY;
					d_sum = INFINITY;
					break;
				}
				nextstart = max(nexttime, nextstart);
				l += nextstart - nowtime; // 2頂点間の移動時間と待ち時間を足す
				nowplace = nextplace;
				nowtime = nextstart + timelength[nextplace];
			}
			if (nowtime <= end[0] && l <= l_min)
			{
			  if(l < l_min || d_sum < d_sum_min)
			  {
				  l_min = l; // 最小値を更新
				  d_sum_min = d_sum;
				  for (j = 0; j < size; j++) index_min[j] = index[j]; // 重み最小の順列を保存
			  }
		  }
		}
		while (next_permutation(index, index + size)); // 次の順列が存在する間繰り返す
		delete[] index;
	}
	delete[] order;
	return index_min;
}

