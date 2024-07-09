# include <string>
# include <vector>
# include <iostream>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>
using namespace std;
/*
hash table
set,multiset,unordered_set,
map,multimap,unordered_map
*/
class Solution {
public:
    bool isAnagram_1(string s, string t) {
        // leetcode 242 12.14  Valid Anagram
        // 383.赎金信(opens new window) 49.字母异位词分组(opens new window) 438.找到字符串中所有字母异位词
        int record[26] = {0};
        for (int i=0;i<s.size();i++){
            // 字符串处理 在此处C风格字符串被存储进string容器 
            // 并不需要记住字符a的ASCII，只要求出一个相对的数值就可以 此处-操作在string中有定义为ASCII差值
            record[s[i]-'a']++;
        }
        for (int i=0;i<t.size();i++){
            record[t[i]-'a']--;
        }
        for (int i=0;i<26;i++){
            if (record[i]!=0){
                // record数组如果有的元素不为0，说明字符串s和t 字符数量不相同 
                return false;
            }
        }
        return true;
    }
    bool isAnagram_2(string s, string t) {
        // 1.直接用sort函数
        sort(s.begin(),s.end());
        sort(t.begin(),t.end());
        if(s==t)
            return true;
        else
            return false;
    }
    bool isAnagram_3(string s, string t) {
        // map计数 
        unordered_map<char,int> map;
        if (s.size() != t.size()) 
            return false;
        for(int i=0;i<s.size();i++){
            ++map[s[i]];
            --map[t[i]];
        }  
        for(unordered_map<char,int>::iterator it=map.begin();it!=map.end();it++){
            if(it->second!=0)
                return false;
        }
        return true;  
    }
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // leetcode 349. Intersection of Two Arrays 12.14 350.两个数组的交集 II
        unordered_set<int> result_set; // 存放结果 set为结果去重复
        unordered_set<int> nums_set(nums1.begin(),nums1.end());// 基于迭代器用一个范围给set进行初始化
        for (int num:nums2){
            // 范围for循环语句
            // find 如果未找到返回尾后迭代器 如果找到 返回指定元素的下标
            if(nums_set.find(num)!=nums_set.end()){
                // insert 返回pair;第一个成员是迭代器 指向具有给定关键词的元素 第二个为bool类型 插入成功则为true
                result_set.insert(num);
            }
        }
        return vector<int>(result_set.begin(),result_set.end());
    }
    int getSum(int n) {
        int sum = 0;
        // n%10 取余则是取尾部的值 n/10则是去除尾部的值 进行重复选取 求余求除
        while (n) {
            sum += (n % 10) * (n % 10);
            n /= 10;
        }
        return sum;
    }
    bool isHappy(int n) {
        // 时间复杂度: O(logn)  空间复杂度: O(logn)
        // leetcode 202 使用哈希法，来判断这个sum是否重复出现，如果重复了就是return false， 否则一直找到sum为1为止。
        unordered_set<int> result;
        // 写一个迭代函数 第一步 取出n的各个单数上的值 第二步加相应的各个数进行平方 相加 
        while(1){
            int sum = getSum(n);
            if (sum==1){
                return true;
            }
            if (result.find(sum)!=result.end()){
                return false;
            }
            else{
                result.insert(sum);
            }
            n = sum;
        }
    }
    vector<int> twoSum(vector<int>& nums, int target) {
        // leetcode 1 twosum 12.14 map思路
        // 双指针的思路 先sort 后首尾进行运行 当相加大于target 尾指针前移 小于target则 首指针后移 直到两个值相加所得相等 或首大于尾部
        unordered_map<int,int> result;
        for(int i=0;i<nums.size();i++){
            if (result.find(nums[i])!=result.end()){
                return vector<int>{result[nums[i]],i};
            }
            else{
                // map存放为值和位置
                result.insert({target-nums[i],i});
            }
        }
        return vector<int>{0,0};
    }
    vector<vector<int>> threeSum(vector<int>& nums) {
        // leetcode 15 ->3sum 12.15 双指针法 时间复杂度: O(n^2) 空间复杂度: O(1)
        // 法2 HASH法 两层for循环就可以确定 a 和b 的数值了，可以使用哈希法来确定 0-(a+b) 是否在 数组里出现过，其实这个思路是正确的，但是我们有一个非常棘手的问题，就是题目中说的不可以包含重复的三元组。
        // 把符合条件的三元组放进vector中，然后再去重，这样是非常费时的，很容易超时，也是这道题目通过率如此之低的根源所在。
        // 1. sort 重排元素
        sort(nums.begin(),nums.end());
        vector<vector<int>> result;
        // 2. 定义对应的指针
        // 因为元素是可以重复的 故而针对于元素的移动 需要添加相等元素情况的判断 此情况下反复移动
        for(vector<int>::iterator first = nums.begin();first!=nums.end();first++){
            // 排序之后如果第一个元素已经大于零，那么无论如何组合都不可能凑成三元组，直接返回结果就可以了
            if (*first > 0) {return result;}
            // 错误去重a方法，将会漏掉-1,-1,2 这种情况
            /*
            判断的依据应当是 上一个元素判断过不行 故而这个元素也不行 因为上一个元素的second指针会经过该元素 
            if (*first == *(first + 1)) {
                continue;
            }
            */
            if (first != nums.begin() && *first == *(first - 1)) {continue;}
            vector<int>::iterator left = first+1;
            vector<int>::iterator right = nums.end()-1;
            while(left<right){
                int sum = *first+*left+*right;
                if (sum==0){
                    result.push_back(vector<int>{*first,*left,*right});
                    // 为了防止重复 在begin和end移动的时候 添加限制条件 很可能移动的过程中冲过大的限制 
                    while (left < right && *left == *(left + 1)) {++left;}
                    while (left < right && *right == *(right - 1)) {--right;}
                    ++left;
                    --right;
                }
                // 在左右移动的时候 去重逻辑只是提前作用 无影响
                else if (sum>0){
                    --right;
                }
                else if (sum<0){
                    ++left;
                }
            }
        }
        return result; 
    } 
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        // leetcode 18. 4Sum 两层循环 再套双指针
        // 运用迭代器写法 auto等 时间复杂度: O(n^3) 空间复杂度: O(1)
        vector<vector<int>> results;
        sort(nums.begin(), nums.end());
        for(auto first = nums.begin();first!=nums.end();first++){
            // 剪枝处理
            if (*first>target && *first>=0){break;}
            // 去重处理
            if (first!=nums.begin() && *first==*(first-1)){continue;}
            for(auto second = next(first); second!=nums.end();second++){
                // 剪枝处理
                if (*first+*second>target && *first+*second>=0){break;}
                // 去重处理
                if (second!=next(first) && *second == *(second-1)){continue;}
                auto left = next(second);
                auto right = prev(nums.end());
                while(left<right)
                {           
                    // nums[k] + nums[i] + nums[left] + nums[right] > target 会溢出    
                    long sum = (long)*first+*second+*left+*right;
                    if(sum<target){left++;}
                    else if(sum>target){right--;}
                    else if(sum==target){
                        results.push_back({*first,*second,*left,*right});
                        while(left<right && *right==*(right-1)){right--;}
                        while(left<right && *left ==*(left+1)){left++;}
                        right--;
                        left++;
                }
                }
            }
        }
        return results;
    }
    int fourSumCount_1(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) {
        // leetcode 454 4Sum II 12.18 哈希表 
        /*
        O(n^2)(a+b循环)+O(n^2)(c+d循环)+O(n^2)(两个map循环)
        因为最后只需要输出对应的count，而不需要输出对应的序号值 故而map设计为（key：a+b的值，value：count）
        有序的map可以为其减少比较次数
        没必要对于C和D也建造一个map，直接在循环中find即可
        */
       unordered_map<long,int> asumb;
       unordered_map<long,int> csumd;
       int len = nums1.size();
       long ab,cd;
       // 双层循环求取各种的和
       for (int i=0;i<len;i++){
           for (int j=0;j<len;j++){
                ab = nums1[i]+nums2[j];
                auto retab = asumb.insert({ab,1});
                // insert操作返回一个pair，first为一个迭代器，second为一个bool值 
                // 如果元素已经存在 则insert什么都不做 bool为false，表示插入失败；如果元素不存在，则插入成功，bool为true；
                if(!retab.second){
                    ++retab.first->second;
                }
                // 将对应的值添加到对应的map中去
                cd = nums3[i]+nums4[j];   
                auto retcd = csumd.insert({cd,1});
                if(!retcd.second){
                    ++retcd.first->second;
                }
           }
       }
       // 迭代两个map，选取对应的元素
       int ret_sum=0;
       for(const auto & m:asumb){
           for (const auto &n:csumd){
            if (m.first+n.first == 0){
                ret_sum += m.second*n.second;
            }
           }
       }
       return ret_sum;
    }
    int fourSumCount_2(vector<int>& A, vector<int>& B, vector<int>& C, vector<int>& D) {
        /*
        时间复杂度: O(n^2)
        空间复杂度: O(n^2)，最坏情况下A和B的值各不相同，相加产生的数字个数为 n^2
        首先定义 一个unordered_map，key放a和b两数之和，value 放a和b两数之和出现的次数。
        遍历大A和大B数组，统计两个数组元素之和，和出现的次数，放到map中。
        定义int变量count，用来统计 a+b+c+d = 0 出现的次数。
        在遍历大C和大D数组，找到如果 0-(c+d) 在map中出现过的话，就用count把map中key对应的value也就是出现次数统计出来。
        最后返回统计值 count 就可以了  
        */
        unordered_map<int, int> umap; //key:a+b的数值，value:a+b数值出现的次数
        // 遍历大A和大B数组，统计两个数组元素之和，和出现的次数，放到map中
        for (int a : A) {
            for (int b : B) {
                umap[a + b]++;
            }
        }
        // 范围for循环语句 
        // umap[]下标的运用 如果umap[k] 返回关键字为k的元素，如果k不在umap中，添加一个关键字为k的元素 ，并对其进行值初始话，此处0
        int count = 0; // 统计a+b+c+d = 0 出现的次数
        // 在遍历大C和大D数组，找到如果 0-(c+d) 在map中出现过的话，就把map中key对应的value也就是出现次数统计出来。
        for (int c : C) {
            for (int d : D) {
                if (umap.find(0 - (c + d)) != umap.end()) {
                    count += umap[0 - (c + d)];
                }
            }
        }
        return count;
    }
    bool canConstruct(string ransomNote, string magazine){
        // leetcode 383. Ransom Note  12.18
        // 统计magazine的map，而后再遍历ransonNote时 find
        // unordered_map<int,int> umap; 其实在本题的情况下，使用map的空间消耗要比数组大一些的，
        // 因为map要维护红黑树或者哈希表，而且还要做哈希函数，是费时的！数据量大的话就能体现出来差别了。 所以数组更加简单直接有效！
        int record[26]={0};
        //add 
        if (ransomNote.size()>magazine.size()){
            return false;
        }
        for(int i=0;i<magazine.size();i++){
            // 通过recoder数据记录 magazine里各个字符出现的次数
            record[magazine[i]-'a']++;
        }
        for(int j=0;j<ransomNote.size();j++){
            record[ransomNote[j]-'a']--;
            if (record[ransomNote[j]-'a']<0){
                return false;
            }
        }
        return true;
    }
};
int main(){
    Solution a;
    vector<int> a1={1,2};
    vector<int> a2={-2,-1};
    vector<int> a3={-1,2};
    vector<int> a4={0,2};
    int b = a.fourSumCount_1(a1,a2,a3,a4);
    return b;
}