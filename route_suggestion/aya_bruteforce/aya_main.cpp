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
	int i, j, n, w, x, y, z;

	cin >> n;	// 入力（頂点数）

	int *v = 0;	// 頂点番号の配列
	int *s = 0;	// 到着したい時刻
	int *e = 0;	// 企画終了時刻
	int *t = 0;	// 費やす時間
	v = new int[n + 1];
	s = new int[n + 1];
	e = new int[n + 1];
	t = new int[n + 1];
	cin >> v[0];	// 入力（始点）
	cin >> s[0];	// 開始時刻
	cin >> e[0];	// 帰る時刻
	cin >> t[0];	// 特に使わない

	for(i = 1; i <= n; i++) cin >> v[i] >> s[i] >> e[i] >> t[i];	// 入力（頂点番号, 開始時刻, 終了時刻, 費やす時間）
	
/* 重み行列 */
	int **d = 0;
	d = new int*[n + 1];
	for(i = 0; i <= n; i++) d[i] = new int[n + 1];
	for(i = 0; i <= n; i++)
	{
		for(j = 0; j <= n; j++) cin >> d[i][j]; // 入力（重み）
	}

	int *q = 0; // 重み最小となる頂点の並びを保存する配列
	q = all_permutation_search(n, s, e, t, d); // 全順列から重み最小の並びを探す
	if(q[0] == -1) cout << -1 << endl; // 出力（解なしの場合）
	else
	{
		cout << n << endl; // 出力（頂点数）
		for(i = 0; i < n; i++) cout << v[q[i]] << endl; // 出力（頂点番号）
	}
	delete[] q;

	delete[] v;
	delete[] s;
	delete[] e;
	delete[] t;
	for(i = 0; i <= n; i++) delete[] d[i];
	delete[] d;

	return 0;
}

