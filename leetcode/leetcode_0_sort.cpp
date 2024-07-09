#include<iostream>
#include<vector>

using namespace std;
using std::vector;
using std::cout;
using std::endl;

class basic_sort{
    private:

    public:
    // 插入排序分析 基于最后一个元素
    void InsertionSort(vector<int> &nums,int n){
        // end condition
        if (n<=1){ // 只有一个元素的时候
            return ;
        }
        else{
            // 首先需要一直递归分析 到最底层后 会开始回溯 逐步组合解的答案 即由1-》2-》3-》4逐步插入排序
            InsertionSort(nums,n-1);
        }
        // 需要先递归调用 到深入点后再回溯分析对应的代码情况
        int key = nums[n];
        int i=n-1;
        // 逐步判断 i与key的关系
        while(i>=0 && nums[i]>key){
            nums[i+1]=nums[i];
            i--;
        }
        nums[i+1]=key;
    }
    // 归并排序分析 基于每次一半元素 还需要写一个归并的过程
    void MergeSort(vector<int> &nums,int l,int r){
        // 边界情况分析
        if(l<r){
            int m = l+((r-l)/2);
            // 开始递归
            MergeSort(nums,l,m);
            MergeSort(nums,m+1,r);
            // 合并前文的两个分组代码
            Merge(nums,l,m,r);
        }
        else{
            return; // l>=r 时说明递归已经到最终点
        }
    }
    void Merge(vector<int> &nums,int l,int m, int r){
        // 复制数组l-m，m+1-r到两个不同的数组
        int left_elements = m-l+1; // 从l到m
        int right_elements = r-m; // 从m+1到r
        vector<int> left(left_elements);
        for(int i=0;i<left_elements;i++){
            left[i] = nums[l+i];
        }
        vector<int> right(right_elements);
        for(int i=0;i<right_elements;i++){
            right[i] = nums[m+1+i];
        }
        // 从l到r遍历分析
        int i=0,j=0,k=l;
        while(i<left_elements && j<right_elements){
            // 运用while循环，先将left-right对应都存在的部分都copy下来
            if (left[i]<right[j]){
                nums[k]=left[i];
                i++;
            }
            else{
                nums[k]=right[j];
                j++;
            }
            k++;
        }
        while(i<left_elements){
            nums[k]=left[i];
            i++;
            k++;
        }
        while(i<right_elements){
            nums[k]=right[j];
            j++;
            k++;
        }    
    }
    // 快速排序分析 基于元素的值进行分析 依据元素的值大小，随机划分为两个部分 每个部分里依次这样做 而后排序 => 包含对应的相等数组
    vector<int> QuickSort(vector<int> &nums){
        if (nums.size()<=1){
            return nums;
        }
        vector<int> SL;
        vector<int> mid;
        vector<int> SR;
        int mid_key = nums[nums.size()/2];
        for(int i=0;i<=nums.size()-1;i++){
            if(nums[i]>mid_key){
               SR.push_back(nums[i]);
            }
            else if (nums[i]==mid_key){
                mid.push_back(nums[i]);
            }
            else if (nums[i]<mid_key){
                SL.push_back(nums[i]);
            }
        }
        vector<int> SL_new = QuickSort(SL);
        vector<int> SR_new = QuickSort(SR);
        // 合并代码
        vector<int> result;
        for(int i:SL_new){
            result.push_back(i);
        }
        for(int i:mid){
            result.push_back(i);
        }
        for(int i:SR_new){
            result.push_back(i);
        }
        return result;
    }
    // 拓展 逆序对计数  找逆序对

    // hard629. K Inverse Pairs Array 构建逆序对
    
    // 以及第k小的数 215
    int findKthLargest(vector<int>& nums, int k) {
        // 基于值的快速排序，分别判断左右的元素大小
        // 终止条件：
        int mid = nums[nums.size()/2];
        vector<int> SL,SR,Mid;
        for(int i=0;i<nums.size();i++){
            if(nums[i]<mid){
                SL.push_back(nums[i]);              
            }
            else if(nums[i]==mid){
                Mid.push_back(nums[i]);
            }
            else if(nums[i]>mid){
                SR.push_back(nums[i]);
            }
        }
        if (SR.size() == k - 1) {return mid;} 
        else if (SR.size() > k - 1) {return findKthLargest(SR, k);} 
        else if (SR.size()<k-1 && SR.size()+Mid.size()>k-1){return mid;}
        else{// 注意更新k为k - SL.size() - Mid.size()
        return findKthLargest(SL, k - SR.size() - Mid.size());}    
    }
    // 冒泡排序 在数据完全有序的时候展现出最优时间复杂度，为O(n)。其他情况下，几乎总是O( n2 )
    void MaopaoSort(vector<int> &nums){
        for(int i=nums.size()-1;i>=0;i--){
            // 分析有多少次冒泡，因为一次冒泡会使得一个最大的数到最后
            for(int j=0;j<i;j++){ // j<i 则对应的内容j+1 不会越界
                if(nums[j]>nums[j+1]){
                    int tmp = nums[j+1];
                    nums[j+1] = nums[j];
                    nums[j] = tmp;
                }
            }
        }
    }
    // 选择排序 每次选取最小值放到最前面
    void SelectSort(vector<int> &nums){
        for(int i=0;i<nums.size();i++){
            int min = i;
            for(int j=i+1;j<nums.size();j++){
                if (nums[min]>nums[j]){
                    min = j;
                }
            }
            if(min!=i){
                // 需要更换一下最前面的数组值大小
                int tmp = nums[i];
                nums[i] = nums[min];
                nums[min] = tmp;
            }
        }
    }
};

// 基本数据结构分析
// 链表 
struct ListNode{
    int val;  // 成员
    ListNode *next; // 指针成员
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {} 
    // *head.val ;head->val;
};
// 树
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
//  map 哈希表组织形式 
// 数组  
// set （集合） set,multiset 红黑树（平衡二叉搜索树，key有序） // unordered_set 哈希表 无序 不可重复 O（1）查询和增删效率
// map  (映射)  map,multimap // unordered_map 哈希表 无序 不可重复 O（1）查询和增删效率

// 字符串 string！！==》当做vector用即可 迭代器

// 栈stack与队列queue 
// 栈是以底层容器完成其所有的工作，对外提供统一的接口，底层容器是可插拔的（也就是说我们可以控制使用哪种容器来实现栈的功能）。
// 栈不提供走访功能，也不提供迭代器(iterator) 所以STL中栈往往不被归类为容器，而被归类为container adapter（容器适配器）
// 队列queue :  push(x) -- 将一个元素放入队列的尾部。pop() -- 从队列首部移除元素。peek() -- 返回队列首部的元素。empty() -- 返回队列是否为空。
// 栈  stack :  push(x) -- 元素 x 入栈 pop() -- 移除栈顶元素 top() -- 获取栈顶元素 empty() -- 返回栈是否为空



class ListNode_Solution{
    public:
    ListNode* RemoveNode(ListNode *head,int target){
        ListNode *dummyhead = new ListNode(0);
        dummyhead->next = head;
        // 循环移除节点
        ListNode* cur = dummyhead;
        while(cur->next!=nullptr){
            if(cur->next->val==target){
                ListNode* tmp = cur->next; // 暂时存储节点 以方便后续分析
                cur->next = cur->next->next;
                delete tmp;
            }
            else{
                cur=cur->next; // 往后移动分析 
            }
        }
        head = dummyhead->next;
        delete dummyhead;
        return head; 
    }
    // 翻转链表 双指针移动 
    ListNode* ReverseNode(ListNode* head){
        ListNode* pre = nullptr;
        ListNode* cur = head;
        ListNode* tmp;
        while(cur){
            tmp = cur->next;
            cur->next = pre;
            pre = cur;
            cur = tmp;
        }
        return pre; // 本意最后应该是cur，但是cur赋给pre了，现在的cur是nullptr；故而这样是对的；
    }
};

int main(){
    basic_sort s;
    vector<int> a={6,5,4,3,2,1};
    s.InsertionSort(a,5);
    cout<<"begin"<<endl;
    for(int i:a){
        cout<<i<<endl;
    }
}