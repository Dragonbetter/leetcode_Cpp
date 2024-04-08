#include <iostream>
#include <sstream>
#include <stack>
#include <queue> // 包含priority_queue
#include <string>
#include <unordered_map>
#include <algorithm>

/*
实现函数递归调用就需要栈，但不是每种编程语言都支持递归，例如：
    递归的实现就是：每一次递归调用都会把函数的局部变量、参数值和返回地址等压入调用栈中，
    然后递归返回的时候，从栈顶弹出上一次递归的各项参数，所以这就是递归为什么可以返回上一层位置的原因
    栈溢出，系统输出的异常是Segmentation fault（当然不是所有的Segmentation fault 都是栈溢出导致的） ，
    如果你使用了递归，就要想一想是不是无限递归了，那么系统调用栈就会溢出。
*/


class MyQueue {
    // leetcode 232 12.23
    /*
    在工业级别代码开发中，最忌讳的就是 实现一个类似的函数，直接把代码粘过来改一改就完事了。
    这样的项目代码会越来越乱，一定要懂得复用，功能相近的函数要抽象出来，不要大量的复制粘贴，很容易出问题！
    =======================================
    时间复杂度: push和empty为O(1), pop和peek为O(n)
    空间复杂度: O(n)
    =======================================
    用两个先进后出 对称模拟一个先进先出；
    在push数据的时候，只要数据放进输入栈就好，
    但在pop的时候，操作就复杂一些，
    输出栈如果为空，就把进栈数据全部导入进来（注意是全部导入），再从出栈弹出数据；
    【此步其实就相当于把数据的方向换了一下】
    如果输出栈不为空，则直接从出栈弹出数据就可以了。
    最后如何判断队列为空呢？如果进栈和出栈都为空的话，说明模拟的队列为空了。
    */
public:
    std::stack<int> stIn;
    std::stack<int> stOut; 
    MyQueue() {}
    
    void push(int x) {
        stIn.push(x);
    }
    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        // 只有当stOut为空时，才从stIn内导入数据（导入stIn的全部数据）
        if(stOut.empty()){
            // 从stIn导入数据
            while(!stIn.empty()){
                // 先取数据 在删数据
                stOut.push(stIn.top());
                stIn.pop();
            }
        }
        // 先取数据 在删数据
        int result = stOut.top();
        stOut.pop();
        return result;
    }
    /** Get the front element. */
    int peek() {
        int res = this->pop(); // 直接使用已有的pop函数
        stOut.push(res); // 因为pop函数弹出了元素res，所以再添加回去
        return res;
    }
    /** Returns whether the queue is empty. */
    bool empty() {
        return stIn.empty() && stOut.empty();
    }
};
class MyStack {
public:
    // （这里要强调是单向队列） 
    // 一个队列在模拟栈弹出元素的时候只要将队列头部的元素（除了最后一个元素外）重新添加到队列尾部，
    // 此时再去弹出元素就是栈的顺序了。
    std::queue<int> que;
    MyStack() {}
    
    void push(int x) {
        que.push(x);
    }
     /** Removes the element on top of the stack and returns that element. */
    int pop() {
        int size = que.size();
        size--;
        // 只需要调动两次 
        while(size--){
            // 把前面的元素加到后面，而后删除前面的元素；
            que.push(que.front());
            que.pop();
        }
        int result = que.front(); // 此时弹出的元素顺序就是栈的顺序了 但也只针对这一个
        que.pop();
        return result;
    }
    /** Get the top element. */
    int top() {
        return que.back();
    }
    
    bool empty() {
        return que.empty();
    }
};
class Solution {
public:
    bool isValid_1(std::string s) {
        // 标准栈的用法 12.23
        /*
            第一种情况，字符串里左方向的括号多余了 ，所以不匹配
            第二种情况，括号没有多余，但是 括号的类型没有匹配上
            第三种情况，字符串里右方向的括号多余了，所以不匹配
        */
        std::stack<char> brackets;
        for(int i=0;i<s.size();i++){
            if(s[i]=='(' || s[i]=='{' || s[i]=='['){
                brackets.push(s[i]);
            }
            else{
                // 此种情况下极大概率会无物入栈
                // 如果栈是空的（即没有与之对应的开放括号），
                // 尝试访问栈顶元素（使用 brackets.top()）将会导致未定义行为，通常是程序崩溃
                if(brackets.empty()){return false;}
                if(s[i] == ')' && brackets.top() == '(' ||
                   s[i] == '}' && brackets.top() == '{' ||
                   s[i] == ']' && brackets.top() == '['){
                        brackets.pop();
                    }
                else{
                    return false;
                }
            }
        }   
        return brackets.empty();
    }
    bool isValid_2(std::string s) {
        // 12.23
        if (s.size() % 2 != 0) return false; // 如果s的长度为奇数，一定不符合要求
        std::stack<char> st;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == '(') st.push(')');
            else if (s[i] == '{') st.push('}');
            else if (s[i] == '[') st.push(']');
            // 第三种情况：遍历字符串匹配的过程中，栈已经为空了，没有匹配的字符了，说明右括号没有找到对应的左括号 return false
            // 第二种情况：遍历字符串匹配的过程中，发现栈里没有我们要匹配的字符。所以return false
            else if (st.empty() || st.top() != s[i]) return false;
            else st.pop(); // st.top() 与 s[i]相等，栈弹出元素
        }
        // 第一种情况：此时我们已经遍历完了字符串，但是栈不为空，说明有相应的左括号没有右括号来匹配，所以return false，否则就return true
        return st.empty();
    }
    std::string removeDuplicates_1(std::string s) {
       // leetcode 1047 12.25 匹配问题都是栈的强项  时间复杂度: O(n) 空间复杂度: O(n)
       // 随着循环进行 不断进栈 进栈前先判断该元素是否与栈顶元素相同 如果相同 则退栈 否则继续进栈 最后逆序返回栈内元素
       std::stack<char> stIn;
       for(int i=0;i<s.size();i++){
           if(!stIn.empty() && stIn.top()==s[i]){
                stIn.pop();
           }
           else {
            // 包含两种情况 第一章 栈为空 第二种栈不空 当前入栈元素与栈顶元素不相同     
                stIn.push(s[i]);
           }
       } 
       // 出栈 逐个出栈 最终返回的是string stack无遍历功能
       // 此处需要在初始化定义时为其声明 大小
       /*
        可以直接用 
        string result = "";
        while (!st.empty()) { // 将栈中元素放到result字符串汇总
            result += st.top();
            st.pop();
        }
        reverse (result.begin(), result.end()); // 此时字符串需要反转一下
       */
       std::string result(stIn.size(),'0');
       for(int j=stIn.size()-1;j>=0;j--){
            char temp = stIn.top();
            result[j] = temp;
            stIn.pop();
       }
       return result;
    }
    std::string removeDuplicates_2(std::string S) {
        // 时间复杂度: O(n) 空间复杂度: O(1)，返回值不计空间复杂度
        // 可以拿字符串直接作为栈，这样省去了栈还要转为字符串的操作
        std::string result;
        for(char s : S) {
            if(result.empty() || result.back() != s) {
                result.push_back(s);
            }
            else {
                // 删除元素 pop_back,pop_front->vector和string不符合,erase(p)删除迭代器p指定的元素
                result.pop_back();
            }
        }
        return result;
    }
    int stringToInt(const std::string& str) {
        // 将字符串转换为数字
        int result = 0;
        for (size_t i = 0; i < str.size(); ++i) {
            if (str[i] < '0' || str[i] > '9') {
                throw std::invalid_argument("Input string contains non-numeric characters");
            }
            result = result * 10 + (str[i] - '0');
        }
        return result;
    }
    int evalRPN_1(std::vector<std::string>& tokens) {
        // leetcode 150. Evaluate Reverse Polish Notation 12.25
        // 时间复杂度: O(n) 空间复杂度: O(n) 所以后缀表达式对计算机来说是非常友好的。
        // 思路是否可以理解为不断进栈 当碰到运算符时 即取出前两个元素进行计算 得到结果后 再压入栈 如此继续
        // 但其实在push时就可以直接将其转换为 longlong或int型 没必要push string进去 这样会很耗时
        std::stack<std::string> stIn;
        for(std::string tocken:tokens){
            if(stIn.empty() || (tocken >= "0" && tocken <= "9")){
                stIn.push(tocken);
            }
            // switch-case 语句在C++中仅仅接受 整形表达式（（包括enum类型）作为条件）
            else{
                int32_t temp;
                // 这样写获得得是ASCIIA码值 '2' 50；
                /*
                    1.运用标准库方法 
                        C++11：std::stoi（string to integer）函数将字符串转换为int
                        C++旧版：使用了字符串流来从字符串中提取整数值
                            std::istringstream iss(str); // 构建istringstream对象 将字符串赋值加入
                            int num;
                            iss >> num; // 读出单词 以空格为分割
                    2.确保字符串只包含数字字符。
                    从字符串中的每个字符中减去 '0'（字符 '0' 在ASCII表中的值），这将把字符转换为相应的整数值。
                    考虑字符在字符串中的位置来确定其数位（十位、百位等）
                    int stringToInt(const std::string& str)          
                */
                int32_t temp_1 = std::stoi(stIn.top());
                stIn.pop();
                /*
                std::istringstream temp_str(stIn.top());
                temp_str>>temp_2;
                stIn.pop();
                */
                // 首先复制栈顶元素到局部变量
                std::string temp_2_str = stIn.top();
                stIn.pop();
                // 然后将字符串转换为整数
                std::istringstream temp_str(temp_2_str);
                int32_t temp_2;
                temp_str >> temp_2;
                if (tocken == "+") {
                    temp = temp_2 + temp_1;
                    // ...
                } else if (tocken == "-") {
                    temp = temp_2 - temp_1;
                    // ...
                } else if (tocken == "*") {
                    temp = temp_2 * temp_1;
                    // ...
                } else if (tocken == "/") {
                    temp = temp_2 / temp_1;
                    // ...
                }
                stIn.push(std::to_string(temp));
            }
        }
        // 判断并计算
        return std::stoi(stIn.top());
    }
    int evalRPN_2(std::vector<std::string>& tokens) {
        // 力扣修改了后台测试数据，需要用longlong
        // 栈直接用数字 不需要用string
        std::stack<long long> st; 
        for (int i = 0; i < tokens.size(); i++) {
            if (tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "*" || tokens[i] == "/") {
                long long num1 = st.top();
                st.pop();
                long long num2 = st.top();
                st.pop();
                if (tokens[i] == "+") st.push(num2 + num1);
                if (tokens[i] == "-") st.push(num2 - num1);
                if (tokens[i] == "*") st.push(num2 * num1);
                if (tokens[i] == "/") st.push(num2 / num1);
            } else {
                st.push(stoll(tokens[i]));
            }
        }
        int result = st.top();
        st.pop(); // 把栈里最后一个元素弹出（其实不弹出也没事）
        return result;
    }
    class Monotonic_queue{
    //  适用于std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k)
    // 题解中单调队列里的pop和push接口，仅适用于本题哈，不同的场景由不同写法 
    public:
        std::deque<int> M_queue;
        // pop
        // 每次弹出的时候，比较当前要弹出的数值是否等于队列出口元素的数值，如果相等则弹出。
        // 同时pop之前判断队列当前是否为空。
        void pop(int value){
            if(!M_queue.empty() && value == M_queue.front()){
                // 队列不为空 而且value等于首个元素 则弹出
                M_queue.pop_front();
            }
        }
        // push
        // 如果push的数值大于入口元素的数值，那么就将队列后端的数值弹出，直到push的数值小于等于队列入口元素的数值为止。
        // 这样就保持了队列里的数值是单调从大到小的了。
        void push(int value){
            // 循环判断value与对应的入口元素的值大小，当小于时 加入，大于 则弹出
            while(!M_queue.empty() && value>M_queue.back()){
                M_queue.pop_back();
            }
            M_queue.push_back(value);
        }
        //front
        int front(){
            return M_queue.front();
        }
    };
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        // leetcode 239. Sliding Window Maximum 12.25
        // 单调队列主要需要想清楚如何去维护
        // 当k=1时 max即为其本身，当k>=2时 每次保存max和second-max=》即需要维护一个元素队列,保存相应的依照递减的值序列 尺度不定
        /*
            单调队列：单调递减，如何构造，如何更新；pop_front()出口元素,push_back()入口元素
            ==》运用
            在滑动窗口的过程中维护一个单调递减（非递增序列）队列，每次移动时 先判断移除该窗口的元素与队列的关系，如果该元素等于入口元素，则弹出入口元素（即便有重复也无关系）
            而后再判断需要进入该窗口的元素，将其与入口元素比较，从而确定在该队列中所应位于的位置，即如果小于入口元素，则加在后面；如果大于入口元素，则弹出入口元素一直循环直到小于
            而每次的最大值即该单调队列的第一个元素。            
            pop_front:比较的是需要元素与队列出口元素    
        时间复杂度: O(n)
        空间复杂度: O(k)
        再来看一下时间复杂度，使用单调队列的时间复杂度是 O(n)。
        单调队列的实现，nums 中的每个元素最多也就被 push_back 和 pop_back 各一次，没有任何多余操作，所以整体的复杂度还是 O(n)。
        空间复杂度因为我们定义一个辅助队列，所以是O(k)。      
        */
        Monotonic_queue M_que;
        std::vector<int> result;
        // 先将前k的元素放进队列
        for(int i=0;i<k;i++){
            M_que.push(nums[i]);
        }
        result.push_back(M_que.front()); // result 记录前k的元素的最大值
        for(int i = k;i<nums.size();i++){
            M_que.pop(nums[i-k]); // 滑动窗口移除最前面元素
            M_que.push(nums[i]);  // 滑动窗口前加入最后面的元素
            result.push_back(M_que.front()); // 记录对应的最大值
        }
        return result;
    }
    std::vector<int> topKFrequent_map(std::vector<int>& nums, int k) {
        // leetcode 347. Top K Frequent Elements 12.25 map实现
        // 使用快排要将map转换为vector的结构，然后对整个数组进行排序， 而这种场景下，我们其实只需要维护k个有序的序列
        // 思路1：pair遍历一遍，储存对应的数据和出现的次数 而后再依据map的值对键进行排序 输出最大的k个
        std::unordered_map<int,int> u_map;
        for(int i=0;i<nums.size();i++){
            // map 的不同下标操作；如果没有该元素 会创建一个该值 并进行值初始化 返回一个mapped_type对象；
            u_map[nums[i]]++;
        }
        // 依据map第二个元素的值进行排序 则需要先将其取出来
        std::vector<std::pair<int,int>> vec;
        // 将映射中的元素复制到一个std::vector<std::pair<std::string, int>>向量中。
        for(const auto & pair:u_map){
            // 遍历map，取出pair对象 赋值
            vec.push_back(pair);
        }
        // 使用std::sort函数和一个自定义的比较函数来根据值对这个向量进行排序 注意排序此处从大到小
        std::sort(vec.begin(),vec.end(),[](const std::pair<int,int> &pair1,const std::pair<int,int> pair2){return pair1.second>pair2.second;});
        std::vector<int> result;
        for(int i=0;i<k;i++){
            result.push_back(vec[i].first);
        }
        return result;
    }
    // 确立优先队列的比较规则
    class mycomparsion{
        // 默认实现为最大堆 即最大的在优先队列头部 此处需要对其进行更改 故而使用
            public:
                bool operator()(const std::pair<int,int> &lhs,const std::pair<int,int> &rhs){
                    return lhs.second > rhs.second;
                }
        };
    std::vector<int> topKFrequent_(std::vector<int>& nums, int k){
        // leetcode 347. Top K Frequent Elements 12.25  优先级队列
        /*
          优先级队列对外接口只是从队头取元素，从队尾添加元素，再无其他取元素的方式，看起来就是一个队列
          优先级队列内部元素是自动依照元素的权值排列
          缺省情况下priority_queue利用max-heap（大顶堆）完成对元素的排序，这个大顶堆是以vector为表现形式的complete binary tree（完全二叉树）
          堆是一棵完全二叉树，树中每个结点的值都不小于（或不大于）其左右孩子的值
          使用快排要将map转换为vector的结构，然后对整个数组进行排序， 而这种场景下，我们其实只需要维护k个有序的序列
          要用小顶堆，因为要统计最大前k个元素，只有小顶堆每次将最小的元素弹出，最后小顶堆里积累的才是前k个最大元素
           //  思维不能固化，大小顶堆的优先队列都可以做。
          如果是小顶堆，维护优先队列的大小为k，如果超过了k就pop掉堆顶，最后把优先队列中的k个元素全都放进vector中返回；
          如果是大顶堆，不限制优先队列的大小，将所有pair都放进堆里，最后依次将k个堆顶元素放进vector中返回，每放一次则pop一次，维护堆。
        */
        // 统计元素顺序
        std::unordered_map<int,int> u_map;
        for(int i=0;i<nums.size();i++){
            u_map[nums[i]]++;
        }
        // 按顺序维护相应的优先队列
        std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,mycomparsion> pri_que;
        // 构建优先队列
        for(std::unordered_map<int,int>::iterator it = u_map.begin();it!=u_map.end();it++){
            pri_que.push(*it); // 入队列
            if(pri_que.size()>k){
                // 如果元素数量大于k，则相应的将其队列头部的最小的踢出去 这样最终保存的便是最大的k个，只需要维护k个，而不需要变成vector再快排
                pri_que.pop();
            }            
        }
         // 找出前K个高频元素，因为小顶堆先弹出的是最小的，所以倒序来输出到数组
        std::vector<int> result(k);
        for(int j=k-1;j>=0;j--){
            result[j] = pri_que.top().first;
            pri_que.pop();
        }
        return result;
    }
};
int main(){
    Solution a;
    std::vector<std::string> rpn{"2","1","+","3","*"};
    int b = a.evalRPN_1(rpn);
    std::cout<<b<<std::endl;
    return 0;
}

