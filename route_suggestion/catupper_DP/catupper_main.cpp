/*
 * catupper_main.cpp
 *
 *  Created on: 2017/05/11
 *      Author: catupper
 */

#include <iostream>
#include <algorithm>
#define INF (1<<20)
using namespace std;


int n;
int v[20], s[20], t[20], e[20], d[20][20];
int **dp, **prev;

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

int calc(int start, int from, int to){//from, toは1-indexed
  int now = start + d[from][to];//nowはtoに到着する時刻
  if(s[to] != -1 && now > s[to])return INF;//企画が始まるまでに到着しない
  if(e[to] != -1 && now > e[to])return INF;//企画が終わるまでに到着しない
  now = max(now, s[to]);// 企画の開始時刻
//  if(e[to] != -1)now = e[to];//企画の終了時刻があるときの終了時刻
//  else now += t[to];//企画の終了時刻がないときの終了時刻
  now += t[to];//企画の終了時刻がないときの終了時刻
  if(e[to] != -1 && now > e[to])return INF;//t[to]分過ごすと、企画の終了時刻を過ぎてしまう。
  return now;
}


int main()
{
  cin >> n;	// 入力（頂点数）
  cin >> v[0];	// 入力（始点）
  cin >> s[0];	// 開始時刻
  cin >> e[0];	// 終了時刻
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
  int lastok = 0;
  for(int i = 0;i < (1 << n);i++){ // 訪れた頂点(0-index)
    for(int j = 0;j < n;j++){ // 今いる頂点(0-index)
      if(dp[i][j] == INF)continue;
      lastok = i;
      for(int k = 0;k < n;k++){ // 次の頂点
	if(i & (1 << k))continue;
	int from = j + 1;//1-indexedになおす
	int to = k + 1;//なおす
	if(i == 0)from = 0;
	int tmp = calc(dp[i][j], from, to);//時刻dp[i][j]にjを出発してkに行ったあとに、kを出発する時間
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
  cout << dp[(1<<n)-1][last] << " "<< e[0] << endl;
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
//  for(int i = 0; i <= n; i++){ //v[0]は不要
    cout << v[ans[i]+1] << endl;
  }

  array_free(dp, 1<<n, n);
  array_free(prev, 1<<n, n);
  
  return 0;
}
