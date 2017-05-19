/*
 * search.cpp
 *
 *  Created on: 2017/05/17
 *      Author: Aya, catupper
 */

#include <iostream>
#include <algorithm>
using namespace std;
#define INF (1<<20)

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
    d_min_sub = INF;
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
  l_min = INF;
  d_sum_min = INF;

  int *index_min = 0;  // 重み最小の順列を保存する配列
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
          l = INF;
          d_sum = INF;
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

int **array_malloc(int a, int b){
  int **x;
  x = (int**)malloc(sizeof(int*)*a);
  for(int i = 0;i < a;i++)
    x[i] = (int*)malloc(sizeof(int)*b);
  return x;
}

void array_free(int **x, int a, int b){
  for(int i = 0;i < a;i++)free(x[i]);
  free(x);
}

int calc(int start, int from, int to, int s[25], int e[25], int t[25], int d[25][25]){//from, toは1-indexed
  int now = start + d[from][to];//nowはtoに到着する時刻
  if(s[to] != -1 && now > s[to])return INF;//企画が始まるまでに到着しない
  if(e[to] != -1 && now > e[to])return INF;//企画が終わるまでに到着しない
  now = max(now, s[to]);// 企画の開始時刻  
  now += t[to];//企画の終了時刻がないときの終了時刻
  if(e[to] != -1 && now > e[to])return INF;//t[to]分過ごすと、企画の終了時刻を過ぎてしまう。
  return now;
}

int main()
{
  int n;

  cin >> n;  // 入力（頂点数）

  if(n < 10) // 入力が少なければ全探索
  {
    int i, j;
    int *v = 0;  // 頂点番号の配列
    int *s = 0;  // 到着したい時刻
    int *e = 0;  // 企画終了時刻
    int *t = 0;  // 費やす時間
    v = new int[n + 1];
    s = new int[n + 1];
    e = new int[n + 1];
    t = new int[n + 1];
    cin >> v[0];  // 入力（始点）
    cin >> s[0];  // 開始時刻
    cin >> e[0];  // 帰る時刻
    cin >> t[0];  // 特に使わない

    for(i = 1; i <= n; i++) cin >> v[i] >> s[i] >> e[i] >> t[i]; // 入力（頂点番号, 開始時刻, 終了時刻, 費やす時間）
  
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
  }
  else
  {
    if(n < 21) // 入力が多ければDP
    {
      int v[25], s[25], t[25], e[25], d[25][25];
      int **dp, **prev;
      cin >> v[0];
      cin >> s[0];
      cin >> e[0];
      cin >> t[0];
      dp = array_malloc(1<<n, n);
      prev = array_malloc(1<<n, n);
      for(int i = 1; i <= n; i++){
        cin >> v[i] >> s[i] >> e[i] >> t[i];	// 入力（頂点番号, 到着したい時刻, 費やす時間）
      }

      for(int i = 0;i <= n;i++){
        for(int j = 0;j <= n;j++){
          cin >> d[i][j];
        }
      }

      for(int i = 0;i < n;i++){
        for(int j = 0;j < (1 << n);j++){
          dp[j][i] = INF;
        }
      }
      dp[0][0] = s[0];
      for(int i = 0;i < (1 << n);i++){ // 訪れた頂点(0-index)
        for(int j = 0;j < n;j++){ // 今いる頂点(0-index)
          if(dp[i][j] == INF)continue;
          for(int k = 0;k < n;k++){ // 次の頂点
            if(i & (1 << k))continue;
            int from = j + 1;//1-indexedになおす
            int to = k + 1;//なおす
            if(i == 0)from = 0;
            int tmp = calc(dp[i][j], from, to, s, e, t, d);//時刻dp[i][j]にjを出発してkに行ったあとに、kを出発する時間
            if(tmp < dp[i | (1 << k)][k]){//改善されるなら更新
              prev[i | (1 << k)][k] = j;
              dp[i | (1 << k)][k] = tmp;
            }
          }
        }
      }
      int last = 0;
      for(int j = 0;j < n;j++){
        if(dp[(1<<n)-1][j] < dp[(1<<n)-1][last]){
          last = j;
        }
      }
      if(dp[(1<<n)-1][last] > e[0]){
        cout << -1 << endl;
        return 0;
      }

      int ans[n+1];
      ans[n] = last;
      int bitmask = (1<<n)-1;
      for(int i = n;i > 0;i--){
        ans[i-1] = prev[bitmask][ans[i]];
        bitmask ^= 1<<ans[i];
      }
      ans[0] = -1;
      cout << n << endl;
      for(int i = 1; i <= n; i++){
        cout << v[ans[i]+1] << endl;
      }

      array_free(dp, 1<<n, n);
      array_free(prev, 1<<n, n);
    }
    else cout << -1 << endl;
  }

  return 0;
}

