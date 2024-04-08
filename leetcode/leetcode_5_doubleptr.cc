#include <iostream>
#include <vector>
#include <string>
// 12.23 集中完成
class ListNode{
    public:
        int val;
        ListNode *next;

        ListNode():val(0),next(nullptr){}
        ListNode(int x):val(x),next(nullptr){}
        ListNode(int x,ListNode *next):val(x),next(next){}
};
class solution{
    // 12.23 
    public:
    // 27 移除元素
    int remove_element_27(std::vector<int> nums,int val){
        // 快慢指针 
        int slow = 0;
        for(int fast=0;fast<nums.size();fast++){
            if(nums[fast]!=val){
                nums[slow]=nums[fast];
                slow++;
            }
        }
        return slow;
    }
    // 344 反转字符串
    // 字符数组 char[] =》 vector<char> &s
    void reverse_string(std::vector<char> &s){
        // 前后双指针进行翻转
        for(int begin=0,end=s.size()-1;begin<s.size()/2;begin++,end--){
            int tmp = s[end];
            s[end] = s[begin];
            s[begin] = tmp;
        }
    }
    // 卡码网 替换数字
    void replace_number(std::vector<char> & s){
        int count=0;
        for(int i=0;i<s.size();i++){
            if(s[i]>='0' && s[i]<='9'){
                count++;
            }
        }
        int old_size = s.size();
        s.resize(old_size+count*5);
        int new_size = s.size();
        // 从后往前遍历
        for(int old_ptr = old_size-1,new_ptr=new_size-1;new_ptr<old_ptr;old_ptr--,new_ptr--){
            if(s[old_ptr]>='0' && s[old_ptr]<='9'){
                // 数字进行替换 倒着替换 number
                s[new_ptr] = 'r';
                s[new_ptr-1] = 'e';
                s[new_ptr-2] = 'b';
                s[new_ptr-3] = 'm';
                s[new_ptr-4] = 'u';
                s[new_ptr-5] = 'n';
                new_ptr = new_ptr-5;
            }
            else{
                // 小写字符直接赋值
                s[new_ptr] = s[old_ptr];
            }
        }
    }
    // 151 翻转字符串里的单词
    void reverse_string_for(std::string &s,int begin,int end){
        for(int i =begin,j=end;i<j;i++,j--){
            std::swap(s[i],s[j]);
        }
    }
    std::string reverse_word_instring(std::string &s){
        // 先去除单词里的空格 
        int slow = 0;
        for(int fast=0;fast<s.size();fast++){
            if(s[fast]!=' '){ // 逐级判断 先非空格 而后再依据slow是否等于0添加对应的 ’‘ 
                // 添加空格
                if (slow!=0) s[slow++]=' ';
                while(fast<s.size() && s[fast]!=' '){
                    s[slow++]=s[fast++];
                }
            }
        }
        s.resize(slow);
        // 全部翻转
        reverse_string_for(s,0,s.size()-1);
        int start = 0;
        for(int i=0;i<=s.size();i++){
            if(i==s.size() || s[i]==' '){
                reverse_string_for(s,start,i-1);
                start = i+1;
            }
        }
        return s;
    }
    // 206 翻转链表
    ListNode* reverse_linkedlist(ListNode* head){
        ListNode* tmp = nullptr;
        ListNode* pre = nullptr;
        ListNode* curr = head;
        // 翻转指针
        while(curr){
            tmp = curr->next;
            curr->next = pre;
            pre = curr;
            curr = tmp;
        }
        // curr为空的时候 pre刚好是最后一个
        return pre;
    }
    // 19 删除列表的倒数第N个节点 
    ListNode* removeNthNode(ListNode* head,int n){
        //有删除第一个节点的可能 即第一个节点具有特殊性 故而对应的定义dummyhead
        ListNode* dummyhead = new ListNode(0);
        dummyhead->next = head;
        ListNode* slow = dummyhead;
        ListNode* fast = dummyhead;
        // 注意n--和--n都可以 但需要注意对应的配对 
        // n-- 配fast = dummyhead；
        // --n 配fast = dummyhead->next; 
        while(n--){
            fast = fast->next;
        }
        while(fast->next!=nullptr){
            slow = slow->next;
            fast = fast->next;
        }
        ListNode* tmp = slow->next;
        slow->next = slow->next->next;
        // delete 一个指针只是删除指针所指的地方 但指针本身仍然一直指着那边 故而需要变为nullptr；
        delete tmp; 
        tmp = nullptr;
        head = dummyhead->next;
        delete dummyhead;
        dummyhead = nullptr;  
        return head;    
    }
    
};
int main(){
    return 0;
}