/*
 * catupper_main.cpp
 *
 *  Created on: 2017/05/11
 *      Author: catupper
 */

#include <iostream>
#include <algorithm>
#include <time.h>
#define INF (1<<20)
using namespace std;
int main()
{
  int n;
  cin >> n;	// 入力（頂点数）

  
  int v[n+1], s[n+1], t[n+1], e[n+1], d[n+1][n+1];
  cin >> v[0];	// 入力（始点）
  cin >> s[0];	// 開始時刻
  //	s[0] = local -> tm_hour * 60 + local -> tm_min;
  cin >> e[0];
  cin >> t[0];	// 終了時刻

  //	t[0] = 0;
  for(int i = 1; i <= n; i++){
    cin >> v[i] >> s[i] >> e[i] >> t[i];	// 入力（頂点番号, 到着したい時刻, 費やす時間）
  }

  for(int i = 0;i <= n;i++){
    for(int j = 0;j <= n;j++){
      cin >> d[i][j];
    }
  }

  int dp[1<<n][n], prev[1<<n][n];
  for(int i = 0;i < n;i++){
    for(int j = 0;j < (1 << n);j++){
      dp[j][i] = INF;
    }
  }
  dp[0][0] = s[0];
  for(int i = 0;i < (1 << n);i++){
    for(int j = 0;j < n;j++){
      if(dp[i][j] == INF)continue;
      for(int k = 0;k < n;k++){
	if(i & (1 << k))continue;
	int from = j+1;
	if(i == 0)from = 0;
	int to = k+1;
	int arv = dp[i][j] + d[from][to];
	if(s[to] != -1 && arv > s[to])continue;
	arv = max(arv, s[to]);
	if(e[to] != -1)arv = e[to];
	else arv += t[to];
	if(arv < dp[i | (1 << k)][k]){
	  prev[i | (1<<k)][k] = j;
	  dp[i | (1<<k)][k] = arv;
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
  for(int i = 0;i <= n;i++){
    cout << v[ans[i]+1] << endl;
  }
  return 0;
}
