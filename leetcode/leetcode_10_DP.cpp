#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 动规五部曲
// 1.确定dp数组（dp table）以及下标的含义
// 2.确定递推公式
// 3.dp数组如何初始化
// 4.确定遍历顺序
// 5.举例推导dp数组

class DP_Solution{
    public: 
    // 509. 斐波那契数 dp[i] = dp[i - 1] + dp[i - 2]; 时间复杂度：O(n) 空间复杂度：O(n)
    int fib_1(int n){
        // 初始化
        if(n<=1) return n;
        std::vector<int> dp(n+1); // 直接初始化 初始化为0 
        dp[0] = 0;
        dp[1] = 1;
        for(int i=2;i<=n;i++){
            dp[i] = dp[i-1]+dp[i-2];
        }
        return dp[n];
    }
    // 当然可以发现，我们只需要维护两个数值就可以了，不需要记录整个序列。时间复杂度：O(n) 空间复杂度：O(1)
    int fib_2(int N) {
        if (N <= 1) return N;
        int dp[2];
        dp[0] = 0;
        dp[1] = 1;
        for (int i = 2; i <= N; i++) {
            int sum = dp[0] + dp[1];
            dp[0] = dp[1];
            dp[1] = sum;
        }
        return dp[1];
    }
    // 递归 时间复杂度：O(2^n) 空间复杂度：O(n)，算上了编程语言中实现递归的系统栈所占空间
    int fib_3(int n){
        if(n<2) return n;
        return fib_3(n-1)+fib_3(n-2);
    }
    // 70. 爬楼梯 dp[i]=dp[i-1]+dp[i-2] (往前退1步或则退2两步；主要在于本身可退 dp[i-1] 包含了dp[i-2]的优化过程)
    int climbStairs(int n){
        if(n<=1) return n;
        vector<int> dp(n+1);
        dp[1]=1;
        dp[2]=2;
        for(int i=3;i<=n;i++){ // 因为有n+1个数值 故而n是存在的 
            dp[i] = dp[i-1]+dp[i-2];
        }
        return dp[n];
    }
    // ==> 拓展：一步爬台阶的数目进行调整，调整为1,2,3,4，，m;则有多少种？？，二维的？
    // Todo 57. 爬楼梯（第八期模拟笔试） https://kamacoder.com/problempage.php?pid=1067

    // 746. 使用最小花费爬楼梯  dp[i]= min(dp[i-1]+cost[i-1],dp[i-2]+cost[i-2])
    int minCostClimbingStairs(vector<int>& cost) {
        // 到达第i台阶所花费的最少体力为dp[i]。
        // dp[i]= min(dp[i-1]+cost[i-1],dp[i-2]+cost[i-2])
        vector<int> dp(cost.size()+1); // 顶端是最后一个 
        dp[0]=0;
        dp[1]=0;
        for(int i=2;i<=cost.size();i++){ // cost.size()是虚拟的最后一个
            dp[i] = min(dp[i-1]+cost[i-1],dp[i-2]+cost[i-2]);
        }
        return dp[cost.size()];
    } 
    // 62.不同路径 dp[i][j] = dp[i-1][j]+dp[i][j-1] 时间复杂度：O(m × n) 空间复杂度：O(m × n)
    // 初始化方式 dp[i][0]=1;dp[0][j]=1;
    // 遍历方式  从左到右一层一层遍历
    // Todo 数论方法 组合 深度搜索
    int uniquePaths(int m, int n) {
        vector<vector<int>> dp(m,vector<int>(n,0));
        // 初始化
        for(int i=0;i<m;i++){dp[i][0]=1;}
        for(int j=0;j<n;j++){dp[0][j]=1;}
        // 开始遍历数组
        for(int i=1;i<m;i++){
            for(int j=1;j<n;j++){
                dp[i][j]=dp[i-1][j]+dp[i][j-1];
            }
        }
        return dp[m-1][n-1];
    }
    // 63. 不同路径 II 考虑障碍物信息
    // 引入判断条件，如果此处有障碍物 那么该点的dp[i][j]=0，表示走到这里后 断了 
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        // 获取obstacleGrid的长宽
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        vector<vector<int>> dp(m,vector<int>(n,0));
        // 初始化
        if (obstacleGrid[m - 1][n - 1] == 1 || obstacleGrid[0][0] == 1) //如果在起点或终点出现了障碍，直接返回0
            return 0;
        // 更加简洁的写法！！
        // for (int i = 0; i < m && obstacleGrid[i][0] == 0; i++) dp[i][0] = 1;
        // for (int j = 0; j < n && obstacleGrid[0][j] == 0; j++) dp[0][j] = 1;
        for(int i=0;i<m;i++)
        {
            if(obstacleGrid[i][0]==0) {dp[i][0]=1;}
            else{break;}
        }
        for(int j=0;j<n;j++){
            if(obstacleGrid[0][j]==0){
            dp[0][j]=1;}
            else{break;}
        }
        // 开始遍历数组
        for(int i=1;i<m;i++){
            for(int j=1;j<n;j++){
                //if(obstacleGrid[i][j]==0){dp[i][j]=dp[i-1][j]+dp[i][j-1];}
                //else{dp[i][j]=0;}
                if (obstacleGrid[i][j] == 1) continue;
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }
        return dp[m-1][n-1];
    }
    // 343. 整数拆分 给定一个正整数 n，将其拆分为至少两个正整数的和，并使这些整数的乘积最大化。 返回你可以获得的最大乘积。
    int integerBreak(int n) {
        // 动态规划分析 ：一维数组 从0到n逐步求解递推获得最终解答 
        // 初步可认为dp[i] = dp[i-j]*dp[j];但这样初始化有问题，可以换一种
        // 即dp[i] = max( dp[i], max((i-j)*j,dp[i-j]*j)) :
        // 其中（i-j）*j表示的是拆分为两个数的情况，而dp[i-j]*j则表示拆分为3个以及上 
        // 可以明确地是dp[i-j]也包含了{dp[i-j-1]*[j-1]等过程，即把dp[j]给包含了其实}
        vector<int> dp(n+1,0); // 从0到n
        dp[2] = 1; // dp[0]dp[1]都初始化为0 符合逻辑
        for(int i=3;i<=n;i++){
            for(int j=1;j<=i/2;j++){
                // j=n-1 和j=1的结果其实一致 故而可用j<n-1
                // 进一步的可以明确地是  拆分一个数n 使之乘积最大，那么一定是拆分成m个近似相同的子数相乘才是最大的
                // 故而应该是i/2
                dp[i] = max(dp[i],max((i-j)*j,dp[i-j]*j));
            }
        }
        // 初始化 递推公式 迭代顺序 ！！！
        return dp[n];
    }
    // 96 不同的二叉搜索树 二叉搜索树是有数值的了，二叉搜索树是一个有序树。
    // 若它的左子树不空，则左子树上所有结点的值均小于它的根结点的值；若它的右子树不空，则右子树上所有结点的值均大于它的根结点的值；
    // 它的左、右子树也分别为二叉排序树。 按中序读取二叉搜索树，获得结果是对应的递增的！！
    // 首先这道题想到用动规的方法来解决，就不太好想，需要举例，画图，分析，才能找到递推的关系。
    int numTrees(int n) {
        // 也是一维dp 一路往上 在依次递推的过程中可以发现 dp[3] = dp[2] * dp[0] + dp[1] * dp[1] + dp[0] * dp[2]
        // 我们只需要数量 不需要知道具体的值 故而依次以1 2 3节点 则可以发现是 左2右0 左2右1 左0右2 
        // 递推公式：dp[i] += dp[j - 1] * dp[i - j]; ，j-1 为j为头结点左子树节点数量，i-j 为以j为头结点右子树节点数量
        vector<int> dp(n+1,0);
        dp[0] = 1;
        // dp[1] = 1;
        for(int i=1;i<=n;i++){
            for(int j=1;j<=i;j++){
                // 不同的递推公式 对应这不同的起始条件！！
                dp[i] += dp[j-1]*dp[i-j];
            }
        }
        return dp[n];
    }
    // 2024.07.09 01背包理论基础 !!! 较为重要！！
    // 01背包 -- 完全背包
    // 有n件物品和一个最多能背重量为w 的背包。第i件物品的重量是weight[i]，得到的价值是value[i] 。
    // 每件物品只能用一次，求解将哪些物品装入背包里物品价值总和最大。
    /*
    二维DP数组01背包：
    1.确定dp数组以及下标的含义：dp[i][j]：考虑第i种物品，背包容量为j时，最多能装多少价值。
        i表示从下标为0到i的物品里任意取，放进容量为j的背包，这是一个递进的过程。
        一个完整的表，从而确保每种内容都能够被考虑到
    2.确定递推公式：
        dp[i][j] = max(dp[i-1][j],dp[i-1][j-weight[i]]+value[i])
        1)不放物品i，那么dp[i][j] = dp[i-1][j]，一方面可能是因为现有的容量本身放不下物品，
          另一方面是因为可能不放该物品在后续能够获得更好的效益（一个遍历的过程）
        2）放物品i，那么dp[i][j] = dp[i-1][j-weight[i]]+value[i]，这是因为背包容量j可以放下物品i，
          那么放下i后，容量会减少为j-weight[i],故而j由j-weight[i]；
        而针对于每个物体考虑与否，我们取两者的最大值，即dp[i][j] = max(dp[i-1][j],dp[i-1][j-weight[i]]+value[i])
    3.确定初始值：
        dp[i][j] 是由左上方数值推导出来了 i从1开始，故而dp[0]需要提前初始化
        初始化主要体现在初始化dp[0][j]，即当物品为0时，容量为j时，最多能装多少价值。
    4.遍历顺序：物品和背包重量：
        虽然两个for循环遍历的次序不同，但是dp[i][j]所需要的数据就是左上角，根本不影响dp[i][j]公式的推导！
    5.举例推导dp数组数值：
    */
    void bag01_dp2(int n,int bagweight){
        // n 表示物品个数，bagweight表示背包容量
        vector<int> weight(n,0);  // 存储每件物品所占空间
        vector<int> value(n,0);   // 存储每件物品的价值
        cout << "Enter n weight: ";
        for(int i=0;i<n;i++){
            cin >> weight[i];
        }
        cout << "Enter n value: ";
        for(int j=0;j<n;j++){
            cin >> value[j];
        }
        // dp数组 初始化为全0
        vector<vector<int>> dp(weight.size(),vector<int>(bagweight+1,0));
        // 初始化dp[0] ， 因为其他初始化都是从左上角过来的，而对应的dp[0]无上 [i-1]
        // j<weight[0]时 说明放不下，则价值仍然时0
        // j>weight[0]时，说明放得下，则初始化为value[0] 
        for(int j=weight[0];j<=bagweight;j++){
            dp[0][j]=value[0];
        }
        // 遍历 先物品后重量 n个物品 从0到n-1；bagweight+1个重量类型，从0到bagweight
        for(int i=1;i<n;i++){
            for(int j=0;j<=bagweight;j++){
                if(j<weight[i]) {dp[i][j] = dp[i-1][j];} // 装不下，直接等于上一个
                // 如果能装下,就将值更新为 不装这个物品的最大值 和 装这个物品的最大值 中的 最大值
                // 装这个物品的最大值由容量为j - weight[i]的包任意放入序号为[0, i - 1]的最大值 + 该物品的价值构成
                else {dp[i][j]=std::max(dp[i-1][j],dp[i-1][j-weight[i]]+value[i]);}
            }
        }
        cout << "Enter max value: ";
        cout<<dp[weight.size()-1][bagweight]<<endl;
    }
    // 2024.07.10 01背包理论基础2
    /*
    1.01背包的一维DP，dp[j]表示的是重量为j的背包所含的价值
    1）难点在于遍历顺序的理解
    2）dp[j] = max(dp[j],dp[j-weight[i]]+value[i])
    倒序遍历的原因是，本质上还是一个对二维数组的遍历，并且右下角的值依赖上一层左上角的值，
    因此需要保证左边的值仍然是上一层的，从右向左覆盖。
    如果是正序遍历的话，则j前面的值刚刚被添加了i，后面又会添加i；同一个i被放进去两次；
    但i应该由上一层i-1推导而来，故而倒序的话此时的ij还是由上一层的i-1，j推导而来的
    */
    void bag01_dp1(int n,int bagweight){
        vector<int> weight(n,0);
        vector<int> value(n,0);
        for(int i=0;i<n;i++){
            cin >> weight[i];
        }
        for(int j=0;j<n;j++){
            cin >> value[j];
        }
        // dp都可以初始化为0
        vector<int> dp(bagweight+1,0);
        for(int i=0;i<n;i++){
            // 内层倒叙遍历循环，同时j的下限为weight[i]表明现有的j还能放进weight[i]，则可靠
            for(int j=bagweight;j>=weight[i];j--){
                dp[j]=std::max(dp[j],dp[j-weight[i]]+value[i]);
            }
        }
        cout<<dp[bagweight]<<endl;
    }
    // leetcode 416 0710 Partition Equal Subset Sum
    bool canPartition(vector<int>& nums) {
        // 1. 01背包问题，主要理解为dp[j]<=j；当dp[target]==target时，说明找到对应的分割集合
        int sum = 0;
        for(int i=0; i<nums.size(); i++){
            sum += nums[i];
        }
        // 1.dp 数组 20000/2+1 = 10001
        vector<int> dp(10001);
        int target = 0;
        if(sum%2){
            return false;
        }
        else{
            target = sum/2; 
        }
        for(int i=0;i<nums.size();i++){
            for(int j=target;j>=nums[i];j--){
                dp[j] = max(dp[j],dp[j-nums[i]]+nums[i]);
            }
        }
        if(dp[target]==target){
            return true;
        }
        else{
            return false;
        }
    }
    // leetcode 1049 0710 last Stone Weight II
    int lastStoneWeightII(vector<int>& stones) {
        // 最多只会剩下一块石头，返回此石头的最小的可能重量
        // 剩下的石头最小，那么如果每次都是相差较近的进行碰撞，最终剩下的则会是较小的。
        // 也就是说尽可能的分成重量相似的两堆，
        /*
        为什么可以转01的原因： 整个题目，每个回合数两两抽出来比较，两个数之差将被再一次扔到数组里面，继续上面的过程。
        每个回合都会丢失掉两个数字，加入一个新的数字，这个数字就是两个数的差。相当于来说，就是少了a和b，但是多了一个a-b，a,b就此消失，
        但是在下一回合，a-b可能又被抓出去pk，pk后a-b就此再消失了，又产生了新的一个差。那么每一步来说，其实就相当于a,b没有真正意义消失。
        到了最后一回合，我们可以知道，其实找出两个最接近的数字堆。
        再举个例子：[31,26,33,21,40] 1：40-21 [19,26,31,33] 2: 31-(40-21) [12,26,33] 3: 33-(31-(40-21)) [21,26] 4: 26-(33-(31-(40-21))) [5]
        总： （26+31+21） - （40+33） 这就是找出两个总和接近的两个堆。 如何让两个堆接近呢？ 那就是沿着中间分两半，找左右各自那一半，那么思路就来到了找出一半堆这里。那么就自然而然地来到取不取的问题，就是01背包问题。
        */
        // 416 相当于是求背包是否正好装满，而本题是求背包最多能装多少
        vector<int> dp(15001, 0);
        int sum = 0;
        for (int i = 0; i < stones.size(); i++) sum += stones[i];
        int target = sum / 2;
        for (int i = 0; i < stones.size(); i++) { // 遍历物品
            for (int j = target; j >= stones[i]; j--) { // 遍历背包
                dp[j] = max(dp[j], dp[j - stones[i]] + stones[i]);
            }
        }
        // 相撞之后剩下的最小石头重量就是 (sum - dp[target]) - dp[target]。
        return sum - dp[target] - dp[target];
    }
    // leetcode 494 0711 
    int findTargetSumWays(vector<int>& nums, int target) {
        
    }

};
int main(){
    DP_Solution s;
    int n, bagweight;
    cout << "Enter n and bagweight: ";
    while(cin>>n>>bagweight){
        s.bag01_dp2(n,bagweight);
    }
    return 0;
}

