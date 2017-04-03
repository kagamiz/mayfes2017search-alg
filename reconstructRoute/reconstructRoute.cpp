#include<iostream>
#include<cstdio>
#include<sstream>
#include<memory>
#include<mysql_driver.h>
#include<mysql_connection.h>
#include<mysql_error.h>
#include<cppconn/statement.h>
#include<cppconn/resultset.h>

#include<algorithm>
#include<vector>
#include<map>
using namespace std;
typedef pair<int, int> P;

#include "../SQLlib/mysql_config.hpp"
#include "../graph_data/config.hpp"


#define INF (1 << 28)

sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
unique_ptr<sql::Connection> con(driver->connect(HOST, USER, PASSWORD));
unique_ptr<sql::Statement> stmt(con->createStatement());
/*********************
stmt -> execute ("SQL statement");
f.g stmt->execute("USE " + DATABASE);

unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM conference"));

size_t row = 0;
while (res->next()) {
            cout << row << "\t";
            cout << "cid = " << res->getInt(1);
            cout << ", name = '" << res->getString("name") << "' " << endl;
            row++;
}
*********************/



void init(){
  stmt->execute("USE " + DATABASE);
}


int getNodeNumber(){
  return NODE_NUMBER;
}

int getBuildingNumber(){
  return BUILDING_NUMBER;
}

int readInt(string str){
  istringstream os(str);
  int res;
  os >> res;
  return res;
}

void setRoute(int from, int to, vector<int> route, int row){
  char str[1080];
  for(int i = 1;i < route.size();i++){
    sprintf(str, "INSERT INTO checkpos_order_89 PERSONAL(%s, %s, %s, %s) VALUES(%d, %d, %d, %d)",
	   "co_cd_pid", "co_node1", "co_node2", "co_order",
	   row, route[i-1], route[i], i
	   );
    stmt->execute(str);
  }
  return;
}
P getUsedExit(int from, int to){
  char str[1080];
  sprintf(str, "SELECT cd_node1, cd_node2 FROM checkpos_data_89 WHERE cd_bd_pid1 = %d AND cd_bd_pid2 = %d",
	 from, to
	 );

  unique_ptr<sql::ResultSet> res(stmt->executeQuery(str));
  P resP;
  resP.first = readInt(res->getString("cd_node1"));
  resP.second = readInt(res->getString("cd_node2"));
  return resP; 
}

vector<int> adjacentList(int node){
  vector<int> resV;
  char str[1080];
  sprintf(str, "SELECT graph_data_89.adjacent_data_89 WHERE from_id = %d", node);
  unique_ptr<sql::ResultSet> res(stmt->executeQuery(str));
  while(res->next()){
    resV.push_back(readInt(res->getString("to_id")));
  }
  return resV;
}


int dist(int u, int v){
  char str[1080];
  sprintf(str, "SELECT graph_data_89.adjacent_data_89 WHERE from_id = %d AND to_id = %d", u, v);
  unique_ptr<sql::ResultSet> res(stmt->executeQuery(str));
  return readInt(res->getString("move_time"));

}


vector<int> rev(vector<int> vec){
  int n = vec.size();
  for(int i = 0;i < (n - i - 1);i++){
    swap(vec[i], vec[n-i-1]);
  }
  return vec;
}

int getRouteRow(int from, int to){
  char str[1080];
  sprintf(str, "SELECT cd_pid FROM checkpos_data_89 WHERE cd_bd_pid1 = %d AND cd_bd_pid2 = %d",
	 from, to
	 );

  unique_ptr<sql::ResultSet> res(stmt->executeQuery(str));
  return readInt(res->getString("cd_pid"));
}

void reconstruct(int from, int to){
  vector<int> route;
  int row = getRouteRow(from, to);
  P exits = getUsedExit(from, to);
  int fromExit = exits.first;
  int toExit = exits.second;
  
  for(int x = toExit;x != fromExit;){
    route.push_back(x);
    vector<int> adj = adjacentList(x);
    for(int i = 0;i < adj.size();i++){
      int tmp = adj[i];
      if(dist(fromExit, tmp) + dist(tmp, x) == dist(fromExit, x)){
	x = tmp;
	break;
      }
    }
  }
  route.push_back(fromExit);
  setRoute(from, to, rev(route), row);
}

int main(){
  void init();

  int N = getNodeNumber();
  int B = getBuildingNumber();

  for(int i = 1;i <= B;i++){
    for(int j = 1;j <= B;j++){
      if(i == j)continue;
      reconstruct(i, j);
    }
  }
}
	  
