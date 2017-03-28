#include<iostream>
#include<fstream>
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

/***
g++ -std=gnu++0x -I/home/kenogura/Documents/mySQLlib/include/
***/

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
  stmt->execute("CREATE DATABASE IF NOT EXIST graph_data_89");
  stmt->execute("DROP TABLE IF EXISTS adjacent_data_89");
  stmt->execute("CREATE TABLE adjacent_data_89(ad_pid int(3) unsigned NOT NULL AUTO_INCREMENT, from_id int(3) DEFAULT NULL, to_id int(3) DEFAULT NULL, edge_cost int(3) DEFAULT NULL)");
  
  stmt->execute("DROP TABLE IF EXISTS exit_data_89");
  stmt->execute("CREATE TABLE exit_data_89(ed_pid int(3) unsigned NOT NULL AUTO_INCREMENT, building_id int(3) DEFAULT NULL, node_id int(3) DEFAULT NULL)");
  
  stmt->execute("USE graph_data_89");
}

void addEdge(int u, int v, int t){
  char str[1080];
  sprintf(str, "INSERT INTO adjacent_data_89 PERSONAL(%s, %s, %s) VALUES(%d, %d, %d)",
	 "from_id", "to_id", "edge_cost",
	 u, v, t
	 );
  stmt->execute(str);
}
void setExits(int bid, int e){
  char str[1080];
  sprintf(str, "INSERT INTO exit_data_89 PERSONAL(%s, %s) VALUES(%d, %d)",
	 "building_id", "node_id",
	 bid, e
	 );
  stmt->execute(str);
}



void setAdjTable(){
  ifstream ifs("campus_graph.txt");
  int n, m, u, v, t;
  ifs >> n >> m;
  for(int i = 0;i < m;i++){
    ifs >> u >> v >> t;
    addEdge(u, v, t);
  }
}

void setBuildTable(){
  ifstream ifs("grouping");
  int n, bid, k, e;
  
  ifs >> n;
  for(int i = 0;i < n;i++){
    ifs >> bid;
    ifs >> k;
    for(int j = 0;j < k;j++){
      ifs >> e;
      setExits(bid, e);
    }
  }
}

int main(){
  void init();
  setAdjTable();
  setBuildTable();
}
	  
