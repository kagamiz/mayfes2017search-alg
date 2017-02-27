/*
 * aya_main.cpp
 *
 *  Created on: 2017/02/19
 *      Author: Aya
 */

#include <iostream>
#include <algorithm>
#include "aya_header.h"

using namespace std;

int main()
{
	int i, j, n, x;
	double z;

	cin >> n;	// 入力（頂点数）

/* 頂点番号の配列 */
	int *p = 0;
	p = new int[n];
	for(i = 0; i < n; i++)
	{
		cin >> x;	// 入力（頂点番号）
		p[i] = x;
	}

/* 重み行列 */
	double **M = 0;
	M = new double*[n];
	for(i = 0; i < n; i++)
	{
		M[i] = new double[n];
	}
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			cin >> z;	// 入力（重み）
			M[i][j] = z;
		}
	}

	int *q = 0;	// 重み最小となる頂点の並びを保存する配列

	q = all_permutation_search(n, M, p);	// 全順列から重み最小の並びを探す

	for(i = 0; i < n; i++)
	{
		cout << q[i] << endl;	// 出力（頂点番号）
	}

	delete[] p;
	delete[] q;
	for(i = 0; i < n; i++)
	{
		delete[] M[i];
	}
	delete[] M;

	return 0;
}
