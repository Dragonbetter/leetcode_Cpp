#include<iostream>
#include<vector>
#include<queue>

using namespace std;

class Solution{
    public:
    // leetcode 797. All Paths From Source to Target
    vector<vector<int>> result; // 收集符合条件的路径
    vector<int> path;  // 0节点到终点的路径
    // x：目前遍历的节点
    // graph：存当前的图
    void dfs_allpath(vector<vector<int>> graph, int node){
        // end condition
        // 要求从节点 0 到节点 n-1 的路径并输出，所以是 graph.size() - 1
        if(node == graph.size()-1){ 
            // 该节点已经放进去了 
            result.push_back(path); // 找到符合条件的一条路径
            return ;
        }
        // dfs 
        for(int i=0;i<graph[node].size();i++){ // 遍历节点n链接的所有节点
            path.push_back(graph[node][i]);// 遍历到的节点加入到路径中来
            dfs_allpath(graph,graph[node][i]);// 进入下一层递归
            path.pop_back();// 回溯，撤销本节点
        }
    }
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>> &graph){
        path.push_back(0);// 无论什么路径已经是从0节点出发
        dfs_allpath(graph,0);// 开始遍历
        return result;
        // 图节点的输入形式 有向无环图 邻接表格式 
    }
    // bfs 需要注意节点的标记信息  加入队列中的节点需要被立刻标记，这样不会重复访问
    int dir[4][2]={0,1,0,-1,1,0,-1,0}; // 四方图的方向信息 
    void bfs(vector<vector<char>> &grid,vector<vector<bool>> visited,int x,int y){
        queue<pair<int,int>> que; 
        que.push({x,y}); // pair的形式 {int,int} 直接进行元素的插入
        visited[x][y] = 1; // 起始节点 加入后立刻标记
        while(!que.empty()){ // 开始遍历队列里的元素 
            pair<int,int> cur = que.front();
            que.pop(); // 从最前面取出元素；而后后续需要遍历的元素加进去
            int curx = cur.first;
            int cury = cur.second;
            for(int i=0;i<4;i++){ // 向四个方向开始遍历
                int nextx = curx + dir[i][0];
                int nexty = cury + dir[i][1]; // 较好的方式去获取周围四个方向的节点情况 获取后加入遍历
                if(nextx<0 || nextx>=grid.size() || nexty<0 || nexty>=grid[0].size()){
                        continue;  // 下标越界 获取不到对应位置的内容
                    }
                if(!visited[nextx][nexty]){    // 如果节点没被访问过
                    que.push({nextx,nexty});   // 添加节点到队列的末尾 
                    visited[nextx][nexty] = 1; // 加入队列后立刻标记 避免重复访问 
                }
            }
        }
    }
        // leetcode 200 岛屿数量：
    void dfs_Islands(vector<vector<char>> &grid,vector<vector<bool>> & visited,int x,int y){
        for(int i=0;i<4;i++){
            int nextx = x+dir[i][0];
            int nexty = y+dir[i][1];
            if(nextx<0 || nextx>=grid.size() || nexty<0 || nexty>=grid[0].size()){continue;}
            if(!visited[nextx][nexty] && grid[nextx][nexty] == '1'){
                // 没有访问过的，同时是陆地的
                // 需要基于该节点可以访问的所有陆地节点全都访问一遍
                visited[nextx][nexty] = true;
                dfs_Islands(grid,visited,nextx,nexty);
            }
        }
    }
     // 广度搜索 同样的道理也是为了获得以该节点出发的所有相邻的节点 
    void bfs_ISlands(vector<vector<char>>& grid, vector<vector<bool>>& visited, int x, int y) {
        queue<pair<int, int>> que;
        que.push({x, y});
        visited[x][y] = true; // 只要加入队列，立刻标记
        while(!que.empty()) {
            pair<int ,int> cur = que.front(); que.pop();
            int curx = cur.first;
            int cury = cur.second;
            for (int i = 0; i < 4; i++) {
                int nextx = curx + dir[i][0];
                int nexty = cury + dir[i][1];
                if (nextx < 0 || nextx >= grid.size() || nexty < 0 || nexty >= grid[0].size()) continue;  // 越界了，直接跳过
                if (!visited[nextx][nexty] && grid[nextx][nexty] == '1') {
                    que.push({nextx, nexty});
                    visited[nextx][nexty] = true; // 只要加入队列立刻标记
                }
            }
        }
    }
    // 是用遇到一个没有遍历过的节点陆地，计数器就加一，然后把该节点陆地所能遍历到的陆地都标记上。
    int numIslands_dfs(vector<vector<char>>& grid) {
        int n=grid.size(),m=grid[0].size();
        vector<vector<bool>> visited = vector<vector<bool>>(n,vector<bool>(m,false));
        int result = 0;
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                if(!visited[i][j] && grid[i][j] == '1'){
                    // 遍历grid，针对于每个是陆地节点，基于该陆地节点首先进行dfs搜索，合并成最初的岛屿
                    visited[i][j] = true;
                    result++; // 遇到没访问过的陆地 +1
                    dfs_Islands(grid,visited,i,j); // 将其与链接的陆地都标记上 true
                }
            }
        }
        return result;
    }
    


};