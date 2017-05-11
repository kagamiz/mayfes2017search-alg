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
		for (j = 0; j <= size; j++)
		{
			d_ij = distancematrix[i][j];
			if (d_ij < d_min_sub && d_ij != 0) d_min_sub = d_ij;
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
	int d, i, j, m, min, nowplace, nextplace, nowtime, nexttime, deadline, nextstart;
	min = INFINITY;

	int *index_min = 0;	// 重み最小の順列を保存する配列
	index_min = new int[size];
	index_min[0] = -1;

	m = 0; // 開始時刻の決まっている企画数
	for (i = 1; i <= size; i++)
		{
			if (start[i] != -1) m++;
		}
	int *order = 0;
	order = determined(size, start, m);
	insertionsort_starttime(order, m, start);

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
			if (!valid(size, index, m, order)) continue;
			nowtime = start[0];
			nowplace = 0;
			d = 0; // 移動と待ち時間の合計
			for (i = 0; i < size; i++)
			{
				nextplace = index[i];
				nexttime = nowtime + distancematrix[nowplace][nextplace];
				deadline = start[nextplace];
				int et = end[nextplace];
				if (et != -1)
				{
					int at = et - timelength[nextplace];
					if (deadline == -1 || at < deadline) deadline = at;
				}
				if (deadline != -1 && deadline < nexttime) // 全部回れそうか
				{
					d = INFINITY;
					break;
				}
				nextstart = max(nexttime, deadline);
				d += nextstart - nowtime; // 2頂点間の移動時間と待ち時間を足す
				nowplace = nextplace;
				nowtime = max(nexttime, deadline) + timelength[nextplace];
			}
			if (d < min && nowtime <= end[0])
			{
				min = d; // 最小値を更新
				for (j = 0; j < size; j++) index_min[j] = index[j]; // 重み最小の順列を保存
			}
		}
		while (next_permutation(index, index + size)); // 次の順列が存在する間繰り返す
		delete[] index;
	}
	return index_min;
}
