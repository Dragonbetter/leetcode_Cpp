// 求最短路径 M矩阵 左上角走到右下角 只能往下走 每个矩阵都有一个weight
// 3*3矩阵 [[1,3,1],[1,5,1],[4,2,1]] (0,0)->(2,2) 往下或则往右 

#include <iostream>
#include <vector>
//#include <cmath>
#include <algorithm> // min max 的算法库错误！！
using namespace std;
int minweight(vector<vector<int>> &weights){
    int size1 = weights.size();
    int size2 = weights[0].size();
    vector<vector<int>> dp(size1, vector<int>(size2, 0));
    dp[0][0]=weights[0][0];
   // 初始化两边序列值 在初始化 dp 矩阵的边界值时，使用了错误的索引
   //，weights[i-1][0]应该是 weights[i][0] 和weights[0][j-1]转为 weights[0][j]。
    for(int i=1;i<size1;i++){
        dp[i][0]=dp[i-1][0]+weights[i][0];
    }
    for(int j=1;j<size2;j++){
        dp[0][j]=dp[0][j-1]+weights[0][j];
    }
    // dp从左到右
    for(int i=1;i<size1;i++){
        for(int j=1;j<size2;j++){
            // 逻辑错误
            // 之前写成了 max(dp[i][j-1]+weights[i][j-1],dp[i-1][j]+weights[i-1][j])
            dp[i][j]=min(dp[i][j-1]+weights[i][j],dp[i-1][j]+weights[i][j]);
        }
    }
    return dp[size1-1][size2-1];
}
int main(){
    vector<vector<int>> weights={{1,3,1},{1,5,1},{4,2,1}};
    int output = minweight(weights);
    cout<<output<<endl;
}
