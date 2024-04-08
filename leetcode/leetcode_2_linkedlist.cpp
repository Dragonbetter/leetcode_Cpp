#include <iostream>

/*
 链表基础知识
哑巴节点，栈，快慢指针
nullptr 和 NULL有什么区别？
在C++中，nullptr和NULL都用于表示指针不指向任何对象。它们的主要区别在于类型和使用上的安全性。
类型:
NULL 在C++中通常被定义为0或者((void*)0)。它是一个预处理宏，用于表示空指针。由于它是一个整型值，有时可能会导致类型混淆的问题。
nullptr 是C++11引入的一个新的关键字，用于表示空指针。它的类型是std::nullptr_t，可以自动转换为任何类型的指针，但不能转换为非指针类型或布尔类型。
*/
//Definition for singly-linked list.
struct ListNode {
     int val;// 节点上存储的元素
     ListNode *next;  // 指向下一个节点的指针
     // 多个不同的构造函数 
     ListNode() : val(0), next(nullptr) {}
     ListNode(int x) : val(x), next(nullptr) {}
     ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
    // leeetcode 203 Remove Linked List Elements 12.07
    ListNode* removeElements_1(ListNode* head, int val) {
        // 直接使用原来的链表来进行移除节点操作
        // 分别考虑头结点和其他节点两种情况
        while(head!=NULL && head->val==val){
            // 表明链表非空 如果头结点值相当
            ListNode* tmp=head;
            head = head->next;
            delete tmp; // 指针指向相同的地方相同 删除tmp同时删除了原先的head？
            tmp = nullptr;
        }
        // 删除非头节点
        ListNode* cur=head;
        while(cur!=NULL && cur->next!=NULL){
            // 本身非空 而且还不到最后一个节点 
            if(cur->next->val==val){
                // tmp 指针在这里是必要的，因为它用于临时保存要删除的节点的地址。
                // 如果不使用 tmp 指针，一旦更新了 cur->next 的值，原本 cur->next 所指向的节点就无法被访问和正确释放，导致内存泄漏。
                ListNode* tmp=cur->next;
                cur->next = cur->next->next;
                delete tmp; // 为了释放内存而准备的
                tmp = nullptr;
            }
            else{
                cur=cur->next;
                // 往后遍历 
            }
        }
        return head;
    }
    // 设置一个虚拟头结点在进行移除节点操作： !! 统一操作较为的方便 速度也从原来的19ms加快到14ms 12.07
    ListNode* removeElements_2(ListNode* head, int val) {
        ListNode* dummyhead = new ListNode(0); // 设置一个虚拟头节点 val=0 构造函数 new动态分配的
        dummyhead->next=head; // 将虚拟头结点指向head，方便后面的删除操作
        ListNode* cur=dummyhead; // 设置一个cur便于后期的循环 不影响头结点 
        while(cur->next!=NULL){
            if(cur->next->val==val){
                ListNode* tmp=cur->next;
                cur->next = cur->next->next;
                delete tmp;
                tmp = nullptr;
            }
            else{
                cur = cur->next;
            }
        }
        // 明确真实的链表仍然是dummyhead->next指向的部分 故而需要显示明确并delete
        head = dummyhead->next;
        delete dummyhead;
        dummyhead = nullptr;
        return head;
    }
    ListNode* reverseList_1(ListNode* head) {
        // 206. Reverse Linked List 12.11
        /*逻辑顺序关系 先取到下一个元素  首先定义一个cur指针，指向头结点，再定义一个pre指针，初始化为null。
        后就要开始反转了，首先要把 cur->next 节点用tmp指针保存一下，也就是保存一下这个节点。
        接下来要改变 cur->next 的指向了，将cur->next 指向pre ，此时已经反转了第一个节点了。
       */
       // 双指针法
       ListNode* temp = nullptr;
       // 一般而言指针未初始化的指针含有一个垃圾值，即它指向一个随机的内存地址。
       // 这种情况下，如果你尝试通过这个指针访问或修改数据，可能会导致不可预测的行为，甚至程序崩溃
       // 但此处在使用该指针前 其已经被赋予新的值了 故而只要确保在使用之前赋予变量一个有效的值，就可以避免未初始化变量带来的问题。
       ListNode* cur=head;
       ListNode* pre=nullptr;
       while(cur){
        temp = cur->next; // 保存cur的下一个节点
        cur->next = pre;// 翻转操作
        // 更新 pre和cur的指针
        pre = cur;
        cur = temp;
       }
       // 当NULL为空时说明已经到达末尾， 故而返回相应的pre
       return pre;
   }
    ListNode* reverse(ListNode* pre,ListNode* cur){
       if(cur==NULL) return pre;
       ListNode* temp=cur->next;
       cur->next = pre;
       // 逐步递归移动 
       return reverse(cur,temp);
   }
    ListNode* reverseList_2(ListNode* head){
    // 递归法  每一层递归都翻转了一次链表 而最深层的递归显示了头结点所在
    return reverse(NULL,head);
    }
    ListNode* swapPairs(ListNode* head) {
        // leetcode 24 Swap Nodes in Pairs 12.11
        // 需要实际的交换节点 也就是还需要交互他的指针指向 单数与双数 
        /*
           初始写法未考虑对应的后一组内部指针对转后 前一组指针指向的地方仍然未变 即用两个指针的方法选取的指针不对 
           此处需要保留不同组之间的关系  这样在处理1234时 结果会为21 3 因为在转变之后 1始终指的是3 虽然3和4已经转变了
        ListNode* temp;
        ListNode* cur=head->next;
        ListNode* new_head = head->next;
        ListNode* pre=head;
        while(pre!=nullptr && cur!=nullptr){
            // 开始两元素逐对翻转
            temp = cur->next;
            cur->next = pre;
            pre->next = temp;
            pre = temp;
            if(temp!=nullptr){
                cur = temp->next;
            }
        }
        */
       ListNode* dummyhead = new ListNode(0);
       dummyhead->next =head;
       ListNode* cur=dummyhead;
       // &&的保护机制 先计算左边 如果左边成立 则右边不会执行 保证了不会出现 nullptr-next的情况
       while(cur->next!=nullptr && cur->next->next!=nullptr){
            // 注意保存和记录的节点 选择 
            // 注意保存好对应的节点，以及相应的转变顺序 
            ListNode* tmp = cur->next;
            ListNode* tmp_after = cur->next->next->next; // 指的是两两一组的下一组首部
            // 开始翻转 依次进行
            cur->next = cur->next->next; // 第一步直接进行 此步进行完 后两步cur的指向就完全变了 故而需要先保存下来
            cur->next->next = tmp;
            cur->next->next->next = tmp_after;// 此处不变的 仍然指向后一组的首部
            // 移动cur到新的位置 应该指的仍然是下一组的上一个 如此 能保证 next next-next next-next-next
            cur = cur->next->next;
       }
       head = dummyhead->next;
       delete dummyhead;
       dummyhead = nullptr;
       return head;
    }
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        //leetcode 19 12-13  Remove Nth Node From End of List
        // Follow up: Could you do this in one pass? 
        //链表需要逐步到达底部  简单方法 遍历第一遍找到size 第二遍找到第n个
        // 双指针法：快指针比慢指针快n步 当快指针为NULL时 慢指针指向的正好是需要删除的元素
        // 因为dummyhead在后续直接使用 故而前期在定义时需要初始化 其new一个空间地址 将其赋予dummyhead
        ListNode* dummyhead = new ListNode(0);
        // 尽量定义虚拟头结点 较好用 
        dummyhead->next = head;
        ListNode* slow = dummyhead;
        ListNode* fast = dummyhead;
        // 确定fast指针的位置  因为1<=n<=sz 1<=sz<=30 
        for (int i=n;i>0;i--){
            fast = fast->next;
        }
        // 同步两个指针一起运行 直到fast指向NULL
        // 如果判断fast本身是否为空 则需要fast走n+1步 如果是fast->next 则需要走n步即可
        while(fast->next!=nullptr){
            fast=fast->next;
            slow=slow->next;
        }
        // 到达指定位置 
        ListNode* tmp = slow->next;
        slow->next = slow->next->next;
        delete tmp;
        tmp = nullptr;  
        head = dummyhead->next;
        delete dummyhead;
        dummyhead = nullptr;  
        return head;    
    }
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        // leetcode 160 12-13 Intersection of Two Linked Lists
        // 交点需要后面的数据完全一样 两个指针 指向不同的链表 首先找到两个指针的元素值相同 而后继续判断 如果一直相同直到终点 则找到 
        // 否则重新开始计算  =》 有一个问题在于如果长的序列中有多段都与短序列的匹配怎么办 ？ 短的已经到尾部 但长的仍然还有很多剩下 
        // 而此处的不仅仅是指相同 应该是指针相同 即后续的链表到末尾完全相同 
        // 先计算出两个序列各自的长度 然后把长序列的指针移到与短序列一样长的地方 而后开始进行上述原始方法的比较

        /*
        TODO  新方法
        我们需要做的事情是，让两个链表从同距离末尾同等距离的位置开始遍历。这个位置只能是较短链表的头结点位置。
为此，我们必须消除两个链表的长度差

指针 pA 指向 A 链表，指针 pB 指向 B 链表，依次往后遍历
如果 pA 到了末尾，则 pA = headB 继续遍历
如果 pB 到了末尾，则 pB = headA 继续遍历
比较长的链表指针指向较短链表head时，长度差就消除了
如此，只需要将最短链表遍历两次即可找到位置       
        */
        int a=0,b=0;
        ListNode* tmpA = headA;
        ListNode* tmpB = headB;
        while(tmpA!=nullptr){
            tmpA=tmpA->next;
            a++;
        }
        while(tmpB!=nullptr){
            tmpB=tmpB->next;
            b++;
        }
        // 移动指针 对齐
        tmpA = headA;
        tmpB = headB;
        if(a>=b){
            for (int n=a-b;n>0;n--){
                tmpA=tmpA->next;
            }
        }
        else {
            for(int n=b-a;n>0;n--){
                tmpB=tmpB->next;
            }
        }
        // 开始查询指针的是是否相等 
        // 需要注意是指针相等 而不是值相等 故而需要用的是 tmpA_1==tmpB_1 而不是 tmpA_1->val==tmpB_1->val
        ListNode* elements = tmpA;
        /*
        其实这处的判断不用如此复杂 只要指向指向相同 后续的必然相同 在该题的特定情况下
        while (curA != NULL) {
            if (curA == curB) {
                return curA;
            }
            curA = curA->next;
            curB = curB->next;
        }
        return NULL;
        */
        int count = 0;
        while(tmpA!=nullptr || tmpB!=nullptr){
            if(tmpA!=tmpB){
                elements = NULL;
                count=0;
            }
            else if (tmpA==tmpB && count==0){
                elements = tmpA;
                count=1;
            }
            else if (tmpA==tmpB && count!=0){
                count+=1;
            }
            tmpA=tmpA->next;
            tmpB=tmpB->next;
        }
        return elements;
    }
    ListNode *detectCycle(ListNode *head) {
        // leetcode 142 Linked List Cycle II 12-13
        /*TODO 后续思考 纯数学思考形式 优点过于复杂 后续如果面试或经验看到这个再花时间学习
        空间复杂度 O(1) ：双指针使用常数大小的额外空间
        1.判断链表是否环:
          快慢指针法，分别定义 fast 和 slow 指针，从头结点出发，
          fast指针每次移动两个节点，slow指针每次移动一个节点，如果 fast 和 slow指针在途中相遇 ，说明这个链表有环
          fast指针一定先进入环中，如果fast指针和slow指针相遇的话，一定是在环中相遇，这是毋庸置疑的。
          其实相对于slow来说，fast是一个节点一个节点的靠近slow的，
        2.如果有环，如何找到这个环的入口:
        */
       ListNode* fast = head;
        ListNode* slow = head;
        while(fast != NULL && fast->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
            // 快慢指针相遇，此时从head 和 相遇点，同时查找直至相遇
            if (slow == fast) {
                ListNode* index1 = fast;
                ListNode* index2 = head;
                while (index1 != index2) {
                    index1 = index1->next;
                    index2 = index2->next;
                }
                return index2; // 返回环的入口
            }
        }
        return NULL;
    }
};
class MyLinkedList {
    // leetcode 707  12.07
    // 实现一个完整的链表类的功能 首尾添加元素 返回元素 在指定点添加元素 
public:
    MyLinkedList() {
        _dummyHead = new ListNode(0);
        _size = 0;  
    }
    
    int get(int index) {
        // 获取链表中第 index 个节点的值。如果索引无效，则返回-1
        if ((index>_size-1) || (index<0)){
            return -1;
        }
        ListNode* cur=_dummyHead->next;
        while(index--){
            // 注意此处 如果--index会多减一个 使得错误 会陷入死循环？？
            cur = cur->next;
        }
        return cur->val;
    }
    
    void addAtHead(int val) {
        //在链表的第一个元素之前添加一个值为 val 的节点。插入后，新节点将成为链表的第一个节点。
        ListNode * newnode = new ListNode(val);
        newnode->next = _dummyHead->next;
        _dummyHead->next = newnode;
        _size++;
    }
    
    void addAtTail(int val) {
        // 将值为 val 的节点追加到链表的最后一个元素。
        // 单向节点 则需要遍历到最后的位置
        ListNode* newnode = new ListNode(val);
        // ListNode* cur = _dummyHead->next; 此处不选next是因为需要考虑一个元素都没有的情况 则是cur本身就是NULL 无next
        ListNode* cur = _dummyHead;
        while(cur->next!=nullptr){
            cur = cur->next;
        }
        cur->next = newnode;
        _size++;
    }
    
    void addAtIndex(int index, int val) {
        // 在第index个节点之前插入一个新节点，例如index为0，那么新插入的节点为链表的新头节点。
        // 如果index 等于链表的长度，则说明是新插入的节点为链表的尾结点
        // 如果index大于链表的长度，则返回空
        // 如果index小于0，则在头部插入节点
        if (index>_size){return;}
        if (index<0){index=0;}
        ListNode* newnode= new ListNode(val);
        ListNode* cur = _dummyHead;
        while(index--){
            cur = cur->next;
        }
        newnode->next = cur->next;
        cur->next = newnode;
        _size++;  
    }
    
    void deleteAtIndex(int index) {
        // 如果索引 index 有效，则删除链表中的第 index 个节点
        if(index<0 || index>=_size){
            return;
        }
        ListNode* cur=_dummyHead;
        while(index--){
            cur=cur->next;
        }
        ListNode* tmp=cur->next;
        cur->next = cur->next->next;
        delete tmp;
        _size--;
        // 空悬指针 delete删除了指针指向的内存 但指针仍然指向哪块已经被释放的内存
        //delete命令指示释放了tmp指针原本所指的那部分内存，
        //被delete后的指针tmp的值（地址）并非就是NULL，而是随机值。也就是被delete后，
        //如果不再加上一句tmp=nullptr,tmp会成为乱指的野指针
        //如果之后的程序不小心使用了tmp，会指向难以预想的内存空间
        tmp=nullptr;
    }
private:
    // _size 共有的 同时更改
    int _size;
    ListNode* _dummyHead;
};
 
