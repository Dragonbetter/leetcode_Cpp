#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class recall{
    public:
        std::vector<std::vector<int>> result;
        std::vector<int> path;
        // leetcode 77 组合问题 
        void backtracking_nk(int n,int k,int startindex){
            // 终止条件
            if (path.size()==k){
                result.push_back(path);
                return;
            }
            // 可以剪枝的地方就在递归中每一层的for循环所选择的起始位置。
            // 如果for循环选择的起始位置之后的元素个数 已经不足 我们需要的元素个数了，那么就没有必要搜索了。
            // for(int i=startindex;i<=n;i++){ 优化 
            for(int i=startindex;i<=n-(k-path.size())+1;i++){
                path.push_back(i);
                // 基于该i进行下一层的回溯遍历
                backtracking_nk(n,k,i+1);
                // 撤销操作
                path.pop_back();
            }
        }
        std::vector<std::vector<int>> combine_set(int n ,int k ){
            backtracking_nk(n,k,1);
            return result;
        }
        // leetcode 216 组合优化问题 值相加等于固定值 
        std::vector<std::vector<int>> result_sum;
        std::vector<int> path_sum;
        void tracebacking(int n,int k,int sum,int startindex){
            if (sum > n) { // 剪枝操作
            return; 
            }
            if (path_sum.size()==k) {
            if (sum==n) result_sum.push_back(path_sum);
            return; // 如果path.size() == k 但sum != targetSum 直接返回
            }
            for(int i=startindex;i<=9 - (k - path_sum.size()) + 1;i++)// 剪枝
            {
                path_sum.push_back(i);
                tracebacking(n,k,sum+i,i+1);
                path_sum.pop_back();
            }
        }
        std::vector<std::vector<int>> combinationSum3(int k, int n) {
            tracebacking(n,k,0,1);
            return result_sum;
        }
        // leetcode 17 给定1个数字组合 获取其对应的字母组合结果 前提在于 比如输入2个 那么字符的组合就是2的3 就是3的
        const std::string letterMap[10] = {
            "", // 0
            "", // 1
            "abc", // 2
            "def", // 3
            "ghi", // 4
            "jkl", // 5
            "mno", // 6
            "pqrs", // 7
            "tuv", // 8
            "wxyz", // 9
            };
        std::vector<std::string> result_string;
        std::string s;
        void backtracking(const std::string& digits, int index) {
            if (index == digits.size()) {
                result_string.push_back(s);
                return;
            }
            int digit = digits[index] - '0';        // 将index指向的数字转为int
            std::string letters = letterMap[digit];      // 取数字对应的字符集
            for (int i = 0; i < letters.size(); i++) {
                s.push_back(letters[i]);            // 处理
                backtracking(digits, index + 1);    // 递归，注意index+1，一下层要处理下一个数字了
                s.pop_back();                       // 回溯
            }
        }
        std::vector<std::string> letterCombinations(std::string digits) {
            if (digits.size() == 0) {
                return result_string;
            }
            backtracking(digits, 0);
            return result_string;
        }
        // leetcode 39
        std::vector<std::vector<int>> result_sum_no;
        std::vector<int> path_no;
        void tracebacking_no(vector<int> & candidates,int target,int sum,int startindex){
            if(sum==target){
            result_sum_no.push_back(path_no);
            return ; 
            }
            if(sum>target){
                return;        
                }
            for(int j=startindex;j<candidates.size();j++){
                path_no.push_back(candidates[j]);
                // j可以重复选取 故而只需要在此处startindex选取j即可
                tracebacking_no(candidates,target,sum+candidates[j],j);
                path_no.pop_back();
            }
        }
        vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
            tracebacking_no(candidates,target,0,0);
            return result_sum_no;        
        }
    // leetcode 40 集合（数组candidates）有重复元素，但还不能有重复的组合。
        std::vector<std::vector<int>> result_sum_no;
        std::vector<int> path_no;
        void tracebacking_no(vector<int> & candidates,int target,int sum,int startindex){
            if(sum==target){
            result_sum_no.push_back(path_no);
            return ; 
            }
            if(sum>target){
                return;        
                }
            for(int j=startindex;j<candidates.size() && sum + candidates[j] <= target;j++){
                // 数组里的数字有的是可以重复的，那么需要在逻辑里判断重复点，并去除重复值 如果一直重复，则一直移除
                // 重复的判断借助于当前与前一个元素值的比较，如果相同，继续比较是同一层还是同一树枝，同一树枝的允许重复，同一层的不允许
                // 如何判断同一层？同一层的重复元素其j必然大于起始index，而同一数字，起始index本身也随着增加
                if (j > startindex && candidates[j] == candidates[j - 1]) {
                    continue;
                }
                path_no.push_back(candidates[j]);
                tracebacking_no(candidates,target,sum+candidates[j],j+1);
                path_no.pop_back();
            }
        }
        vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
            // sort
            sort(candidates.begin(),candidates.end());
            tracebacking_no(candidates,target,0,0);
            return result_sum_no;        
        }
        // 
            // pailndrome decide
        bool isPalindrome(const string& s, int start, int end) {
            for (int i = start, j = end; i < j; i++, j--) {
                if (s[i] != s[j]) {
                    return false;
                }
            }
            return true;
        }
        // pailndrome partitioning 131
        /*
        切割问题可以抽象为组合问题
        如何模拟那些切割线
        切割问题中递归如何终止
        在递归循环中如何截取子串
        如何判断回文
        */
        vector<vector<string>> result_pali_part;
        vector<string> result_part;
        void palindrome_part(const string &s,int startindex){
            // when to end? final
            // when to end? final 等于与大于都存在 等于时应该是指前面刚好切割完，还剩余一个的时候；
            if(startindex==s.size()){
                result_pali_part.push_back(result_part);
                return;
            }
            for(int i=startindex;i<s.size();i++){
                if(isPalindrome(s,startindex,i)){
                    // 如果是回文，截取字符串，加入
                    string sub_string = s.substr(startindex,i-startindex+1);
                    result_part.push_back(sub_string);
                }
                else{
                    // 不是回文，则继续++分析
                    continue;
                }
                palindrome_part(s,i+1);
                result_part.pop_back();
            }
        }

        vector<vector<string>> partition(string s) {
            palindrome_part(s,0);
            return result_pali_part;
        }
        //  93. Restore IP Addresses
        bool IP_check(const string &s,int start,int end){
        string sub_IP = s.substr(start,end-start+1);
        // 检查是否具有前导0
        if(sub_IP[0]=='0' && sub_IP.size()>1){
                return false;
            }
            // 检查是否在区间范围内
            if(sub_IP.size()<=3 && stoi(sub_IP)>=0 && stoi(sub_IP)<=255){
                return true;
            }
            return false;
        };
        vector<string> result_IP_final;
        vector<string> result_IP;
        void IP_backtracing(const string &s,int startindex){
            // 终止条件
            if(result_IP.size()==4){
                if(startindex==s.size()){
                    string IP = result_IP[0]+'.'+result_IP[1]+'.'+result_IP[2]+'.'+result_IP[3];
                    result_IP_final.push_back(IP);
                }
                return ;
            };
            // for
            for(int i=startindex;i<s.size();i++){
                // 如此写存在一个问题，即如果前面3个都是1,1,1,那么最终这段会很长，但你仍然是i遍历过去，其实是一直无效
                if(IP_check(s,startindex,i)){
                    string IP_sub = s.substr(startindex,i-startindex+1);
                    // 然后验证一下第四段是否合法，如果合法就加入到结果集里
                    result_IP.push_back(IP_sub);
                }
                else{
                    // 如此写存在一个问题，即如果前面3个都是1,1,1,那么最终这段会很长，但你仍然是i遍历过去，其实是一直无效
                    // continue;  改成break 避免该问题！！
                    break;

                }
                IP_backtracing(s,i+1);
                result_IP.pop_back();
            }
        }
        vector<string> restoreIpAddresses(string s) {
            IP_backtracing(s,0);
            return result_IP_final; 
        }
        // 93 基于point-num的解法 
        vector<string> result_93;// 记录结果
        // startIndex: 搜索的起始位置，pointNum:添加逗点的数量
        void backtracking(string& s, int startIndex, int pointNum) {
            if (pointNum == 3) { // 逗点数量为3时，分隔结束
                // 判断第四段子字符串是否合法，如果合法就放进result中
                if (isValid(s, startIndex, s.size() - 1)) {
                    result_93.push_back(s);
                }
                return;
            }
            for (int i = startIndex; i < s.size(); i++) {
                if (isValid(s, startIndex, i)) { // 判断 [startIndex,i] 这个区间的子串是否合法
                    s.insert(s.begin() + i + 1 , '.');  // 在i的后面插入一个逗点
                    pointNum++;
                    backtracking(s, i + 2, pointNum);   // 插入逗点之后下一个子串的起始位置为i+2
                    pointNum--;                         // 回溯
                    s.erase(s.begin() + i + 1);         // 回溯删掉逗点
                } else break; // 不合法，直接结束本层循环
            }
        }
        // 判断字符串s在左闭又闭区间[start, end]所组成的数字是否合法
        bool isValid(const string& s, int start, int end) {
            if (start > end) {
                return false;
            }
            if (s[start] == '0' && start != end) { // 0开头的数字不合法
                    return false;
            }
            int num = 0;
            // 字符转int型分析 从第一个开始 *10逐渐分析 
            for (int i = start; i <= end; i++) {
                if (s[i] > '9' || s[i] < '0') { // 遇到非数字字符不合法
                    return false;
                }
                num = num * 10 + (s[i] - '0');
                if (num > 255) { // 如果大于255了不合法
                    return false;
                }
            }
            return true;
        }
        vector<string> restoreIpAddresses(string s) {
            result.clear();
            if (s.size() < 4 || s.size() > 12) return result_93; // 算是剪枝了
            backtracking(s, 0, 0);
            return result_93;
        }
        //78
        // leetcode 698 .Partition to K Equal Sum Subsets
        // https://leetcode.cn/problems/partition-to-k-equal-sum-subsets/solutions/1441006/by-lfool-d9o7/
        bool canPartitionKSubsets(vector<int>& nums, int k) {
            
        }


        
};