#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using namespace std;

struct TreeNode{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0),left(nullptr),right(nullptr){}
    TreeNode(int x):val(x),left(nullptr),right(nullptr){}
    TreeNode(int x,TreeNode *left,TreeNode *right):val(x),left(left),right(right){}
};


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
    // leetcode 494 0711 -》 0730 ！！ 真的必须每天刷题 
    // 分析即left组合-right组合 = target； 而left + right = sum; ==>left =(target+sum)/2
    // 首先尝试使用回溯法，即需要在数组集合里取出部分值，从而使得其值的总和为left;
    // 回溯主要分为及部分，第一结束条件，第二循环的起始点或剪枝，第三答案的逐步添加
    vector<vector<int>> targetsum_result;
    vector<int> targetsum_path;
    void TargetSumbacktracing(vector<int> & nums, int left_target,int sum, int startindex){
        // 输入的参数需要包含对应的已有累加值以及该次循环的起始值
        if(sum == left_target){
            targetsum_result.push_back(targetsum_path);
        }
        for(int i=startindex;i<nums.size()&&sum+nums.at(i)<=left_target;i++){
            sum += nums.at(i);
            targetsum_path.push_back(nums[i]);
            // 继续进行下一步的迭代 ==> 基于当前的i进一步的往下
            TargetSumbacktracing(nums,left_target,sum,i+1);
            sum -= nums.at(i);
            targetsum_path.pop_back();
        }
    }
    int findTargetSumWays_backtracing(vector<int>&nums,int target){
        int sum=0 ;
        for(int i=0;i<nums.size();i++){sum+=nums[i];}
        if(target>sum){return 0;}
        if((target+sum)%2){return 0;}
        int left_sum = (target+sum)/2;
        targetsum_result.clear();
        targetsum_path.clear();
        sort(nums.begin(),nums.end());
        // 排序后可以进一步的剪枝 
        TargetSumbacktracing(nums,left_sum,0,0);
        return targetsum_result.size();
    }
    // 回溯法耗时严重 动态规划可以转换为背包问题 但背包问题里的组合数量如何获取
    // 01背包问题 装满容量为x的背包，有几种方法。
    // 设置dp[j]为装满容量为j的背包，有几种方法；那么对应的递推关系则是相应的dp[j]+=dp[j-nums[i]];
    // 因为dp[j-nums[i]]所对应的数量只要再加一个nums{i}就能同时满足要求，故而次数需要同时累加 
    // 还有一个重要的是对应的循环遍历的顺序，外循环对应的是nums[i]，而内循环对应的是dp[j],dp[j]需要倒序去进行遍历
    // 因为本质上dp[j]基于的是上一层的dp[j]，即从后更新则能满足该要求
    int findTargetSumWays(vector<int>& nums, int target) {
        int sum=0;
        for(int i=0;i<nums.size();i++){sum+=nums.at(i);}
        if(abs(target)>sum) return 0;
        if((target+sum)%2) {return 0;}
        int left_target = (target+sum)/2;
        vector<int> dp(left_target+1,0);
        dp[0]=1; // 初始化得为1，因为递推得条件基于该起，如果不是1，则后续一直是0；
        for(int i=0;i<nums.size();i++){
            for(int j=left_target;j>= nums[i];j--){
                // j >= nums[i] 是为了确保在当前背包容量 j 下，可以加入当前数字 nums[i]。
                // 如果 j < nums[i]，则当前数字 nums[i] 无法放入容量为 j 的背包中。
                dp[j] += dp[j-nums[i]]; // ==》 j-nums[i]>=0 !!
            }
        }
        return dp[left_target];
    }
    // leetcode 474 一和零 0730
    /*
    基于回溯的原则进行设计分析 ==》 
    从里面挑选x个元素，满足不超过m和n的限制，同时x尽可能的大
    本题中strs 数组里的元素就是物品，每个物品都是一个！
    而m 和 n相当于是一个背包，两个维度的背包。
    dp[i][j]：最多有i个0和j个1的strs的最大子集的大小为dp[i][j]。
    dp[i][j] 可以由前一个strs里的字符串推导出来，strs里的字符串有zeroNum个0，oneNum个1
    dp[i][j] 就可以是 dp[i - zeroNum][j - oneNum] + 1。
    然后我们在遍历的过程中，取dp[i][j]的最大值。
    递推公式：dp[i][j] = max(dp[i][j], dp[i - zeroNum][j - oneNum] + 1);
    
    */ 
    int findMaxForm(vector<string>& strs, int m, int n) {
        vector<vector<int>> dp(m+1,vector<int>(n+1,0));
        for(string str: strs){
            // 统计计算对应的字符数量
            int oneNum = 0,zeroNum =0;
            for(char c:str){
                if(c=='0'){
                    zeroNum++;
                }
                else{
                    oneNum++;
                }
            }
            for(int i=m;i>=zeroNum;i--){
                for(int j=n;j>=oneNum;j--){
                    dp[i][j] = max(dp[i][j],dp[i-zeroNum][j-oneNum]+1);     
                }
            }
        }
        return dp[m][n];        
    }
    // 完全背包理论基础 2024.07.31!!
    // 每件物品都有无限个，完全背包的物品可以添加多次，故而内循环采用从小到大的遍历顺序
    // 为什么遍历物品在外层循环，遍历背包容量在内层循环？
    // 对于纯完全背包，对于一维dp数组来说，其实两个for循环嵌套顺序是无所谓的！
    // 但是对应的一些变种，对于遍历顺序还是会有要求的
    void test_CompletePack(){
        int N,V;
        cin>>N>>V;
        vector<int> weight;
        vector<int> value;
        for(int i=0;i<N;i++){
            int w;
            int v;
            cin>>w>>v;
            weight.push_back(w);
            value.push_back(v);
        }
        vector<int> dp(V+1,0);
        for(int i=0;i<N;i++){
            for(int j=weight[i];j<=V;j++){
                // 需要有足够的空间去装这个背包
                dp[j]=max(dp[j],dp[j-weight[i]+value[i]]);
            }
        }
    }
    // leetcode 518 零钱兑换II
    // ==》 完全背包问题，将物品刚好放进去有多少中可能
    // dp[j] 代表为j时有多少种可能 dp[j] += dp[j-nums[i]]
    // 组合不强调元素之间的顺序，排序强调元素之间的顺序
    // 故而对于的循环顺序是物品在外，确保不会出现{1，5}和{5，1}同时存在，因为前面加了1之后，后面就不会重复加，只会出现在最前面
    // 而如果是物品在内，则{1，5}和{5，1}则可能在不同背包容量下被不同的选择；就是排列数了
    // dp[0]一定要为1，dp[0] = 1是 递归公式的基础。如果dp[0] = 0 的话，后面所有推导出来的值都是0了。
    // dp[2]=dp[0]=1 ==>>dp[1],dp[3]=0;
    int change(int amount, vector<int>& coins) {
        vector<int> dp(amount+1,0);
        dp[0] = 1;
        for(int i=0;i<coins.size();i++){ // 遍历物品
            for(int j=coins[i];j<=amount;j++){   // 遍历背包
                dp[j] += dp[j-coins[i]]; // 递推
            }
        }
        return dp[amount];
    }
    // 377 组合总和IV；
    // 给定由正整数组成而且存在不重复数字的数组，找出和为给定目标正整数的组合的个数
    // 完全背包问题，求排列
    int combinationSum4(vector<int>& nums, int target) {
        vector<int> dp(target+1,0);
        dp[0] = 1;
        for(int j=0;j<=target;j++){
            for(int i=0;i<nums.size();i++){
                // 需要注意的是测试用例里可能有这两个数相加超过int的存在==>需要避免这种
                if(j>=nums[i] && dp[j]<INT_MAX-dp[j-nums[i]]) dp[j] +=dp[j-nums[i]];
            }
        }
        return dp[target];
    }
    // 322. 零钱兑换 最小的组合数 
    // 定义dp[i]为凑成总金额最小需要的个数 dp[i] = min{dp[i],dp[i-nums[i]]+1}; dp[0]=1
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount+1,INT_MAX);
        // 此处必须设置为INT_MAX 因为里面的每个硬币都是数额无线的，则组合的总数本就没有上限；
        // 设置其他值都有可能会被超过
        dp[0]=0; // 面额硬币没有0，故而凑成0的则为0 ==》由示例进行定义
        for(int i=0;i<coins.size();i++){
            for(int j=coins[i];j<=amount;j++){
                if (dp[j - coins[i]] != INT_MAX) { // 如果dp[j - coins[i]]是初始值则跳过
                // 初始值设置的特别大，则不可能会产生该值  ==》 避免INT-MAX+1 产生越界的操作
                dp[j] = min(dp[j],dp[j-coins[i]]+1);
                }
            }
        }
        if (dp[amount]==INT_MAX) return -1;
        return dp[amount];
    }
    // leetcode 279 完全平方数
    // 完全背包问题，可以转化为由1到n对应的完全平方数，由这些数字去填满结果
    int numSquares(int n){
        vector<int> dp(n+1,INT_MAX);
        dp[0] = 0;
        // 需要注意边界条件，i不能为0，i*i可以等于n，比如1*1=1
        for(int i=1;i*i<=n;i++){
            for(int j=i * i;j<=n;j++){ // 起始值需要大于
                 dp[j] = min(dp[j - i * i] + 1, dp[j]);
            }
        }
        return dp[n];
    }
    // 求组合数：动态规划：518.零钱兑换II (opens new window)
    // 求排列数：动态规划：377. 组合总和 Ⅳ (opens new window)、动态规划：70. 爬楼梯进阶版（完全背包） (opens new window)
    // 求最小数：动态规划：322. 零钱兑换 (opens new window)、动态规划：279.完全平方数
    // leetcode 139 单词拆分 2024.08.03
    /* 
    非空字符串s和一个包含非空单词的列表wordDict，判定s是否可以被空格拆分为1一个或多个在字典中出现的单词
    排列问题，是否存在一个排列其对应的组合可以和s一样；
    完全背包 ==》 不仅字符的个数需要满足，还需要满足匹配的原则，即该字符与字典中的字符的前面几个是一一对应的
    dp[i]；字符串长度为i的话，dp[i]为true，表示可以拆分为一个或多个在字典中出现得单词
    如果确定dp[j] 是true，且 [j, i] 这个区间的子串出现在字典里，那么dp[i]一定是true。（j < i ）。
    所以递推公式是 if([j, i] 这个区间的子串出现在字典里 && dp[j]是true) 那么 dp[i] = true。
    
    */ 
    bool wordBreak(string s,vector<string> &wordDict){
        unordered_set<string> wordSet(wordDict.begin(),wordDict.end());
        vector<bool> dp(s.size()+1,false);
        dp[0] = true;
        for(int i = 1; i<=s.size();i++ ){ // 先遍历背包
            for(int j=0;j<i;j++){ // j需要小于背包的容量！！
                string word = s.substr(j,i-j);
                 // 获取从j开始，i-j个位置的字符串，如果在字典里，则说明dp[i] = dp[i-j]
                 if(wordSet.find(word) != wordSet.end() && dp[j]){
                    dp[i] = true;
                 }
            }
        }
        return dp[s.size()];
    } 
    // 背包问题主要需要集中关注几个内容 2024.08.03
    /*
    1. 背包和物品的遍历顺序，以及各自遍历顺序的正序还是倒序
    2. 遍历时的起点和终点，同时需要关注背包容量与物品之间的关系，需要确保背包的容量大于物品的重量
    3. 背包问题公式的设置，递推顺序，求组合，求排列，求最小数，
    4. 背包问题的遍历起点
    */
    // 多重背包 ==》 在01背包和完全背包之间，每个背包的数量是有限制的
    // 解决方法可以基于01背包进行进一步的衍生，主要解决方法即是在不同物品的循环下再加一个循环，表示的是对应物品数量的循环
    
    // 198，打家劫舍
    /*
    1. dp[i]表示的是考虑下标i（包括i）以内的房屋，最多可以偷窃的金额为dp[i]。
       ==>所以下标为0，如果设置其为代表第一件房屋，则对应的结果为nums[0]；
       而如果设置的是nums.size()+1,则对应的第一个应该是0；
    2. dp[i] = max(dp[i-1],dp[i-2]+nums[i])
    3. 初始状态，可能不止一个：dp[0]=0, dp[1] = nums[0]
    3. 遍历顺序：从小到大，
    */
    int rob_101(vector<int> &nums){
        if (nums.size() == 0) return 0;
        if (nums.size() == 1) return nums[0];
        vector<int> dp(nums.size()+1);
        dp[0] = 0;
        dp[1] = nums[0];
        for(int i=2;i<=nums.size();i++){
            dp[i] = max(dp[i-1],dp[i-2]+nums[i-1]);
        }
        return dp[nums.size()];
    }
    int rob_102(vector<int>& nums) {
        if (nums.size() == 0) return 0;
        if (nums.size() == 1) return nums[0];
        vector<int> dp(nums.size()); // 从0开始的！！
        dp[0] = nums[0];
        dp[1] = max(nums[0], nums[1]); // 针对于第一个则是在0和1之间进行取舍
        for (int i = 2; i < nums.size(); i++) {
            dp[i] = max(dp[i - 2] + nums[i], dp[i - 1]);
        }
        return dp[nums.size() - 1];
    }
    // leetcode 213 打家劫舍II 2024.08.03
    // 由线性的变成一圈了！！即首尾也是相邻的，则需要剔除该种情况
    // 对应的即可在两种情况下(不包含首元素，不包含尾元素)取max 
    int rob_2(vector<int> &nums){
        if(nums.size() == 0) return 0;
        if(nums.size() == 1) return nums[0];
        // end -1[包含尾元素]/-2[不包含尾元素]
        int result1 = robrange(nums,0,nums.size()-2); // 情况二
        int result2 = robrange(nums,1,nums.size()-1); // 情况三
        return max(result1,result2);
    }
    int robrange(vector<int> &nums,int start,int end){
        if(end == start) return nums[end];
        vector<int> dp(nums.size());
        // 重新定义起点
        dp[start] = nums[start];
        dp[start+1] = max(nums[start],nums[start+1]);
        for(int i=start+2;i<=end;i++){
            // 重新定义遍历千点和终点
            dp[i] = max(dp[i-1],dp[i-2]+nums[i]);
        }
        return dp[end];
    }
    // leetcode 337 打家劫舍III 2024.08.03
    /*
    动态规划仍然是准确的，需要更改和确认的是主要是遍历的顺序；
    父节点和子节点不能够被同时获取，那么可以利用对应的二叉树的顺序公式进行分析；
    对于树的话，首先就要想到遍历方式，前中后序（深度优先搜索）还是层序遍历（广度优先搜索）。
    如果抢了当前节点，两个孩子就不能动，如果没抢当前节点，就可以考虑抢左右孩子（注意这里说的是“考虑”）
    1. 可以暴力递归，直接基于树的结构撰写对应的dp规则，从而获取到对应的结果，但存在重复计算的过程
    2. 记忆化递推, 前文第一种暴力递归存在的问题是冗余计算，因为当我们计算左右孩子时，需要用到左右孩子的左右孩子，
    但这个在之前就已经计算过了，故而可以采用一个map去存储这个结果。
    3. 动态规划设计，
    
    */
    int rob_301(TreeNode* root){
        // 时间复杂度：O(n^2)，我们计算了root的四个孙子（左右孩子的孩子）为头结点的子树的情况，又计算了root的左右孩子为头结点的子树的情况，计算左右孩子的时候其实又把孙子计算了一遍。
        if (root == nullptr) return 0;
        if (root->left == nullptr && root->right == nullptr) return root->val;
        // 偷父节点
        int val1 = root->val;
        if (root->left) val1 += rob_301(root->left->left) + rob_301(root->left->right); // 跳过root->left，相当于不考虑左孩子了
        if (root->right) val1 += rob_301(root->right->left) + rob_301(root->right->right); // 跳过root->right，相当于不考虑右孩子了
        // 不偷父节点
        int val2 = rob_301(root->left) + rob_301(root->right); // 考虑root的左右孩子
        return max(val1, val2);
    }
    // 记忆化递推 ==> 记录中间过程 2024.08.03
    unordered_map<TreeNode*,int> umap; //记录计算过的结果
    int rob_302(TreeNode* root){
        if(root==nullptr) return 0;
        if(root->left == nullptr && root->right == nullptr) return root->val;
        if(umap[root]) return umap[root];
        // 偷父节点
        int val1 = root->val;
        if(root->left) val1 += rob_302(root->left->left) + rob_302(root->left->right);
        if(root->right) val1 += rob_302(root->right->left) + rob_302(root->right->right);
        // 不偷父节点
        int val2 = rob_302(root->left) + rob_302(root->right);
        umap[root] = max(val1,val2);
        return max(val1,val2);
    } 
    // 树形dp ==> 在树上进行状态转移 2024.08.03
    /*
    树形DP，即在树上进行递归公式的推导；需要适当的结合树的遍历方式
    针对于节点而言，只有两个状态，取或不取，故而可以设计一个长度为2的数组，0位代表不取，1位代表取
    结合递归，并采用后序遍历，则可以实现正确的数组的更新与替代   
    */
    vector<int> robTree(TreeNode* cur){
        if(cur==nullptr) return vector<int>{0,0};
        vector<int> left = robTree(cur->left);
        vector<int> right = robTree(cur->right);
        // 不偷该节点 ==> 则左右节点都可以偷，故而纳入对应的考虑范围
        int val1 = max(left[0],left[1])+max(right[0],right[1]);
        // 偷该节点 则不能偷左右节点
        int val2 = cur->val + left[0] + right[0];
        return vector<int>{val1,val2};
    }
    int rob(TreeNode * root){
        vector<int> result = robTree(root);
        return max(result[0],result[1]);
    }
    // 【系列专题学习】买卖股票的最佳时机系列I-II-III-IV-含冷冻期-含手续费-总结
    // leetcode 121 买卖股票的最佳时机 2024.08.04
    // 贪心做法 ==》 因为全程只有一次买卖，而想要获得最佳收益，只需要右边的最大值减去左边的最小值即可
    // 具体做法的话，在遍历过程中不断搜寻左边的最小值，同时用当前值进去最小值，并取max，则可以通过遍历的方式获取对应的max
    int maxProfit_101(vector<int> & prices){
        int min_value = INT_MAX,result = 0;
        for(int i=0;i<prices.size();i++){
            min_value = min(min_value,prices[i]);
            result = max(result,prices[i]-min_value);
        }
        return result;
    }
    // 动态规划的做法==》针对于股票的持有情况进行分析，类似于前文的树形DP，需要记录两个状态01，代表的时持有股票和不持有股票
    /* 2024.08.04
    目标是获得最多的现金，故而都应该是基于max 
    dp[i][0]: 代表在第i天持有股票时的现金，注意，持有股票并不代表时当天买入的股票；其有两种对应的情况
              第i-1天就持有了，则dp[i][0] = dp[i-1][0] 
              第i天买入的，即dp[i][0] = -price[i]
              ==> dp[i][0] = max(dp[i-1][0],-price[i])
    dp[i][1]：代表在第i天不持有股票时的现金，注意，不持有股票也不代表时当天卖出的股票，其也有两种对应的情况
              第i-1天就未持有了，则dp[i][1] = dp[i-1][1]
              第i-1天卖出的，则dp[i][1] = dp[i-1][0] + price[i]
              dp[i][1] = max(dp[i-1][1],dp[i-1][0] + price[i])
    初始化条件，即dp[0][0] = -price[0] ,dp[0][1] = 0
    循环遍历条件即顺序遍历即可，因为对应的i依赖于先前的i-1的结果
    return dp[i][1];不持有股票时获得价值会更大
    */
   int maxProfit_102(vector<int> & prices){
        vector<vector<int>> dp(prices.size(),vector<int>(2));
        dp[0][0] = -prices[0];
        for(int i=0;i<prices.size();i++){
            // 依次递推分析
            dp[i][0] = max(dp[i-1][0],-prices[i]);
            dp[i][1] = max(dp[i-1][1],dp[i-1][0]+prices[i]);
        }
        return dp[prices.size()-1][1];
   }
   // leetcode 122 买卖股票的最佳时机II ==》 可以多次买卖
   /* 2024.08.04
   贪心做法：==》 局部最优可以转化为全局最优；利润同样是可分解的 
   当可以同时买卖多次股票的时候，我们需要尽量选择只有正利润的时刻进行交易，避免负利润的影响
   故而可以首先基于差值计算出正利润，并将其累加，即可获得最终的结果
   */
   int maxProfit_201(vector<int> & prices){
    int result = 0;
    for(int i=1;i<prices.size();i++){
        result += max(prices[i]-prices[i-1],0);
    }
    return result;
   }
   /*
    动态规划做法：
      ==》 主要在于递推公式的变化上；
      当买入股票时，当前时刻的现金需要考虑前期的利润情况，即不再是-prices[i]；而是对应的dp[i-1][1]-prices[i];
      需要将前一时刻不持有股票的利润对应的也加进去
   */
    int maxProfit_202(vector<int> &prices){
        vector<vector<int>> dp(prices.size(),vector<int>(2));
        dp[0][0] = -prices[0];
        for(int i=1;i<prices.size();i++){
            dp[i][0] = max(dp[i-1][0],dp[i-1][1]-prices[i]);
            dp[i][1] = max(dp[i-1][1],dp[i-1][0]+prices[i]);
        }
        return dp[prices.size()-1][1];
    }
    // leetcode 123 买卖股票的最佳时机III ==> 最多可以完成两笔交易
    // ==》 可以买卖一次，可以买卖两次，甚至我可以不买卖！！
    /* 2024.08.04
    只有一次交易的时候，对应的结果是两个状态，而当对应有两笔交易的时候；
    可以同步的进行扩展为四个状态，即dp[i][0],dp[i][1],dp[i][2],dp[i][3]
    分别表示的是第一次持有，第一次不持有，第二次持有，第二次不持有的一个状态
    而后可以基于此进行状态转换的递推方程的撰写
    */
    int maxProfit_301(vector<int> &prices){
        vector<vector<int>> dp(prices.size(),vector<int>(4));
        dp[0][0] = -prices[0]; // 第一次持有
        dp[0][1] = 0; // 第一次不持有
        dp[0][2] = -prices[0]; // 第二次持有 =》 在第一次买入卖出的基础上进行分析
        dp[0][3] = 0; // 第二次不持有
        for(int i=1;i<prices.size();i++){
            dp[i][0] = max(dp[i-1][0],-prices[i]); // 第一次买股票，则前面并无累计的金额
            dp[i][1] = max(dp[i-1][1],dp[i-1][0]+prices[i]);
            dp[i][2] = max(dp[i-1][2],dp[i-1][1]-prices[i]);
            dp[i][3] = max(dp[i-1][3],dp[i-1][2]+prices[i]);
        }
        return dp[prices.size()-1][3];
    }
    // leetcode 188 买卖股票的最佳时机IV ==> 最多可以完成k次交易
    /* 2024.08.04
    沿用I-III的思路，从1次交易，扩展到2次交易，继续扩展到k次交易，以及无数次交易
    那么基于k次交易则可以采用与前面类似的方法设计二维数组，即dp[i][j]；其中j则代表买入卖出等的一些状态
    ==》 那么基于先前的参考可以转换为二层for循环的形式去填充数组。
    ==》 而同时为了使得数组的形式一直，引入dp[i][0] =dp[i-1][0] 表示无操作。
    即0(无操作)-1(买入)-2(卖出)-3(买入)-4(卖出)！！
    */
    int maxProfit_401(vector<int> &prices,int k){
        vector<vector<int>> dp(prices.size(),vector<int>(2*k+1));
        // 初始化
        // 奇数代表的是持有，而持有状态下的结果为-prices[0]
        for(int i=0;i<k;i++){
            dp[0][2*i+1] = -prices[0]; // 所有的持有状态都是-prices[0]，主要因为是dp[0]==》还未遍历到后面的i
        }
        // 遍历结构
        for(int j=1;j<prices.size();j++){
            // 注意的是需要确保遍历从1开始 因为dp[j][0]的值是固定的为0
            for(int i=1;i<=k;i++){ 
                // 遍历的范围: 1->k ; 0->k-1 
                // 第2*i+1代表的是持有，则max = 等于前一次持有，与当前此不持有-买出
                dp[j][2*i-1]=max(dp[j-1][2*i-1],dp[j-1][2*i-2]-prices[j]);
                // 第2*i代表的是不持有，则max = 等于前一次不持有，与当前次持有+卖出
                dp[j][2*i] =max(dp[j-1][2*i],dp[j-1][2*i-1]+prices[j]);
                
            }
        }
        return dp[prices.size()-1][2*k];
    }
    // leetcode 309 包含冷冻期 
    /* 2024.08.04
    包含冷冻期的话，应该对应的是买入卖出的递推公式需要往前修改？
    原有的
    dp[i][0] = max(dp[i-1][0],dp[i-1][1]-prices[i]);
    dp[i][1] = max(dp[i-1][1],dp[i-1][0]+prices[i]);
    ==> 需要修改的主要是i-1，即从上一次不持有到这次持有需要隔一天；而从持有到不持有没有限制
    ==> 可能还是存在问题，需要直接将冷冻期也作为一个状态加入，进行设计
    动态规划的本质还是一个复杂的状态转移过程，明确状态设计，明确各个状态之间的转移过程即可设计出合适的代码
    本题状态包含，
    0.股票持有状态（即是前一天就持有，也是可能今天刚买入(也分为两种情况)）
    1.股票不持有状态（即是前一天就不持有，也可能是前一天为冷冻期）
    2.今日售出状态 （即由股票持有转变为不持有）
    3.冷冻期状态 （由今日售出状态转变过来）
    状态转移方程设计：==> 需要前期思考明确！！！
    dp[i][0] = max(dp[i-1][0],dp[i-1][3]-prices[i],dp[i-1][1]-prices[i])
    dp[i][1] = max(dp[i-1][1],dp[i-1][3])
    dp[i][2] = dp[i-1][0] + price[i]
    dp[i][3] = dp[i-1][2]
    ==>初始化设计，股票持有状态为-prices[0]；其他仍然为0
    遍历顺序不变，仍然由左向右
    */
    int maxProfit_withcoldtime(vector<int> &prices){
        int n = prices.size();
        if (n == 0) return 0;
        vector<vector<int>> dp(prices.size(),vector<int>(4));
        dp[0][0] = -prices[0];
        for(int i =1;i<prices.size();i++){
            // max函数主要用于两个内容的比较 
            // 状态的设计很为重要！！
            dp[i][0] = max(dp[i-1][0],max(dp[i-1][3]-prices[i],dp[i-1][1]-prices[i]));
            dp[i][1] = max(dp[i-1][1],dp[i-1][3]);
            dp[i][2] = dp[i-1][0] + prices[i];
            dp[i][3] = dp[i-1][2];
        }
        return max(dp[prices.size()-1][3],max(dp[prices.size()-1][2],dp[prices.size()-1][1]));
    }
    // leetcode 714 买卖股票的最佳时机 含手续费
    /*
    区别就是这里需要多一个减去手续费的操作    
    */
    int maxProfit_with_fee(vector<int>& prices, int fee) {
        int n = prices.size();
        vector<vector<int>> dp(n, vector<int>(2, 0));
        dp[0][0] -= prices[0]; // 持股票
        for (int i = 1; i < n; i++) {
            dp[i][0] = max(dp[i - 1][0], dp[i - 1][1] - prices[i]);
            dp[i][1] = max(dp[i - 1][1], dp[i - 1][0] + prices[i] - fee);
        }
        return max(dp[n - 1][0], dp[n - 1][1]);
    }
    /*
    股票问题总结篇：
    从买卖一次到买卖多次，从最多买卖两次到最多买卖k次，
    从冷冻期再到手续费，
    */
   // 子序列问题分析 41-44 2024.08.05
   /*
   1）子序列问题是动态规划解决的经典问题，当前下标i的递增子序列长度，其实和i之前的下表j的子序列长度有关系
   2）dp[i]表示i之前包括i的以nums[i] 结尾 的最长递增子序列的长度 【注意此处强调结尾的元素】
   3) 不连续递增子序列的跟前0-i 个状态有关，连续递增的子序列只跟前一个状态有关
   
   
   */
    // leetcode 300 最长递增子序列
    /*2024.08.05 
    1. dp[i]表示i之前包括i的以nums[i] 结尾 的最长递增子序列的长度
    2. 当nums[i]>nums[j]时，dp[i] = max(dp[i],dp[j]+1),其中j是从0到i-1，
        即当前的i元素比之前以j结尾的元素更大，那么就可以基于其+1；
        不同的以j结尾反映了这过程中可能的元素突然增大的情况
    3. 元素的初始化都是1，因为以自己为对应的单个元素的递增情况
    */
    int lengthOfLIS(vector<int>& nums) {
        vector<int> dp(nums.size(),1);
        for(int i=1;i<nums.size();i++){
            for(int j=0;j<i;j++){
                if(nums[i]>nums[j]){
                    dp[i] = max(dp[i],dp[j]+1);
                }
            }
        }
        int result = 0;
        for(int i=0;i<dp.size();i++){
            result = max(result,dp[i]);
        }
        return result;
    }
    // leetcode 674. 最长连续递增序列 
    // 2024.08.05
    // 
    int findLengthOfLCIS(vector<int>& nums) {
        vector<int> dp(nums.size(),1);
        int result=dp[0];
        for(int i=1;i<nums.size();i++){
            if(nums[i]>nums[i-1]) {dp[i]=dp[i-1]+1;}
            else{dp[i]=1;}
            result = max(result,dp[i]);
        }
        return result;
    }
    // leetcode 718. 最长重复子数组
    /* 2024.08.05
    需要完全相同的，不能拆分
    给两个整数数组 A 和 B ，返回两个数组中公共的、长度最长的子数组的长度。
    1）用二维数组可以记录两个字符串的所有比较情况
    2）dp[i][j]表示以i和j为结尾的两个字符串的比较情况
    3）dp[i][j] = dp[i-1][j-1]+1 (if nums[i]=nums[j])
    4) 初始化需要针对于dp[0][j],dp[i][0]进行专门的分析
    5）最终的结果取整个dp表格的max，当可以把这个融入到遍历当中，这需要遍历的顺序从（0，0）开始
        因为比较需要从该处开始
    */
    int findLength(vector<int>& nums1, vector<int>& nums2) {
        vector<vector<int>> dp(nums1.size(),vector<int>(nums2.size()));
        // inital
        for(int i=0;i<nums1.size();i++){
            if(nums1[i]==nums2[0]){dp[i][0]=1;}
        }
        for(int j=0;j<nums2.size();j++){
            if(nums2[j]==nums1[0]){dp[0][j]=1;}
        }
        int result = 0;
        for(int i=0;i<nums1.size();i++){
            for(int j=0;j<nums2.size();j++){
                if(nums1[i]==nums2[j]&&i>0&&j>0){
                    dp[i][j] = dp[i-1][j-1]+1;
                }
                if(dp[i][j]>result){result = dp[i][j];}
            }
        }
        return result;   
    }
    // leetcode 1143.最长公共子序列
    /* 2024.08.05
    给定两个字符串 text1 和 text2，返回这两个字符串的最长公共子序列的长度。
    一个字符串的 子序列 是指这样一个新的字符串：
    它是由原字符串在不改变字符的相对顺序的情况下删除某些字符（也可以不删除任何字符）后组成的新字符串。
    ==》子序列和子数组的区别之一，即其可以不连续！！
    递推
    */
    int longestCommonSubsequence_complex(string text1, string text2) {
        vector<vector<int>> dp(text1.size(),vector<int>(text2.size()));
        // inital
        for(int i=0;i<text1.size();i++){
            if(text1[i]==text2[0]){dp[i][0]=1;}
        }
        for(int j=0;j<text2.size();j++){
            if(text2[j]==text1[0]){dp[0][j]=1;}
        }
        for(int i=0;i<text1.size();i++){
            for(int j=0;j<text2.size();j++){
                if(text1[i]==text2[j]&&i>0&&j>0){
                    dp[i][j] = dp[i-1][j-1]+1;
                }
                else{
                    // 遍历需要从0开始，因为再推导的时候仍然需要用到基于0的
                    // 那么在max的过程中，会出现i-1或者j-1为零的情况，故而需要谨慎处理该边界情况
                    if(i>0){
                        dp[i][j]=max(dp[i][j],dp[i-1][j]);
                    }
                    if(j>0){
                        dp[i][j]=max(dp[i][j],dp[i][j-1]);
                    }
                    // 不需要连续的情况下，当不相同的时候则不是直接赋予0，而是继承先前的结果
                }
            }
        }
        return dp[text1.size()-1][text2.size()-1];  
    } 
    // ==》优化版本 现在的边界处理情况很复杂  2024.08.05
    // 主要体现在因为i-1和j-1的存在，需要处理负数的情况，那么可以基于此横纵向都多添加一列
    int longestCommonSubsequence(string text1, string text2) {
        vector<vector<int>> dp(text1.size()+1,vector<int>(text2.size()+1));
        for(int i=1;i<=text1.size();i++){
            for(int j=1;j<=text2.size();j++){
                // 多了一个全0的，而后从1开始，则避免了dp里面负数的情况
                // dp[i][j]代表的是以i-1和j-1结尾的两个字符的匹配情况
                if(text1[i-1]==text2[j-1]){dp[i][j]=dp[i-1][j-1]+1;}
                else{dp[i][j]=max(dp[i-1][j],dp[i][j-1]);}
            }
        }
        return dp[text1.size()][text2.size()];
    }
    // leetcode 1035 不相交的线 2024.08.06
    /*
    ==》其实可以转换为两个数组的最长子序列
    dp[i][j] 表示以i-1结尾的序列A和以j-1结尾的序列B的最长子序列的长度
    */
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {
        vector<vector<int>> dp(nums1.size()+1,vector<int>(nums2.size()+1));
        for(int i=1;i<=nums1.size();i++){
            for(int j=1;j<=nums2.size();j++){
                if(nums1[i-1]==nums2[j-1]){dp[i][j]=dp[i-1][j-1]+1;}
                else{
                    dp[i][j]=max(dp[i][j-1],dp[i-1][j]);
                }
            }
        }
        return dp[nums1.size()][nums2.size()];
    }      
    // leetcode 53. 最大子序和 2024.08.06
    /*
        1.仍然以dp[i]表示以i为结尾的最大子序和
        2.dp[i]=max(dp[i-1]+nums[i],nums[i])
        ==> 以i-1为结尾的元素代表的是累计的加和，如果其为正，则继续累加；如果为负，则重新开始累加即nums[i]
    */
    int maxSubArray(vector<int>& nums) {
        if (nums.size() == 0) return 0;
        vector<int> dp(nums.size());
        dp[0] = nums[0];
        int result = dp[0];
        for(int i =1;i<nums.size();i++){
            dp.at(i) = max(dp.at(i-1)+nums.at(i),nums.at(i));
            result = max(result,dp.at(i));
        }
        return result;
    }
    // 编辑距离系列分析！！
    // leetcode 392 判断子序列 2024.08.06
    // dp[i][j] 表示以下标i-1为结尾的字符串s，和以下标j-1为结尾的字符串t，相同子序列的长度为dp[i][j]。
    bool isSubsequence(string s, string t) {
        // 基于最长公共子序列即可完成，但应该可以进一步的简化
        vector<vector<int>> dp(s.size() + 1, vector<int>(t.size() + 1, 0));
        for (int i = 1; i <= s.size(); i++) {
            for (int j = 1; j <= t.size(); j++) {
                if (s[i - 1] == t[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
                else dp[i][j] = dp[i][j - 1];
                // 与最长公共子序列的区别在这里，即i是s的，不用考虑i-1；因为主要是在t里找s；
            }
        }
        if (dp[s.size()][t.size()] == s.size()) return true;
        return false;
    }
    /*
    leetcode 72: 编辑距离： 2024.08.06
    给你两个单词 word1 和 word2，请你计算出将 word1 转换成 word2 所使用的最少操作数 。
    你可以对一个单词进行如下三种操作：插入,删除,替换;
    dp[i,j]表示以word1下以i-1结尾和word2下以j-1结尾的最短编辑距离；
    分为多种情况；
    如果word1[i-1] == word2[j-1] 的话； 则说明他们不需要操作，即距离为0
    dp[i][j] = dp[i-1][j-1];
    如果word1[i-1] ！= word2[j-1] 的话，也分多种情况：
    1. 他们确实不相等，采用替换操作 dp[i][j] = dp[i-1][j-1]+1;
    2. 即word2的第j个字符应该与word1的前i-1个进行匹配：说明x多写，或y漏写
        dp[i][j] = dp[i-1][j] + 1(x,_)
    3. 即word2的前j-1个字符与word1的第i个字符匹配，则说明x漏写，或y多写
        dp[i][j] = dp[i][j-1] + 1(_,y)
    而后应该取三种操作里的最小值！！
    ==》初始化：
    dp[i][0] ：以下标i-1为结尾的字符串word1，和空字符串word2，最近编辑距离为dp[i][0]。
    那么dp[i][0]就应该是i，对word1里的元素全部做删除操作，即：dp[i][0] = i;
    */
    int minDistance(string word1, string word2) {
        if(word1.size()==0 || word2.size() == 0){return max(word1.size(),word2.size());}
        vector<vector<int>> dp(word1.size()+1,vector<int>(word2.size()+1));
        for (int i = 0; i <= word1.size(); i++) dp[i][0] = i;
        for (int j = 0; j <= word2.size(); j++) dp[0][j] = j;
        for(int i=1;i<=word1.size();i++){
            for(int j=1;j<=word2.size();j++){
                if(word1[i-1]==word2[j-1]){dp[i][j]=dp[i-1][j-1];}
                else{
                    dp[i][j] = min(dp[i-1][j-1]+1,min(dp[i-1][j]+1,dp[i][j-1]+1));
                }
            }
        }
        return dp[word1.size()][word2.size()];
    }
    // leetcode 115 不同的子序列 2024.08.08
    /*
    给定一个字符串 s 和一个字符串 t ，计算在 s 的子序列中 t 出现的个数。
    ==》基于是子序列进行更一步升级，有多少中形成该子序列的方法！！
    dp[i][j]：以i-1为结尾的s子序列中出现以j-1为结尾的t的 个数 为dp[i][j]。
    分析分为两种情况：
    1. s[i-1] == t[j-1]:
        继续分为两种：
        1）用s[i-1]与t[j-1]去匹配，那么相当于序列的延申，故而dp[i][j] = dp[i-1][j-1];
        2）不用s[i-1]与t[j-1]去匹配，即用i-2去匹配，用前面一个的结果；dp[i][j] = dp[i-1][j]
        则dp[i][j] = dp[i-1][j-1]+dp[i-1][j];
    2.s[i-1] != t[j-1]:
        1) 则只能用i-2去与j-1匹配，故而dp[i][j] = dp[i-1][j];
    
    ==> 初始化分析：
    1). dp[i][0],以i-1为末尾的字符包含空字符串，则只能全删除完毕，故而dp[i][0] =1;
    2). dp[0][i],空字符串包含以i-1结尾的字符串，则不可能，故而dp[0][j] = 0;
    3). dp[0][0] = 1;
    */
    int numDistinct(string s, string t) {
        vector<vector<uint64_t>> dp(s.size()+1,vector<uint64_t>(t.size()+1));
        for(int i=0;i<=s.size();i++){dp[i][0]=1;}
        for(int j=1;j<=t.size();j++){dp[0][j]=0;}
        for(int i=1;i<=s.size();i++){
            for(int j=1;j<=t.size();j++){
                if(s.at(i-1)==t.at(j-1)){
                    dp.at(i).at(j) = dp.at(i-1).at(j-1)+dp.at(i-1).at(j);
                }
                else{
                    dp.at(i).at(j) = dp.at(i-1).at(j);
                }
            }
        }
        return dp.at(s.size()).at(t.size());
    }
    // leetcode 583. 两个字符串的删除操作 2024.08.08
    /*
    1. 其实就是先找到最长的公共子序列
    2. 将word1和word2中更长的序列减去最长公共子序列的长度
    dp[i][j] 表示以i-1为结尾的word1和以j-1为结尾的word2的最长公共子序列长度
    当word1[i-1] == word2[j-1] dp[i][j] = dp[i-1][j-1]+1;
    != 时 dp[i][j] = max(dp[i-1][j],dp[i][j-1])
    初始化则全为0；
    */
    int minDistance(string word1, string word2) {
        vector<vector<int>> dp(word1.size()+1,vector<int>(word2.size()+1));
        for(int i=1;i<=word1.size();i++){
            for(int j=1;j<=word2.size();j++){
                if(word1[i-1]==word2[j-1]){dp[i][j]=dp[i-1][j-1]+1;}
                else{
                    dp[i][j] = max(dp[i-1][j],dp[i][j-1]);
                }
            }
        }
        int max_len =  dp[word1.size()][word2.size()];
        return max(word1.size(),word2.size()) - max_len;
    }
    // leetcode 647 回文子串 2024.08.08
    /*
    回文子串的性质在于可以从中间往两边延申；其中对应的dp[i][j]可以由dp[i+1][j-1]推出来
    故而基于该性质推导对应的递推公式
    当s[i] == s[j]时,有多种情况：
    1. i==j 肯定是回文子串 true
    2. i!=j 相差为1，也肯定是
    3. i!=j 相差大于1，则基于dp[i+1][j-1]的值进行分析
    当s[i]!=s[j]:
    则直接是false
    */
    int countSubstrings(string s) {
        vector<vector<bool>> dp(s.size(),vector<bool>(s.size(),false));
        // 初始化结构分析：
        int result;
        for(int i=s.size()-1;i>=0;i--){
            for(int j=i;j<s.size();j++){
                if(s[i]==s[j]){
                    if(j-i<=1){
                        // 刚好对应回文子串的单个和两个的情况
                        dp[i][j] = true;
                        result++;
                    }
                    else{
                        if(dp[i+1][j-1]){
                            dp[i][j] = true;
                            result++;
                        }
                    }
                }
            }
        }
        return result;
    }
    // leetcode 516 最长回文子序列 2024.08.08
    /*
    回文子串是要连续的，回文子序列可不是连续的！
    dp[i][j]：字符串s在[i, j]范围内最长的回文子序列的长度为dp[i][j]。
    在判断回文子串的题目中，关键逻辑就是看s[i]与s[j]是否相同。
    如果s[i]与s[j]相同，那么dp[i][j] = dp[i + 1][j - 1] + 2;
    如果s[i]与s[j]不相同，说明s[i]和s[j]的同时加入 并不能增加[i,j]区间回文子序列的长度，那么分别加入s[i]、s[j]看看哪一个可以组成最长的回文子序列。
    加入s[j]的回文子序列长度为dp[i + 1][j]。
    加入s[i]的回文子序列长度为dp[i][j - 1]。
    那么dp[i][j]一定是取最大的，即：dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
    */
    int longestPalindromeSubseq(string s) {
        vector<vector<int>> dp(s.size(),vector<int>(s.size()));
        for (int i = 0; i < s.size(); i++) dp[i][i] = 1;
        for(int i=s.size()-1;i>=0;i--){
            for(int j=i+1;j<s.size();j++){
                if(s[i]==s[j]){
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                }
                else{
                     dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[0][s.size()-1];
    }
};
int main(){
    DP_Solution s;
    vector<int> input = {10,9,2,5,3,7,101,18};
    int result = s.lengthOfLIS(input);
    cout<<"result"<<result;
    return 0;
}

