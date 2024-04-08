#include <string>
#include <vector>
using namespace std;
// string
class Solution {
public:
    void reverseString(vector<char>& s) {
        // leetcode 344 Reverse String 12.20
        // 将i<=j（22ms）会比i<s.size()/2（15ms）慢，因为其多运行了一次 
        for(int i=0,j=s.size()-1;i<j;i++,j--){
            // swap(s[i],s[j]);
            int tmp = s[i]; // O(1)
            s[i] = s[j];
            s[j] = tmp;
        }
    }
    void reverse(string &s,int start,int end){
        // 左闭右闭区间
        for(int i=start,j=end;i<j;i++,j--){
            swap(s[i],s[j]);
        }
    }
    string reverseStr_1(string s, int k) {
        //时间复杂度: O(n)
        //空间复杂度: O(1)或O(n), 取决于使用的语言中字符串是否可以修改.
        // leetcode 541 Reverse String II 12.20
        // 基础的翻转操作仍然不变 只是对于何时转 有一个规则的判断 不用单元素逐个加 可以一次直接加2k个
        // 记录还剩下多少个字符
        for(int i=0;i<s.size();i+=2*k){
            // 如果是第二种情况 则相应的执行完 前k个元素翻转后 再加2k，会不满足条件，直接结束；
            // 而如果大于2k，则可以进一步执行 每次加2k是好方法
            // 1. 每隔 2k 个字符的前 k 个字符进行反转
            // 2. 剩余字符小于 2k 但大于或等于 k 个，则反转前 k 个字符
            if(i+k<=s.size()){
                reverse(s,i,i+k-1);
                continue;
            }
            // 3. 剩余字符小于k个 则将剩余字符全部翻转
            reverse(s,i,s.size()-1);
        }
        return s;    
    }
    string reverseStr_2(string s, int k) {
        int n = s.size(),pos = 0;
        while(pos < n){
            //剩余字符串大于等于k的情况
            if(pos + k < n) reverse(s,pos,pos+k-1);
            //剩余字符串不足k的情况 
            else reverse(s,pos,s.size()-1);
            pos += 2 * k;
        }
        return s;
    }
    string Replacdigit(string s){
        // 12.20 给定一个字符串 s，它包含小写字母和数字字符，将字符串中的字母字符保持不变，而将每个数字字符替换为number。
        // 数组和字符串都是连续存储的 将一个数字替换为字符串需要额外赋予空间 并且在该数字后的已有字母都要后移 
        // 可以先循环一遍 统计处数字的数量 而后赋予新空间（两种赋值方法 原修改或则重新开辟一个数组 ）
        // 从前往后赋值的话 每个字母都需要移动 0（n2）从后往前 则赋予空的 不需要移动 
        int count =0;
        int sOldSize = s.size();
        for(int i=0;i<s.size();i++){
            if(s[i]>'0' && s[i]<'9'){
                count++;
            }
        }
        // 扩充字符串的大小 顺序容器都有的操作 resize() 转变为具有固定大小的数值
        s.resize(s.size()+count*5); // 原有字符的位置不变 
        int sNewSize = s.size();
        //从后向前将空格替换成“number”
        for(int i=sNewSize-1,j=sOldSize-1;j<i;i--,j--){
            if(s[j]>'9' || s[j]<'0'){
                // 如果是字符则正常执行
                s[i] = s[j];
            }
            else {
                // 数字逐步替代 更换number
                s[i] = 'r';
                s[i-1] = 'e';
                s[i-2] = 'b';
                s[i-3] = 'm';
                s[i-4] = 'u';
                s[i-5] = 'n';
                i-=5;
            }
        }
        return s;
    }
    void removeExtraSpaces_erase(string& s) {
        // 删除重复的空格 
        for (int i = s.size() - 1; i > 0; i--) {
            if (s[i] == s[i - 1] && s[i] == ' ') {
                s.erase(s.begin() + i);
            }
        }
        // 删除字符串最后面的空格
        if (s.size() > 0 && s[s.size() - 1] == ' ') {
            s.erase(s.begin() + s.size() - 1);
        }
        // 删除字符串最前面的空格
        if (s.size() > 0 && s[0] == ' ') {
            s.erase(s.begin());
        }
    }
    void removeExtraSpaces_doubleptr(string& s){
        int slow=0;
        for(int fast=0;fast<s.size();fast++){
            // 外界fast的循环是没有问题的 遇到非空格的进行处理 遇到空格直接跳过 不赋给slow
            if(s[fast]!=' '){
                // s[slow] = s[fast]; 这样写的话会移除所有的空格 不太正确 故而遇到空格后可以手动保留一个空格 而后加上对应的单词序列
                if(slow!=0) s[slow++]=' ';
                while(fast < s.size() && s[fast]!=' '){ // 不能等于空格 等于空格相当于 一个单词的结束 因为if是一个空格的开始 此处一个空格的结束
                    s[slow++] = s[fast++];
                }
            }
        }
        s.resize(slow); // slow的大小即为去除多余空格后的大小
    }
    string reverseWords(string s) {
        // leetcode 151. Reverse Words in a String 12.20
        // 翻转字符串中的单词 并相应的去除首尾的多余空格以及单词之间的空格。
        // 空间O(1)？
        /*
        1.简单方法 先将其作为一个大的整体进行reverse，而后从头开始 去除头部空格 针对每个单词判断其不是空格则将其反转
        删除多余的空格的话 需要运用什么 erase？==>先去除 在整体reverse 然后部分reverse
        =====================
        移除多余空格 从前向后遍历，遇到空格了就erase 
        erase本来就是O(n) 遍历O(n) 整体O(n2)
        void removeExtraSpaces_erase(string& s) 
        =====================
        移除多余空格 运用双指针法 快慢指针 慢指针指向真实的值 快指针遍历 将不等于' '的值赋给慢指针。
        O(n)
        */
       removeExtraSpaces_doubleptr(s);
       reverse(s,0,s.size()-1);
       /*
       注意 此处的begin+count-1其实完全不需要 可以用i-1去代替 而后每次更新begin为i+1，因为i代表的是空格       
       int i=0;
       int begin = 0;
       int count = 0;
       while(i<s.size()){
          if(s[i]!=' '){
            count++;
          }
          else if(s[i]==' '){
          reverse(s,begin,begin+count-1);
          begin = begin+count+1;
          count = 0;
          }
          // 添加一个处理最后的代码的 因为没有' '去判断。
          if(i==s.size()-1){
            reverse(s,begin,begin+count-1);
          }
          i++;
       }
       */
       int start = 0; //removeExtraSpaces后保证第一个单词的开始下标一定是0
       for (int i=0;i<=s.size();i++){
       if(i==s.size() || s[i]==' '){  // 到达末尾 或则遇到相应的空格 说明需要转换单词了
            reverse(s,start,i-1); //翻转，注意是左闭右闭 []的翻转。
            start = i+1;  // 更新下一个单词的开始下标start;
        }
       }
       return s;
    }
    string Righthanded_string(string s,int n){
        // 卡码网题目 12.21 不能申请额外空间，只能在本串上操作
        // 字符串的右旋转操作是把字符串尾部的若干个字符转移到字符串的前面。给定一个字符串 s 和一个正整数 k，
        // 请编写一个函数，将字符串中的后面 k 个字符移到字符串的前面，实现字符串的右旋转操作
        // 通过 整体倒叙，把两段子串顺序颠倒，两个段子串里的的字符在倒叙一把，负负得正，这样就不影响子串里面字符的顺序了
        // 先整体 再局部右旋转  或则 先局部再整体 左旋转、
        int len = s.size(); //获取长度
        reverse(s,0,len-1); // 整体反转
        reverse(s,0,n-1); // 先反转前一段，长度n
        reverse(s,n, len-1); // 再反转后一段
        return s;
    }
    int strStr(string haystack, string needle) {
        // leetcode 28  -> 12.21 Find the Index of the First Occurrence in a String
        // 一遍循环 逐个比较 若不行 在指针后移 O(n × m)
        int low =0;
        for(int fast=0;fast<haystack.size();fast++){
            // 比较 
            int curr_fast = fast;
            if(haystack[curr_fast]==needle[0]){
                // 持续比较
                while(low<needle.size()){
                    if(haystack[curr_fast]!=needle[low]){break;}
                    else{curr_fast++;low++;}
                }
                if(low==needle.size()){return fast;}
            }
            low = 0;
        }
        return -1;    
    }
     // TODO KMP的经典思想就是:当出现字符串不匹配时，可以记录一部分之前已经匹配的文本内容，利用这些信息避免从头再去做匹配。
    void getNext(int* next, const string& s) {
        // 相同的前后缀时 i和j同时向前移动 因为j表示的是从头开始的前缀 故而j的值可以直接等于next[i]的值 
        int j = 0;
        next[0] = 0;
        // 两个指针的位置不同 初始相邻 j=0，i=1；
        for(int i = 1; i < s.size(); i++) { // 注意i从1开始
            while (j > 0 && s[i] != s[j]) { // 前后缀不相同了
                j = next[j-1]; // 向前回退
            }
            if (s[i] == s[j]) { // 找到相同的前后缀
                j++;
            }
            next[i] = j; // 将j（前缀的长度）赋给next[i]
        }
    }
    int strStr_KMP(string haystack, string needle) {
        if (needle.size() == 0) {
            return 0;
        }
        int next[needle.size()];
        // 先求出前缀表 
        getNext(next, needle);
        int j = 0; // // 因为next数组里记录的起始位置为-1 =》 j值得是needle的
        for (int i = 0; i < haystack.size(); i++) { // 注意i就从0开始
            while(j > 0 && haystack[i] != needle[j]) { // 不匹配
                j = next[j-1]; // j 寻找之前匹配的位置
            }
            if (haystack[i] == needle[j]) { // 匹配，j和i同时向后移动
                j++; // i的增加在for循环里
            }
            if (j == (needle.size()) ) { // 文本串s里出现了模式串t
                return (i - needle.size() + 1);
            }
        }
        return -1;
    }
    bool repeatedSubstringPattern_1(string s) {
        //leetcode 459 Repeated Substring Pattern
        // 错误思路：如果单词是单数 则必然所有词一样 如果单词是double的 则必然可以拆分成2份 对称(不一定的 比如可以ababab)
        /*思路2：当一个字符串s：abcabc，内部由重复的子串组成，那么这个字符串的结构由前后相同的子串组成，那么既然前面有相同的子串，后面有相同的子串，用 s + s，
                这样组成的字符串中，后面的子串做前串，前面的子串做后串，就一定还能组成一个s;所以判断字符串s是否由重复子串组成，只要两个s拼接在一起，里面还出现一个s的话，就说明是由重复子串组成。
                当然，我们在判断 s + s 拼接的字符串里是否出现一个s的的时候，要刨除 s + s 的首字符和尾字符，这样避免在s+s中搜索出原来的s，我们要搜索的是中间拼接出来的s
        */ 
        string t = s + s;
        t.erase(t.begin()); t.erase(t.end() - 1); // 掐头去尾
        if (t.find(s) != std::string::npos) return true; // r
        return false;
        // 当然find 函数的时间复杂度（暴力解法是m * n，一般库函数实现为 O(m + n)）
    }
    bool repeatedSubstringPattern_2(string s){
        // TODO最小重复子串 
        // 在由重复子串组成的字符串中，最长相等前后缀不包含的子串就是最小重复子串
        if (s.size() == 0) {
            return false;
        }
        int next[s.size()];
        getNext(next, s);
        int len = s.size();
        /*
            如果next[len - 1] != 0，说明以最后这个字符是存在相等的前缀和后缀和的 ；
            而len-（next[len-1]）则表明最小重复子串的长度 则总的len%x如果可以整除则表明都可以由其构成。
        */
        // 数组长度减去最长相同前后缀的长度相当于是第一个周期的长度，也就是一个周期的长度，如果这个周期可以被整除，就说明整个数组就是这个周期的循环
        if (next[len - 1] != 0 && len % (len - (next[len - 1] )) == 0) {
            return true;
        }
        return false;
    }
};
int main(){
    Solution a;
    string s = "the sky is blue";
    string b = a.reverseWords(s);
    return 0;
}

