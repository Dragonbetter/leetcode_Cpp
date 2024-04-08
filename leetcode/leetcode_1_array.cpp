#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_map>
//In C++17 and later, you can use a comma-separated list in a single using declaration to bring multiple names into scope.
//However, in earlier versions of C++, this is not allowed and each name must be brought into scope with a separate using declaration.
using namespace std;
using std::vector;
using std::cout;
using std::endl;
// binary search function
// int 类型在此处实现为32位的，范围为（-2,147,483,648，2,147,483,647）
class SolutionforArray{
    public:
    int search(vector<int> &nums,int target){
        // 2023.11.28 Leetcode 704
        int left = 0;
        int right = nums.size()-1;
        while(left<=right){
            int mid = left +((right-left)/2);
            if (nums[mid]==target){
                return mid;}
            else if (nums[mid]>target){
                right=mid-1;
            }
            else if (nums[mid]<target){
                left=mid+1;
            }
        }
        //not find target
        return -1;
    }
    int searchInsert(vector<int> &nums,int target){
        //2023.11.29 leetcode 35 Search Insert Position
        /*
        二分查找 如果找到 则返回相应的位置 如果没找到 则依据其大小返回插入的位置 
        */
       int  left=0;
       int right=nums.size()-1;
       while(left<=right){
        int mid = left+((right-left)/2);
        if (nums[mid]==target){
            return mid;
        }
        else if (nums[mid]>target){
            right=mid-1;
        }
        else if (nums[mid]<target){
            left=mid+1;
        }
       }
       return left;
       /*未正确的考虑截止条件 破除循环的情况，循环一般都会进行到left=right的情况，继而进一步的判断
         返回值究竟是low还是high的思考过程： 如果数组中没有target，那么跳出循环前的一步一定是mid=low=high
         (1)如果target>nums[mid],low=mid+1 target的位置在右侧，正好是low
         (2)如果target<nums[mid],high=mid-1 target的位置就是mid这个位置（这里有个隐含条件是target比nums[mid-1]大）。
        if (nums[right]<target){
            return right+1;
        }
       else if(nums[left]>target){
        return left-1;
       }
       */
    }
    vector<int> searchRange_1(vector<int> &nums,int target){
        // leetcode 34. Find First and Last Position of Element in Sorted Array
        // 利用二分法找到中心点后，向左右两边进行扩张，但worst case情况即数据全一样时，向左右两边的延伸复杂度是O（n），不满足O（logn）
        // 另外一种方法是分别寻找左右边界，而后进行判断 
        int left = 0;
        int right =nums.size()-1;
        while (left<=right){
            int mid = left+((right-left)/2);
            if(nums[mid]==target){
                int mid_left=mid,mid_right=mid;
                // 前提条件即需要其相应的left和right在对应的range之内
                while( (mid_left>=0) && nums[mid_left]==target ){
                    --mid_left;
                }
                while((mid_right<=nums.size()-1) && nums[mid_right]==target){
                    ++mid_right;
                }
                vector<int> output_find={mid_left+1,mid_right-1};
                return output_find;
            }
            else if(nums[mid]<target){
                left=mid+1;
            }
            else if(nums[mid]>target){
                right=mid-1;
            } 
        }
        vector<int> output_not_find ={-1,-1};
        return output_not_find;
    }
    vector<int> searchRange_2(vector<int> &nums,int target){
        // 两边同步寻找 左右边界 都用二分 worst-case也是logn
        int leftborder = getleftborder(nums,target);
        int rightborder = getrightborder(nums,target);
        //三种情况，左边界 右边界
        if (leftborder==-2 || rightborder==-2){
           return {-1,-1};
        }
        else if((rightborder-leftborder)>1){
            vector<int> output={leftborder+1,rightborder-1};
            return output;
        }
        return {-1,-1};
    }
    int mySqrt(int x){
       int  left=0;
       int right=x/2+1;
       while(left<=right){
        // 注意此处 mid如果定义成int，此处的x的范围为2^31-1，故而int*int极有可能超过int所能表示的范围，因为其是先以int范围计算再赋值
        long mid = left+((right-left)/2);
        // 或则仍然是int 但改成 x/mid < mid，但相应的需要注意此处的mid不能为0，从1开始
        if ((mid*mid)==x){
            return mid;
        }
        else if ((mid*mid)>x){
            right=mid-1;
        }
        else if ((mid*mid)<x){
            left=mid+1;
        }
       }
       return right; 
    }
    int removeElement_1(vector<int> &nums,int target){
        // 27. Remove Element 返回的只是一个长度 故而快慢指针法 快指针指向遍历元素，两个指针都从前面遍历
        // 慢指针指向真实保存下来的元素，只有当所指的值不等于target时，其才真实更新
        // 同时注意其judge-code需要评测返回引用类型的nums，即原地更改过的，故而nums此处也需要变化
        int slowIndex = 0 ;
        for (int fastIndex = 0;fastIndex<nums.size();fastIndex++){
            if (nums[fastIndex]!=target){
                // fastIndex在前面，故而后面赋值给慢的话逐步覆盖的
                nums[slowIndex++]=nums[fastIndex];
            }
        }
        return slowIndex;
    }
    int removeElement_2(vector<int> &nums,int target){
        // 主要目标 将右边不等于target的值赋予给左边等于target的值 
        // 指标前后一起遍历？第一：右边元素在k之外的元素不管，故而右边等于target的话，可以直接忽略；
        // 寻找右边不等于target的值来覆盖左边等于target的值；多层while循环
        int leftIndex = 0,rightIndex =nums.size()-1;
        // 此处的=是必要的 左右相等时元素仍然未判断
        while(leftIndex<=rightIndex){
            // 寻找左边不等于target的值 
            while(leftIndex<=rightIndex && nums[leftIndex]!=target){
                ++leftIndex;
                // 而且最终leftIndex会指向数组尾部的下一个元素，正好弥补了数组从0开始所带来的的size少1，故而该return是正确的
            }
            // 寻找右边等于target的值
            while(leftIndex<=rightIndex && nums[rightIndex]==target){
                --rightIndex;
            }
            while(leftIndex<rightIndex){
                nums[leftIndex++]=nums[rightIndex--];
            }
        }
        return leftIndex;
    }
    int removeDuplicates(vector<int>& nums){
        int solwIndex =0;
        for(int fastIndex =0;fastIndex<nums.size();++fastIndex){
            if (nums[solwIndex]!=nums[fastIndex]){
                nums[++solwIndex]=nums[fastIndex];
            // 此处因为对应的solwIndex就是取的元素值 index的++在前面 =》需要特殊注意==以及相应的++前后所造成的返回值的不同
            }
        }
        return solwIndex+1;
    }
    vector<int> sortedSquares(vector<int>& nums){
        vector<int> output(nums.size(),0);
        // leetcode 977 12.4双指针首尾倒推 逐个比较取值获取赋值 非迭代器版本 O（n）一遍循环
        int leftIndex= 0,rightIndex=nums.size()-1,i=nums.size()-1;
        while(leftIndex<=rightIndex){
            // int leftvalue = nums[leftIndex]*nums[leftIndex];可以直接写到if与else的语句中
            // int rightvalue = nums[rightIndex]*nums[rightIndex];
            if (nums[leftIndex]*nums[leftIndex]<=nums[rightIndex]*nums[rightIndex]){
                output[i--]=nums[rightIndex]*nums[rightIndex];
                rightIndex--;
            }
            else {
                output[i--]=nums[leftIndex]*nums[leftIndex];
                leftIndex++;
            }
            // i--; 可以直接与output[i]合并为output[i--]
            // memory相差不大，但时间快许多 由28ms-》18ms？
        }
        return output;
    }
    int minSubArrayLen_1(int target,vector<int> &nums){
        // leetcode 209 12.4 滑动窗口法 双指针的一种 主要在于确定起始指针的移动 
        // 而对于尾部指针添加放何处 思考放前面合适 ！
        // 如果有负数的话 是否仍然合适呢？
        int leftIndex=0,sum_array=0,sublength=0;
        int result = target+1;
        for(int rightIndex=0;rightIndex<nums.size();rightIndex++){
            sum_array+=nums[rightIndex];// sum_array放前面
            // 注意这里使用while，每次更新 i（起始位置），并不断比较子序列是否符合条件
            while(sum_array>=target){
                sublength =rightIndex-leftIndex+1;
                result = result<sublength ? result:sublength;
                if (result==1){
                    return 1;
                }
                sum_array -= nums[leftIndex++];
            }
        }
        return result==target+1?0:result;
    }
    int minSubArrayLen_2(int target,vector<int> &nums){
        // 前缀和+二分查找O(nlogn)
        int n=nums.size();
        if (n==0){
            return 0;
        }
        int ans =INT32_MAX;
        vector<int> sums(n+1,0);
         // 为了方便计算，令 size = n + 1 
        // sums[1] = A[0] 前 1 个元素的前缀和为 A[0]
        for (int i = 1; i <= n; i++) {
            sums[i] = sums[i - 1] + nums[i - 1];
        }
        for(int i=1;i<=n;i++){
            int target_s = target+sums[i-1];
            auto bound=std::lower_bound(sums.begin(),sums.end(),target_s);
            // lower_bound 寻找第一个小于或等于给定值对象的下标 如果没有 返回end（）
            if (bound!=sums.end()){
                ans=std::min(ans,static_cast<int>(bound-sums.begin()-(i-1)));
                // static_cast<int> 这是将计算结果强制转换为 int 类型。由于迭代器相减的结果通常是 ptrdiff_t 类型，为了与 ans 的类型一致（也是 int），进行了显式类型转换。
            }
        }
        return ans==INT32_MAX?0:ans;
    }
    int totalFruit_1(vector<int>& fruits) {
        // leetcode 904 Fruit into baskets 12.5
        // 1：取得果树必定是连续的，是一个子序列问题 在这个子序列中 只有两种元素 子序列的长度即是取的水果的数目
        // 需要斟酌的是 用何数据结构来保存这两个元素 如果仅保留元素值 那么在遇到新的元素时 
        // 简单的方法就是将新的保存为新元素以及新元素之前 相应的更新leftindex 并注意最初始情况下的vector构建
        if (fruits.size()==1 || fruits.size()==2){
            return fruits.size();
        }
        int leftIndex = 0,ans=0;
        vector<int> fruit_save;// 新建一个空vector 后续运用push-back和push-front进行操作
        for (int rightIndex=0;rightIndex<fruits.size();rightIndex++){
            if (fruit_save.size()==2){
                if (fruits[rightIndex]!=fruit_save[0] && fruits[rightIndex]!=fruit_save[1]){
                    // 弹出前面的 添加后面的 
                    // 依据前一个元素来判定需要替代的元素值
                    if (fruits[rightIndex-1]==fruit_save[0]){
                        fruit_save[1]=fruits[rightIndex];
                        leftIndex = rightIndex-1;
                        while(fruits[leftIndex]==fruit_save[0]){
                            leftIndex--;
                        }
                        leftIndex++;
                    }
                    else if (fruits[rightIndex-1]==fruit_save[1]){
                        fruit_save[0]=fruits[rightIndex];
                        leftIndex = rightIndex-1;
                         // leftIndex 应该为第一个不等于对应元素的值  
                        while(fruits[leftIndex]==fruit_save[1]){
                            leftIndex--;
                        }
                        // --后++是为了回复原始的值 
                        leftIndex++;
                    }
                   
                }
            }
            else if (fruit_save.size()==1){
                if (fruits[rightIndex]!=fruit_save[0]){
                    fruit_save.push_back(fruits[rightIndex]);
                }
            }
            else{
                fruit_save.push_back(fruits[rightIndex]);
            }
            ans = std::max(ans,rightIndex-leftIndex+1);
        }
        return ans;
    }
    int totalFruit_2(vector<int>& fruits){
        // 12.5采用新的数据结构  unordered_map 用哈希表组织的 map 保存关键键值对 此处键为元素 值为其出现的次数 
        // 在rightindex右行过程中 添加对应键进去 或增加对应值 当超过3时 增加leftindex 并删除对应的键或值 当某个值为0时 删除该键 直到map中只有两个键
        int n = fruits.size();
        unordered_map<int, int> cnt;
        int left=0,ans=0;
        for (int right=0;right<n;right++){
            ++cnt[fruits[right]]; // 提取fruits[right]的计数器并加一 如果没有该元素 则会创建该元素
            while(cnt.size()>2){
                auto it=cnt.find(fruits[left]); // find返回一个迭代器 相应的如果找到 则迭代器指向该关键字 不然指向尾喉迭代器
                --it->second; // 减少该键对应的值 当其为0时 删除
                if (it->second==0){
                    cnt.erase(it);
                }
                ++left; // 递增left 而后再逐步更改map中的值
            }
            ans = max(ans,right-left+1);
        }
        return ans;
    }
    vector<vector<int>> generateMatrix_1(int n) {
        // leetcode 59 59. Spiral Matrix II 12.6
        // 模拟过程 循环不变量 保持准确而又统一的原则 左闭右开 注重边界条件的控制 
        // 需要注意控制 循环的次数 循环的起始点与终点 循环奇偶的区别 
        vector<vector<int>> multi_nums(n, vector<int>(n, 0));  // 多维数组初始化 运用下标引用赋值相关元素
        int loop = n/2; // 定义大循环转动的次数
        int loop_num_each = n-1; // 每边循环的次数 会依据starx进行改变 每次减小2
        // 在while循环内部 依据 左闭右开原则 遍历赋值 
        int startx=0,starty=0; // 全局变量 
        int global_num = 1 ; // 赋值矩阵准备
        while(loop--){
            // 依据startx和starty的值确认有关的旋转上界
            int i_start=startx,j_start=starty;
            // 从左到右
            while(j_start<starty+loop_num_each){
                multi_nums[i_start][j_start++]=global_num++;
            }
            // 从上到下
            while(i_start<starty+loop_num_each){
                multi_nums[i_start++][j_start]=global_num++;
            }
            //从右到左
            while(j_start>starty){
                multi_nums[i_start][j_start--]=global_num++;
            }
            // 从下到上
            while(i_start>startx){
                multi_nums[i_start--][j_start]=global_num++;
            }
            startx++;
            starty++;
            // 两种写法 此处startx每次加一 而loop-num-each每次减2 此处是以每个startx开始的视角进行观察 每条边循环的数目
            // 故而整体等价于 没有-1即 n-offset 每次offset在循环结束后加1 而这个n-0ffset相当于每次的倒数第二个元素 以对角形式递减
            loop_num_each-=2;
        }
        // 依据奇偶分析的 最后一个元素 
        if (n%2){
            //奇数
            multi_nums[startx][starty]=global_num;
        }
        return multi_nums;
    }
    vector<vector<int>> generateMatrix_2(int n) {
        // TODO 取模防止越界 -》每次碰到头，方向向量顺时针旋转九十度， (dx,dy) 变成 (dy, - dx) 
        vector<vector<int>> res(n, vector<int>(n, 0));
        //方向向量,初始向右
        int dx = 0, dy = 1;
         //坐标
        int x = 0, y = 0;
        for (int i = 1; i <= n * n; ++i) {
            res[x][y] = i;
            //这个判断语句最巧妙
            //如果撞头就向右调转90度 先加n再对n取模这个操作能防止越界,省了很多判断语句
            if (res[(x + dx+n) % n][(y + dy+n) % n] != 0) {
                int tmp = dy;
                dy = -dx;
                dx = tmp;
            }
            x += dx;
            y += dy;
        }
        return res;
    }
    /*错误的旋转矩阵 较难分析清楚左右边界旋转的情况
        vector<int> spiralOrder(vector<vector<int>>& matrix) {
        // leetcode 54 Spiral Matrix 12.6 M*N 需要首先比较两者的大小确定循环次数 而后的循环模拟规则保持同样

        int m=matrix.size(),n=matrix[0].size();
        vector<int> output(m*n,0);
        int loop = m>n?n/2:m/2;
        // 针对于loop循环次数的分析 当取的小的是奇数 loop+1
        if (min(m,n)%2){
            loop+=1;
        }
        int max_num = m*n-1;
        int offset=1;
        int startx=0,starty=0;
        int global_num=0;
        while(loop--){
            int i_start=startx,j_start=starty;
            while(j_start<n-offset && (global_num<max_num)){
                output[global_num++]=matrix[i_start][j_start++];
            }
            while(i_start<m-offset  && (global_num<max_num)){
                output[global_num++]=matrix[i_start++][j_start];
            }
            while(j_start>starty  && (global_num<max_num)){
                output[global_num++]=matrix[i_start][j_start--];
            }
            while(i_start>startx  && (global_num<max_num)){
                output[global_num++]=matrix[i_start--][j_start];
            }
            startx++;
            starty++;
            offset++;     
        }
        return output;        
    }
    
    
    */
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        // TODO leetcode 54 Spiral Matrix 12.6 M*N ==》885. Spiral Matrix III Spiral Matrix IV
        if (matrix.empty()) return {};
        int l = 0, r = matrix[0].size() - 1, t = 0, b = matrix.size() - 1;
        vector<int> res;
        /*
        算法流程：
空值处理： 当 matrix 为空时，直接返回空列表 [] 即可。
初始化： 矩阵 左、右、上、下 四个边界 l , r , t , b ，用于打印的结果列表 res 。
循环打印： “从左向右、从上向下、从右向左、从下向上” 四个方向循环打印。
根据边界打印，即将元素按顺序添加至列表 res 尾部。
边界向内收缩 1 （代表已被打印）。
判断边界是否相遇（是否打印完毕），若打印完毕则跳出。
返回值： 返回 res 即可。
        */
        while (true) {
            for (int i = l; i <= r; i++) res.push_back(matrix[t][i]); // left to right
            if (++t > b) break;
            for (int i = t; i <= b; i++) res.push_back(matrix[i][r]); // top to bottom
            if (l > --r) break;
            for (int i = r; i >= l; i--) res.push_back(matrix[b][i]); // right to left
            if (t > --b) break;
            for (int i = b; i >= t; i--) res.push_back(matrix[i][l]); // bottom to top
            if (++l > r) break;
        }
        return res;
    }
private:
    int getrightborder(vector<int> &nums,int target){
        // 右边界是第一个大于target的值-1.由相应的left+1不断产生
        int left= 0;
        int right=nums.size()-1;
        int rightborder =-2;// when not find target
        while(left<=right){
            int mid=left+((right-left)/2);
            if (nums[mid]>target){
                right=mid-1;
            }
            else if(nums[mid]<=target){
                left = mid+1;
                rightborder=left;
            }
        }
        return rightborder;
    }
    int getleftborder(vector<int> &nums,int target){
    // 右边界是第一个大于target的值-1.由相应的left+1不断产生
    int left= 0;
    int right=nums.size()-1;
    int leftborder =-2;// when not find target
    while(left<=right){
        int mid=left+((right-left)/2);
        if (nums[mid]>=target){
            right=mid-1;
            leftborder=right;
        }
        else if(nums[mid]<target){
            left = mid+1;
        }
    }
        return leftborder;
    }
    
};

int main(){
    SolutionforArray s;
    vector<int> nums={1,1,2};
    cout<<s.removeDuplicates(nums)<<endl;
    cout << "Minimum value for int: " << std::numeric_limits<long>::min() << endl;
    cout << "Maximum value for int: " << std::numeric_limits<int>::max() << endl;
    return 0;
}
