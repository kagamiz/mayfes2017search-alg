#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

#define INF (1<<28)

int n, id[30], start[30], end[30], open[30], stay[30], d[30][30], ans[30];

void ng(){
  cout << -1 << endl;
  exit(0);
}

int **array_malloc(int n, int m){
  int **res;
  res = (int**)malloc(sizeof(int *) * n);
  for(int i = 0;i < n;i++)res[i] = (int*)malloc(sizeof(int) * m);
  return res;
}

int calc(int s, int from, int to){
  if(s >= INF)return INF;
  int now = s + d[from][to];
  if(start[to] != -1 && now > start[to])return INF;
  
  if(end[to] != -1 && now > end[to])return INF;
  now = max(now, open[to]);
  now = max(now, start[to]);
  now += stay[to];
  if(end[to] != -1 && now > end[to])return INF;
  return now;
}

int calcbf(vector<int> index){
  int nowtime = start[0];
  int from = 0;
  for(int i = 0;i < index.size();i++){
    int to = index[i];
    nowtime = calc(nowtime, from, to);
    from = to;
  }
  return nowtime;
}

void bf(){
  vector<int> index;
  int best = INF;
  vector<int> res;
  for(int i = 1;i <= n;i++)index.push_back(i);
  do{
    int tmp = calcbf(index);
    if(tmp < best){
      best = tmp;
      res = index;
    }
  }while(next_permutation(index.begin(),  index.end()));
  if(best > end[0])ng();
  //  cout << best << endl;
  cout << res.size() << endl;
  for(int i = 0;i < res.size();i++){
    cout << id[res[i]] << endl;
  }
}

void bitDP(){
  int **dp = array_malloc(n, 1<<n);
  int **prev = array_malloc(n, 1<<n);
  for(int i = 0;i < n;i++)
    for(int j = 0;j < (1<<n);j++)
      dp[i][j] = INF;

  dp[0][0] = start[0];

  for(int i = 0;i < (1<<n);i++){
    for(int j = 0;j < n;j++){
      if(dp[j][i] == INF)continue;
      for(int k = 0;k < n;k++){
	if(i & (1 << k))continue;
	int from = j + 1;
	int to = k + 1;
	if(i == 0)from = 0;
	int dep = calc(dp[j][i], from, to);
	int bitmask = i | (1 << k);
	if(dep < dp[k][bitmask]){
	  dp[k][bitmask] = dep;
	  prev[k][bitmask] = j;
	}
      }
    }
  }

  int last = 0;
  for(int i = 0;i < n;i++){
    if(dp[i][(1<<n)-1] < dp[last][(1<<n)-1])last = i;
  }
  if(dp[last][(1<<n)-1] > end[0])ng();
  //  cout << dp[last][(1<<n)-1] << endl;
  int bitmask = (1<<n) - 1;
  for(int i = n-1;i >= 0;i--){
    int  tmp = last;
    ans[i] = id[last+1];
    last = prev[last][bitmask];
    bitmask ^= 1 << tmp;
  }
  cout << n << endl;
  for(int i = 0;i < n;i++)cout << ans[i] << endl;
}

int main(){
  cin >> n;
  for(int i = 0;i <= n;i++){
    cin >> id[i] >> open[i] >> start[i] >> end[i] >> stay[i];
  }
  for(int i = 0;i <= n;i++)
    for(int j = 0;j <= n;j++)
      cin >> d[i][j];
  if(n <= 10)bf();
  else bitDP();
  return 0;
}
