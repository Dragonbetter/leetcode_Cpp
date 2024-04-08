#include <vector>
#include <stack>
#include <algorithm>
#include <queue>
#include <string>
#include <unordered_map>

// 定义二叉树数据结构
struct TreeNode{
    int val;
    TreeNode * left;
    TreeNode * right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x):val(x),left(nullptr),right(nullptr){}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
// 定义N叉树数据结构
class N_Node {
public:
    int val;
    std::vector<N_Node*> children;

    N_Node() {}

    N_Node(int _val) {val = _val;}

    N_Node(int _val, std::vector<N_Node*> _children) {val = _val;children = _children;}
    // 针对与N叉树数据结构的设计  -> 定义一个vector存储下续节点 而不是单独两个 
};
// 具有next指针的二叉树 
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};

class Solution{
    public:
    // 12.26
    // 递归法实现 前中后序遍历
    void travelsal_preorder(TreeNode *cur,std::vector<int> & vec){
        // 前序遍历
        // 终止条件
        if(cur==nullptr) return;
        vec.push_back(cur->val);
        travelsal_preorder(cur->left,vec);
        travelsal_preorder(cur->right,vec);
    }
    void travelsal_inorder(TreeNode *cur,std::vector<int> & vec){
        // 中序遍历
        // 终止条件
        if(cur==nullptr) return;
        travelsal_inorder(cur->left,vec);
        vec.push_back(cur->val);
        travelsal_inorder(cur->right,vec);
    }
    void travelsal_posterorder(TreeNode *cur,std::vector<int> & vec){
        // 后序遍历
        // 终止条件
        if(cur==nullptr) return;
        travelsal_posterorder(cur->left,vec);
        travelsal_posterorder(cur->right,vec);
        vec.push_back(cur->val);
    }
    std::vector<int> Binary_Traversal(TreeNode * root){
        std::vector<int> result;
        travelsal_posterorder(root,result);
        return result;
    }
    // 迭代法实现 前中后序遍历
    std::vector<int> travelsal_preorder_iteration(TreeNode* root){
        // 迭代实现 运用stack 注意出入栈的顺序以及位置
        std::stack<TreeNode*> stIn;
        std::vector<int> vec;
        if(root==nullptr){
            return vec;
        }
        stIn.push(root);
        while(!stIn.empty()){
            // 弹出节点 并获取值 最终加入节点对应的左右树结构！！ 
            // 每次访问到 即可取出对应的节点 而后继续往下访问 压入栈
            // 因为中序遍历的 节点存储域访问不是同步的 访问到中心节点但却不需要将其加入进去 要等待到最后的左边
            TreeNode* node = stIn.top();
            stIn.pop();
            vec.push_back(node->val);
            if(node->right) stIn.push(node->right);
            if(node->left) stIn.push(node->left);
        }
        return vec;
    }
    std::vector<int> travelsal_inorder_iteration(TreeNode* root){
        std::stack<TreeNode*> stIn;
        std::vector<int> result;
        // 中序遍历的过程 左中右 即需要一直深入左节点 直到最终 而后再返回相应的节点
        TreeNode* cur = root;
        // 如何判断 终止条件： 栈为空（即不需要元素加入vector） 而且无新的节点可以入栈
        while(cur!=nullptr || !stIn.empty()){
            if(cur!=nullptr){           // 指针来访问节点，访问到最底层
                stIn.push(cur);         // 将访问的节点放进栈 cur非空才能进栈 故而cur为空时存储的是其父类节点的数据
                cur = cur->left;    // 左
            }
            else{
                // 开始反向读取数据 cur 为空 故而反过来去寻找最近压入栈的 对应的应该是其父节点 
                cur = stIn.top();
                stIn.pop();
                result.push_back(cur->val);
                cur = cur->right;
                 // 读取完 即当前节点的左节点为空 故而继续去寻找当前节点的右节点 
                 // 同时此时该节点不会继续存在于stack中 故而如果右边也无 则返回继续往上 
            }
        }
        return result;
    }
    std::vector<int> travelsal_posterorder_iteration(TreeNode* root){
        // 迭代实现 运用stack 注意出入栈的顺序以及位置
        std::stack<TreeNode*> stIn;
        std::vector<int> vec;
        if(root==nullptr){
            return vec;
        }
        stIn.push(root);
        while(!stIn.empty()){
            // 每次访问到 即可取出对应的节点 而后继续往下访问 压入栈
            TreeNode* node = stIn.top();
            stIn.pop();
            vec.push_back(node->val);
            // 前序遍历是 中左右 -》后续遍历的是左右中 那么翻转一下就是中右左 故而 需要先入左边
            if(node->left) stIn.push(node->left);
            if(node->right) stIn.push(node->right); 
        }
        reverse(vec.begin(),vec.end());
        return vec;
    }
    // TODO ！！ 统一遍历顺序 那我们就将访问的节点放入栈中，把要处理的节点也放入栈中但是要做标记。
    // 如何标记呢，就是要处理的节点放入栈之后，紧接着放入一个空指针作为标记。 这种方法也可以叫做标记法。
    std::vector<int> inorderTraversal(TreeNode* root) {
        // 主要需要理解的是 在二叉树便利的过程中 其实中间节点才是需要处理的角色 中间节点的往下的左右节点也是下一个的中间节点 故而逻辑处理只在针对中间节点这一块
        std::vector<int> result;
        std::stack<TreeNode*> st;
        if (root != NULL) st.push(root);
        while (!st.empty()) {
            // 每次都对中间节点所代表的的小二叉树进行处理
            TreeNode* node = st.top();
            if (node != NULL) {
                st.pop(); // 将该节点弹出，避免重复操作，下面再将右中左节点添加到栈中
                /*
                    此处为前序 但是中序与后序只需要修改此处的入栈顺序即可 
                    前：
                    if (node->right) st.push(node->right);  // 右
                    if (node->left) st.push(node->left);    // 左
                    st.push(node);                          // 中
                    st.push(NULL);
                    后：
                    st.push(node);                          // 中
                    st.push(NULL);
                    if (node->right) st.push(node->right);  // 右
                    if (node->left) st.push(node->left);    // 左
                */
                if (node->right) st.push(node->right);  // 添加右节点（空节点不入栈）

                st.push(node);                          // 添加中节点
                st.push(NULL); // 中节点访问过，但是还没有处理，加入空节点做为标记。

                if (node->left) st.push(node->left);    // 添加左节点（空节点不入栈）
            } else { // 只有遇到空节点的时候，才将下一个节点放进结果集
                st.pop();           // 将空节点弹出
                node = st.top();    // 重新取出栈中元素
                st.pop();
                result.push_back(node->val); // 加入到结果集
            }
        }
        return result;
    }
    // 二叉树层序遍历  Binary Tree Level Order Traversal  12.26
    //DFS--递归方式
    void order(TreeNode* cur,std::vector<std::vector<int>> &result,int depth){
        // 递归遍历 需要借助 depth确定输入的层级
        // 终止条件
        if(cur==nullptr) return;
        // 如果第一次进入该元素层 则需要为其建立一个空vector用于存放数据 
        if(result.size() == depth) result.push_back(std::vector<int>());
        // 先本身节点 而后左右节点
        result[depth].push_back(cur->val); // 对应层级进行添加元素
        order(cur->left,result,depth+1);
        order(cur->right,result,depth+1);
    }
    std::vector<std::vector<int>> levelOrder_dfs_recursion(TreeNode*root){
        std::vector<std::vector<int>> result;
        int depth=0;
        order(root,result,depth);
        return result;
    }
    //BFS--迭代方式--借助队列
    std::vector<std::vector<int>> levelOrder_dfs_iteration(TreeNode*root){
        std::vector<std::vector<int>> result;
        std::queue<TreeNode*> que;
        // 需要对队列中的每个元素进行替换
        if(root!=nullptr)  que.push(root);
        while(!que.empty()){
            int size = que.size(); // 在构建过程中que会变换
            std::vector<int> vec; // 用于存储这一层的数据结果
            // 队列的先入先出 正好可以在循环的过程中 既取上层数据 并pop 又同时添加下层数据
            for(int i=0;i<size;i++){
                TreeNode* cur = que.front();
                que.pop();
                vec.push_back(cur->val);
                //进队列
                if(cur->left) que.push(cur->left);
                if(cur->right) que.push(cur->right);
            }
            result.push_back(vec);
        }
        return result;
    }
    std::vector<std::vector<int>> levelOrderBottom(TreeNode* root) {
        // leetcode 107. Binary Tree Level Order Traversal II 12.27
        // 先顺序返回list 然后 再 reverse vector
        // 思路：运用queue去进出 
        // 需要注意如何基于vector数组 构建处完整的树结构 
        std::vector<std::vector<int>> result;
        std::queue<TreeNode*> que;
        if(root!=nullptr)  que.push(root);
        // 进行循环 终止条件应该是 
        while(!que.empty()){
            int que_size = que.size();
            std::vector<int> vec;
            for(int i=0;i<que_size;i++){
                TreeNode *cur = que.front();
                que.pop();
                vec.push_back(cur->val);
                if(cur->left) que.push(cur->left);
                if(cur->right) que.push(cur->right);
            }
            result.push_back(vec);
        }
        // 反转
        std::reverse(result.begin(),result.end());
        return result;
    }
    std::vector<int> rightSideView_levelOrder_dfs(TreeNode* root) {
        // leetcode 199 Binary Tree Right Side View
        // 先层序遍历 获取完整的vector 而后每次挑选vector中最后的元素
        // 其实在每次加的时候可以直接只加最后一个元素 
        std::vector<std::vector<int>> full_result;
        full_result = levelOrder_dfs_iteration(root);
        std::vector<int> result;
        for(std::vector<int> vec:full_result){
            std::vector<int>::iterator end = vec.end()-1;
            result.push_back(*end);
        }
        return result;
    }
    std::vector<int> rightSideView_dfs_onetime(TreeNode* root){
        // leetcode 199 Binary Tree Right Side View 在调用的过程中即获取右边的节点 
        // O(n)
        std::vector<int> result;
        std::queue<TreeNode*> que;
        // 需要对队列中的每个元素进行替换
        if(root!=nullptr)  que.push(root);
        while(!que.empty()){
            int size = que.size(); // 在构建过程中que会变换
            // 队列的先入先出 正好可以在循环的过程中 既取上层数据 并pop 又同时添加下层数据
            for(int i=0;i<size;i++){
                TreeNode* cur = que.front();
                que.pop();
                if(i==size-1){
                    result.push_back(cur->val);
                }
                //进队列
                if(cur->left) que.push(cur->left);
                if(cur->right) que.push(cur->right);
            }
        }
        return result;
    }
    Node* connect_BFS(Node* root) {
        // leetcode 116 117   Populating Next Right Pointers in Each Node II 12.27
        /*
            TODO 二刷的时候再来看DFS以及O（1）的next链表方法
            116 完整二叉树 117 普通二叉树 （存在问题 -> ）
            BFS可以忽略各种空节点的问题，故而此处是否是完整二叉树 无影响
            DFS的递归是有区别的 
            如果这道题使用BFS的话那么和116没有任何区别，因为BFS可以忽略各种空节点造成的影响，
            然而如果想要尝试使用递归的话，限制就比较多了，因为这并不是一颗完美二叉树（各个节点可能都只有一个孩子），
            这会导致我们无法轻松判断应该将当前节点的孩子连接到右侧的哪个节点上，
            因为有可能是当前节点的next的左右子节点也有可能是next和next的，
            因此基于这种思想可以定义一个递归函数不断向右进行搜索直到搜索到对应的节点进行返回，
            这是一个难点，此外需要注意的是先处理当前节点的左右孩子之间的连接然后处理层之间的连接，
            最后在递归的时候必须先递归右子树，否则无法保证递归到左侧的时候右侧的节点都是相连的。
            二刷的时候BFS做法完全没问题，递归做法想不到，还是忽略了整体思想”递归到当前层的时候将下一层链接起来“，
            而这个思想的本质就是每一个节点只需要考虑对左右子节点通过遍历上一层的时候已经把这一层链接好了，
            因此可以通过当前层的连接来得到子节点层第一个非空的next节点。
        */
        // 依然是层序遍历 只是返回的不再是vector 而是一个新的二叉树结构 
        // 只不过在单层遍历的时候记录一下本层的头部节点，然后在遍历的时候让前一个节点指向本节点就可以了
        std::queue<Node*> que;
        if (root != NULL) que.push(root);
        while (!que.empty()) {
            int size = que.size();
            // vector<int> vec;
            Node* nodePre;
            Node* node;
            for (int i = 0; i < size; i++) {
                if (i == 0) {
                    nodePre = que.front(); // 取出一层的头结点
                    que.pop();
                    node = nodePre;
                } else {
                    node = que.front();
                    que.pop();
                    nodePre->next = node; // 本层前一个节点next指向本节点
                    nodePre = nodePre->next;
                }
                if (node->left) que.push(node->left);
                if (node->right) que.push(node->right);
            }
            nodePre->next = NULL; // 本层最后一个节点指向NULL
        }
        return root;
    }
    Node* getNext(Node* root){
        if(root==NULL){
            return NULL;
        }
        if(root->left!=NULL){
            return root->left;
        }
        if(root->right!=NULL){
            return root->right;
        }
        return getNext(root->next);
    }
    Node* connect_DFS(Node* root) {
        if(root==NULL){return root;}
        if(root->left!=NULL&&root->right!=NULL){
                root->left->next=root->right;}
        if(root->left!=NULL&&root->right==NULL){
                root->left->next=getNext(root->next);
        }
        if(root->right!=NULL){
                root->right->next=getNext(root->next);
        }
        connect_DFS(root->right);
        connect_DFS(root->left);
        return root;
    }     
    Node* connect(Node* root) {
        // 而既然有这种思想那么其实就可以直接利用当前层的连接关系来直接使用BFS把下一层连接起来，只需要引入一个dummy节点即可。
        //cur是当前层的节点
        Node* cur=root;
        while(cur){
            //dummy是下一层的虚拟头节点 temp是下一层的节点
            Node *dummy=new Node();
            Node *temp=dummy;
            //整体思路是在遍历cur的基础上利用temp把所有的cur的子节点链接起来
            while(cur){
                if(cur->left){
                    temp->next=cur->left;
                    temp=temp->next;
                }
                if(cur->right){
                    temp->next=cur->right;
                    temp=temp->next;
                }
                cur=cur->next;
            }
            cur=dummy->next;
            delete dummy;
        }
        return root;
    }
    int maxDepth(TreeNode* root) {
        // leetcode 104 Maxmum Depth of Binary Tree  12.28
        // 法1 层序遍历 最终的层数即为max
        // 法2 深度遍历 二叉树的最大深度为左子树和右子树的最大深度+1 即max（l,r）+1 后序 
        // 终止条件即为节点为nullptr 此时返回0，如果某一个root的左右都为空，则返回0后 +1 正好等于最终正确答案
        if(root==nullptr) return 0;
        return std::max(maxDepth(root->left),maxDepth(root->right))+1;
    }
    int result;
    void getdepth(TreeNode* node, int depth) {
        result = depth > result ? depth : result; // 中

        if (node->left == NULL && node->right == NULL) return ;

        if (node->left) { // 左
            depth++;    // 深度+1
            getdepth(node->left, depth);
            depth--;    // 回溯，深度-1
        }
        if (node->right) { // 右
            depth++;    // 深度+1
            getdepth(node->right, depth);
            depth--;    // 回溯，深度-1
        }
        return ;
    }
    int maxDepth_posterorder(TreeNode* root) {
        result = 0;
        if (root == NULL) return result;
        getdepth(root, 1);
        return result;
    }



    int minDepth_iteration(TreeNode* root) {
        // 层序遍历 如果发现某个节点两边都为0，则结束
        // 时间复杂度：O(N)，其中 N 是树的节点数。对每个节点访问一次。

      //空间复杂度：O(N)，其中 N是树的节点数。空间复杂度主要取决于队列的开销，队列中的元素个数不会超过树的节点数
        int depth = 0;
        std::queue<TreeNode* > que;
        if(root!=nullptr){que.push(root);}
        else {return 0;}
        while(!que.empty()){
            int size = que.size();
            depth++; // 注意depth++放的位置 
            for(int i=0;i<size;i++){
                TreeNode* cur = que.front();
                que.pop();
                // 需要注意的是 相应的节点可能左边有 但右边没有 而且depth不能在每一个for内都++ 应该用一个count 去表述是否存在min
                if(cur->left) que.push(cur->left);
                if(cur->right) que.push(cur->right);
                // 当左右孩子都为空的时候，说明是最低点的一层了，退出
                if(!cur->left && !cur->right){return depth;}
            }
        }
        return depth;
    }
    int minDepth(TreeNode* root){
        /*
        ==> 最小深度是从根节点到最近叶子节点的最短路径上的节点数量。注意是叶子节点。
        什么是叶子节点，左右孩子都为空的节点才是叶子节点;
            最大深度和最小深度都可以运用递归 即当前节点的深度都是等于左右节点深度+1；
            写出结束条件
            不要把树复杂化，就当做树是三个节点，根节点，左子节点，右子节点
            只考虑当前做什么，不用考虑下次应该做什么
            每次调用应该返回什么
        */
        //这道题递归条件里分为三种情况
        //1.左孩子和有孩子都为空的情况，说明到达了叶子节点，直接返回1即可
        if(root==nullptr) return 0;
        if(root->left == nullptr && root->right==nullptr) return 1;
        //2.如果左孩子和由孩子其中一个为空，那么需要返回比较小的那个孩子的深度
        int left = minDepth(root->left);
        int right = minDepth(root->right);
        //这里其中一个节点为空，说明m1和m2有一个必然为0，所以可以返回m1 + m2 + 1;
        /*
            如果这样写 则没有左孩子的分支会算为最短深度；
            int leftDepth = getDepth(node->left);
            int rightDepth = getDepth(node->right);
            int result = 1 + min(leftDepth, rightDepth);
            return result;
            如果左子树为空，右子树不为空，说明最小深度是 1 + 右子树的深度。
            反之，右子树为空，左子树不为空，最小深度是 1 + 左子树的深度。 
            最后如果左右子树都不为空，返回左右子树深度最小值 + 1 。
        */
        if(root->left==nullptr || root->right == nullptr) return left+right+1;
        //3.最后一种情况，也就是左右孩子都不为空，返回最小深度+1即可
        return std::min(left,right)+1;
    }
    TreeNode* invertTree_preorder_recursion(TreeNode* root) {
        // leetcode 226 翻转二叉树 12.28 
        // 递归翻转 针对当前节点 翻转其左右 翻转完后 继续进入去翻转左右 依次进行
        // 终止条件：节点为空 --前序遍历
        if(root==nullptr) return root;
        TreeNode* temp = root->left;
        root->left = root->right;
        root->right = temp;
        invertTree_preorder_recursion(root->left);
        invertTree_preorder_recursion(root->right);
        return root;
    }
    TreeNode* invertTree_iteration(TreeNode* root) {
        // 迭代法 前序迭代 用的是栈 每次取一个出来 而后再将其左右分别入栈 
        // 同样的方法还可以用 层序遍历 相应的 在层序的过程中 翻转每一个节点的左右 
        if(root==nullptr) return root;
        std::stack<TreeNode*> st;
        st.push(root);
        while(!st.empty()){
            TreeNode* temp = st.top();              // 中
            st.pop();
            std::swap(temp->left,temp->right);
            if(temp->right) st.push(temp->right);   // 右
            if(temp->left) st.push(temp->left);     // 左
        }
        return root;
    }
    TreeNode* invertTree_iteration(TreeNode* root){
        std::stack<TreeNode*> st;
        if (root != NULL) st.push(root);
        while (!st.empty()) {
            TreeNode* node = st.top();
            if (node != NULL) {
                st.pop();
                if (node->right) st.push(node->right);  // 右
                if (node->left) st.push(node->left);    // 左
                st.push(node);                          // 中
                st.push(NULL);
            } else {
                st.pop();
                node = st.top();
                st.pop();
                std::swap(node->left, node->right);          // 节点处理逻辑
            }
        }
        return root;
    }
    // 2024.1.3 开始新一轮的刷题学习
    bool compare(TreeNode* left,TreeNode* right){
        //1.单层递归逻辑
        // 1）如果两个都为空 
        if (left==NULL && right==NULL) return true;
        // 2) 如果其中一个为空
        else if (left==NULL || right==NULL) return false;
        // 3) 如果两个都不为空 但值不相等
        else if (left->val != right->val) return false;
        // 4) 如果两个都不为空 而且值相等 则继续判断
        bool outside = compare(left->left,right->right) ;   // 左子树的左子树 和右子树的右子树 外侧
        bool inside = compare(left->right,right->left) ;  // 左子树的右子树 和右子树的左子树 内侧
        bool final = outside && inside;
        return final;       
    }
    bool isSymmetric_recursion(TreeNode* root) {
        // leetcode 101 Symmetric Tree 2024.1.3
        // 要比较的是根节点的两个子树是否是相互翻转的，进而判断这个树是不是对称树
        // 树的方案解决尽量用递归的知识去做；而后再用迭代解决一遍
        // TODO 另外一层层序遍历 将空节点用null填充 而后利用回文判断方法去判断的 可以后续再思量 
        /*
            对称数，需要判断的是根节点的左右子树是否是翻转的 
            注意的是比较的两个对象 就根节点而言，是左右子树，
            就左右子树而言，是更进一步的 外侧与内侧，即左子树的左边和右子树的右边，左子树的右边和右子树的左边；
            如果再往下一层则 同样的道理，上层为对应的节点，故而下一层仍然为对应的节点
            =====================================
            返回值 即true或false
            单层逻辑 即判断两个节点的相等情况 如果皆不为空而且相等，则继续向下判断 
            依据下一层判断结果综合得到该层的结果
        */
       if(root==NULL) return true;
       return compare(root->left,root->right);
    }
    bool isSymmetric_iteration(TreeNode* root){
        // 迭代法 运用的是 成对的思想 成对的入栈和出栈 并进行比较
        /*
            每次成对的入栈
            出栈也是成对的 而后判断完后 继续进行 则其会一路判断下去 
            对于栈而言，即刚刚push进去，马上就又要拿出来进行比较，即同等于递归的含义 
            而队列则是 会稍微延后比较 但本质上都不影响其成对的比较性
        */
       if(root==NULL) return true;
       std::stack<TreeNode*> st;
       st.push(root->left);
       st.push(root->right);
       while(!st.empty()){
            TreeNode* leftNode = st.top();st.pop();
            TreeNode* rightNode = st.top();st.pop();
            if(!leftNode&&!rightNode){
                // 两个节点都为空 则true，并继续比较st里的元素
                continue;
            }
            if(!leftNode||!rightNode||(leftNode->val!=rightNode->val)){
                return false;
            }
            // 最后一种情况 即两者元素都不为空 而且相等 故而继续往下去判断
            st.push(leftNode->left);
            st.push(rightNode->right);
            // 内侧
            st.push(leftNode->right);
            st.push(rightNode->left);
       }
       return true;
    }
    // 深度 和 高度  根节点的高度就是二叉树的最大深度
    // 递归法 节点数目等于左右节点数目加上本身的节点数目
    int countSingle(TreeNode* node){
        if(node==NULL) return 0;
        int left_count = countSingle(node->left);
        int right_count = countSingle(node->right);
        return left_count+right_count+1;
    }
    int countNodes_recursion(TreeNode* root) {
        // leetcode 222 Count Complete Tree Nodes 
        // 1.递归法
        // 2.迭代法
        // 3.完全二叉树的性质方法
        return countSingle(root);
    }
    int countNodes_iteration(TreeNode* root){
        // 迭代法 需要一个stack 每次取出本身加入而后 加入左右进栈
        std::stack<TreeNode*> st;
        int count=0;
        if(root==NULL) return 0;
        st.push(root);
        while(!st.empty()){
           TreeNode* tmp = st.top();
           st.pop();
           count++;
           if(tmp->left) st.push(tmp->left);
           if(tmp->right) st.push(tmp->right); 
        }
        return count;
    }
    /*
    完全二叉树只有两种情况，情况一：就是满二叉树，情况二：最后一层叶子节点没有满。
    对于情况一，可以直接用 2^树深度 - 1 来计算，注意这里根节点深度为1。
    对于情况二，分别递归左孩子，和右孩子，递归到某一深度一定会有左孩子或者右孩子为满二叉树，
    然后依然可以按照情况1来计算。
    ==>关键在于如何去判断一个左子树或者右子树是不是满二叉树呢？
    
    !!!! 如果递归向左遍历的深度等于递归向右遍历的深度，那说明就是满二叉树(!!只是遍历的的深度)

    */
    int countNodes(TreeNode* root) {
        /*
        如果树是完全二叉树，并且每一次递归都能够剔除半边的子树，那么每次调用 countNodes 函数时树的高度都会减少1，
        这个过程会发生大约 logn 次，因为二叉树的高度是对数级的。
        在每一次递归调用中，计算最左侧和最右侧路径的深度也是对数级的操作，因为每次都是沿着最左/最右路径向下，
        直到达到叶节点。
        */
        if (root == nullptr) return 0;
        TreeNode* left = root->left;
        TreeNode* right = root->right;
        int leftDepth = 0, rightDepth = 0; // 这里初始为0是有目的的，为了下面求指数方便
        while (left) {  // 求左子树深度==》 递归向左遍历的深度
            left = left->left;
            leftDepth++;
        }
        while (right) { // 求右子树深度==》递归向右遍历的深度
            right = right->right;
            rightDepth++;
        }
        if (leftDepth == rightDepth) {
            return (2 << leftDepth) - 1; // 注意(2<<1) 相当于2^2，所以leftDepth初始为0
        }
        return countNodes(root->left) + countNodes(root->right) + 1;
    }
    int height(TreeNode* node){
        if(node==NULL){return 0;}
        int left = height(node->left);
        if (left==-1) return -1;
        int right = height(node->right);
        if (right==-1) return -1;
        if(left-right>1 || right-left>1){
            // 如何break；break对应为-1相应的则继续-1
            return -1;
        }
        else{
            return 1+std::max(left,right);
        }
    }
    bool isBalanced(TreeNode* root) {
        // 分析递归结构 两科子树的高度差不超过1，则视为平衡 故而递归的去计算高度 当遇到两棵树高度差大于1时，直接返回false
        /*
            leetcode 01.07
            递归结构：三步骤 结束条件，单层逻辑，返回与进入类型
            错误的返回-1；如果返回的是-1，则继续返回-1；
        */
        return height(root)==-1?false:true;
    }
    void traversal_all_binary(TreeNode* cur,std::vector<int>& path,std::vector<std::string> &result){
        path.push_back(cur->val); // 中，中为什么写在这里，因为最后一个节点也要加入到path中 
        if(cur->left==NULL && cur->right==NULL){
            std::string sPath;
            for(int i=0;i<path.size()-1;i++){
                sPath += std::to_string(path[i]);
                sPath += "->";
            }
            sPath += std::to_string(path[path.size()-1]);
            result.push_back(sPath);
            return;
        }
        if(cur->left){
            // cur->left 加入后 记录完对应的path 而后再弹出
            traversal_all_binary(cur->left,path,result);
            path.pop_back(); // 回溯
        }
        if(cur->right){
            traversal_all_binary(cur->right,path,result);
            path.pop_back();
        }
        // path.pop_back()在外面也是可以的 前期加入节点 进行分析 后期弹出节点 
    }
    std::vector<std::string> binaryTreePaths_1(TreeNode* root) {
        // 将第一次涉及到回溯，因为我们要把路径记录下来，需要回溯来回退一个路径再进入另一个路径。
        /*
            leetcode 257 0107 
            递归和回溯是连在一起的 运用一个path去记录对应的节点进入和弹出的情况
            终止逻辑：左右节点都为空的时候 说明一条路径结束 此时拼接vector为string
        如果把 path + "->"作为函数参数就是可以的，因为并没有改变path的数值，
        执行完递归函数之后，path依然是之前的数值（相当于回溯了）
        */
       std::vector<int> path;
       std::vector<std::string> result;
       if(root==NULL){return result;}
       traversal_all_binary(root,path,result);
       return result;
    }
    /*
        简化整体的代码逻辑
        1.引用与内容拷贝的区别 结合迭代过程可以实现 加入函数是path是拷贝的 故而不会影响外围的path
        2.作为函数参数进行传递，不会影响本值 
    */
    void traversal_all_binary_smiple(TreeNode* cur,std::string path,std::vector<std::string> &result){
        path+=std::to_string(cur->val); // path非引用传递值进来 故而内部的更改不会反映到外部
        if(cur->right==NULL&&cur->left==NULL){
            result.push_back(path);
            return;
        }
        if(cur->left) traversal_all_binary_smiple(cur->left,path+"->",result);
        if(cur->right) traversal_all_binary_smiple(cur->right,path+"->",result);
    }  
    std::vector<std::string> binaryTreePaths_2(TreeNode* root){
        std::string path;
        std::vector<std::string> result;
        if(root==NULL) return result;
        traversal_all_binary_smiple(root,path,result);
        return result;
    }
    void LeftLeaves(TreeNode* cur,int &sum){
        if(cur->left==NULL&&cur->right==NULL) return;
        if(cur->left){
            if(cur->left->left==NULL&&cur->left->right==NULL){sum+=cur->left->val;} //leaf
            LeftLeaves(cur->left,sum);
        }
        if(cur->right) LeftLeaves(cur->right,sum);
    }
    int sumOfLeftLeaves(TreeNode* root) {
        /*
            leetcode 404 0107
            递归：遇到节点左右都无  传递一个引用参数进来 从上而下
            单层逻辑：累加当前节点的左节点的值  而后左右节点都得进
            left leaf 
        */
       //节点A的左孩子不为空，且左孩子的左右孩子都为空（说明是叶子节点），那么A节点的左孩子为左叶子节点
       int result = 0;
       if(root==nullptr) return result;
       LeftLeaves(root,result);
       return result;
    }

    void depth_find(TreeNode* cur,int depth,int &max_depth,int &value){
        if(cur->left==NULL&&cur->right==NULL){
            // 当遇到叶子节点的时候，就需要统计一下最大的深度了，所以需要遇到叶子节点来更新最大深度。
            if(depth>max_depth){
                max_depth = depth;
                value = cur->val;
            }
            return;
        }
        //  隐藏着回溯 =>C++的拷贝调用 参数传递 
        if(cur->left) depth_find(cur->left,depth+1,max_depth,value);
        if(cur->right) depth_find(cur->right,depth+1,max_depth,value);
    }
    int findBottomLeftValue(TreeNode* root) {
        // leetcode 513. Find Bottom Left Tree Value 0107
        /*
            1.先确定在最后一层 而后再取数据
            2.方法：层序遍历 queue 
            3.递归法：
                深度最大的叶子节点一定是最后一行，
                可以使用前序遍历（当然中序，后序都可以，因为本题没有 中间节点的处理逻辑，只要左优先就行），
                保证优先左边搜索，然后记录深度最大的叶子节点，此时就是树的最后一行最左边的值。
                1.终止条件：遇到叶子节点，并比较处理逻辑
                2.单层逻辑：计算深度
                3.输入输出： 输入节点 当前深度 输出 无

        */
       if(root==NULL){return 0;}
       int max_depth=1;
       int depth=1;
       int bottomleftvalue = root->val;
       depth_find(root,depth,max_depth,bottomleftvalue);
       return bottomleftvalue;
    }
    int path_sum(TreeNode* cur,int value_sum,int &targetSum){
        value_sum+=cur->val;
        if(cur->left==NULL&&cur->right==NULL){
            if(value_sum==targetSum) {return 1;}
            else{return 0;}
        }
        int left=0,right=0;
        if(cur->left)  left= path_sum(cur->left,value_sum,targetSum);
        if(cur->right) right = path_sum(cur->right,value_sum,targetSum);
        /*
            尽量给每个值都进行初始化 如果此处用的是 int left,right;
            ==================================================
            当你在一个函数内部声明局部变量，如 int left, right;，这些变量不会自动初始化为零。
            它们将被分配内存空间，但其中的值是未定义的，这意味着它们可能包含任何值。
            这些值是内存中该位置先前的残留数据，因此它们的初始值是不可预测的。
            ^^^^^自动初始化问题

            ==================================================
            而如果后续的if判断中 有没进去的left或right，其值则为NULL 
            会检查为初始化的变量
            ===>
            将int可以都转为bool型数据更加清晰！！
        */
        if(left||right) {return 1;}
        else {return 0;}
    }
    bool hasPathSum(TreeNode* root, int targetSum) {
        // 112. Path Sum 0107
        // 结合回溯的知识 函数开始时值加进去 最后再退出来 
        /*
            1.递归终止条件：叶子节点 不需要储存 只需要判断即可 
        如果需要搜索整棵二叉树且不用处理递归返回值，递归函数就不要返回值。（这种情况就是本文下半部分介绍的113.路径总和ii）
        如果需要搜索整棵二叉树且需要处理递归返回值，递归函数就需要返回值。 （这种情况我们在236. 二叉树的最近公共祖先 (opens new window)中介绍）
    如果要搜索其中一条符合条件的路径，那么递归一定需要返回值，因为遇到符合条件的路径了就要及时返回。（本题的情况）
        */
        if(root==NULL) return false;
        int value_sum = 0;
        return path_sum(root,value_sum,targetSum);   
    }
    void path_sum_2(TreeNode* cur,int targetSum,std::vector<int> path,std::vector<std::vector<int>> &result,int value_sum){
        //1.终止条件
        // 借用C++特性！！ 回溯在内部实现 内存复制 非地址调用 
        value_sum+=cur->val;
        path.push_back(cur->val); //此处直接存储int 不需要存储节点 
        if(cur->left==NULL&&cur->right==NULL){
            // std::vector<int> vec;
            if(value_sum==targetSum){
                result.push_back(path);
            }
        }
        //2.单层逻辑
        if(cur ->left) path_sum_2(cur->left,targetSum,path,result,value_sum);
        if(cur->right) path_sum_2(cur->right,targetSum,path,result,value_sum);        
    }
    std::vector<std::vector<int>> pathSum(TreeNode* root, int targetSum) {
        // leetcode 113. Path Sum II  0107 
        std::vector<std::vector<int>> result;
        if(root==NULL) return result;
        std::vector<int> path;
        int value_sum = 0;
        path_sum_2(root,targetSum,path,result,value_sum);
        return result;       
    }
    TreeNode* bulidTree_inorder_postorder(std::vector<int>&inorder,std::vector<int>postorder){
        // 此处非下标版本 用了多个vector去保存相应的不同切割后数组 则使得空间占用较大 更改为下标尝试一般
        if(postorder.size()==0) return NULL;
        int value = postorder[postorder.size()-1];
        TreeNode* root = new TreeNode(value);
        if(postorder.size()==1) return root;
        int delimiterIndex = 0 ;
        for(delimiterIndex;delimiterIndex<inorder.size()-1;delimiterIndex++){
            if(inorder[delimiterIndex]==value) break; // 目标是寻找和是的delimiterindex
        }
        // 运用迭代直接赋值 左闭右开 即[begin,delimiterindex) [delimiterindex+1,end)
        std::vector<int> inorder_left(inorder.begin(),inorder.begin()+delimiterIndex);
        std::vector<int> inorder_right(inorder.begin()+delimiterIndex+1,inorder.end());
        // 后序赋值 大小一样的
        postorder.pop_back();// 弹出最后一个元素即root
        // 左闭右开 [begin,begin+size) ,[begin+size,end)
        std::vector<int> postorder_left(postorder.begin(),postorder.begin()+inorder_left.size());
        std::vector<int> postorder_right(postorder.begin()+inorder_left.size(),postorder.end());
        root->left = bulidTree_inorder_postorder(inorder_left,postorder_left);
        root->right = bulidTree_inorder_postorder(inorder_right,postorder_right);
        return root;
    }
    TreeNode* buildTree_in_poster_Subscript(std::vector<int>& inorder, std::vector<int>& postorder,int inorder_begin,int inorder_end,int postorder_begin,int postorder_end){
        // 下标的话 需要输入的则应该是中序数组和后续数组对应的位置 
        if(postorder_end==postorder_begin) return NULL; // begin==end 的情况下 表示的是空 可能需要注意传入的区间的左闭右开性 
        int value = postorder[postorder_end-1];
        TreeNode* root = new TreeNode(value);
        if(postorder_end-postorder_begin==1) return root;
        int delimiterIndex = 0;
        for(delimiterIndex=inorder_begin;delimiterIndex<inorder_end;delimiterIndex++){
            if(inorder[delimiterIndex]==value) break;
        }
        // 切割中序数组
        // 左中序区间，左闭右开[leftInorderBegin, leftInorderEnd)
        int leftInorderBegin = inorder_begin;
        int leftInorderEnd = delimiterIndex;
        // 右中序区间，左闭右开[rightInorderBegin,rightInorderEnd)
        int righInorderBegin = delimiterIndex+1;
        int rightInorderEnd = inorder_end;
        // 切割后续数组
        // 左后序区间，左闭又开[)
        int leftPostorderBegin = postorder_begin;
        int leftPostorderEnd = postorder_begin+delimiterIndex-inorder_begin; // 终止位置需要添加长度相关序列
        int rightPostorderBegin = postorder_begin+delimiterIndex-inorder_begin;
        int righrPostorderEnd = postorder_end-1; // 舍去最后的某位元素
        root->left = buildTree_in_poster_Subscript(inorder,postorder,leftInorderBegin,leftInorderEnd,leftPostorderBegin,leftPostorderEnd);
        root->right = buildTree_in_poster_Subscript(inorder,postorder,righInorderBegin,rightInorderEnd,rightPostorderBegin,righrPostorderEnd);
        return root;
    }
    TreeNode* buildTree(std::vector<int>& inorder, std::vector<int>& postorder) {
        // leetcode 106. Construct Binary Tree from Inorder and Postorder Traversal 0108
        /*
            后序数组的最后一个元素为切割点，先切中序数组，根据中序数组，反过来再切后序数组。
            一层一层切下去，每次后序数组最后一个元素就是节点元素。
            递归思路
            1.判断空的情况 即在递归的过程中 会遇到某个节点 其只有一边的情况 此时分割完后 有一边的序列为空
            2.选取对应的后续数组的最后一位 作为root
              判断中序数组的大小 如果为1 说明其为叶子节点 后续不用切割了 直接返回return
            3.依据root找到在中序数组中对应的位置 
            4.依据位置切分中序数组为中左和中右两个数组
            5.依据中序数组和后序数组大小相同的原因 切分后左和后右两个数组
            6.继续依据对应的数组构建root->left和root->right
            7 返回root
        */
       // inorder
        if (inorder.size() == 0 || postorder.size() == 0) return NULL;
        // vector版
        return bulidTree_inorder_postorder(inorder,postorder);
        // 下标版
        return buildTree_in_poster_Subscript(inorder,postorder,0,inorder.size(),0,postorder.size());
    }
    TreeNode* MaximunBinary(std::vector<int>&nums,int begin_index,int end_index){
        if(begin_index==end_index) return NULL;
        int max_index = begin_index; //0 <= nums[i] <= 1000
        int max_value = nums[begin_index];
        for(int i=begin_index;i<end_index;i++){
            if(nums[i]>nums[max_index]){
                max_index = i;
                max_value = nums[max_index];
            }
        }
        TreeNode* root = new TreeNode(max_value);
        if(end_index-begin_index==1) return root;
        // 保持左闭右开
        int left_begin = begin_index;
        int left_end = max_index;
        // 保持左闭又开  当right部分无值的时候 则max_index+1与end_index的值是相同的 
        int right_begin = max_index+1;
        int right_end = end_index;
        root->left = MaximunBinary(nums,left_begin,left_end);
        root->right = MaximunBinary(nums,right_begin,right_end);
        return root;
    }
    TreeNode* constructMaximumBinaryTree(std::vector<int>& nums) {
        // leetcode 654. Maximum Binary Tree 0108 
        /*
            1.每次找到最大的 最大的作为root值
            2.而后依据该root值 拆分左右的序列 继续找下一个最大的值最为最大的
            是允许空节点进入递归，所以不用在递归的时候加判断节点是否为空
            ==================================
            意类似用数组构造二叉树的题目，每次分隔尽量不要定义新的数组，而是通过下标索引直接在原数组上操作，这样可以节约时间和空间上的开销。
            如果让空节点（空指针）进入递归，就不加if，如果不让空节点进入递归，就加if限制一下， 终止条件也会相应的调整。
        */
        return MaximunBinary(nums,0,nums.size());
    }
    TreeNode* mergeTree_recursion(TreeNode* root1,TreeNode* root2){
        // 前序遍历 先根节点 后左右节点 
        int value=0;
        TreeNode * cur_root = new TreeNode(value);
        if(!root1&&!root2) return NULL; // 这句话似乎可以省略 因为相应的如果两者都是NULL的话 相应的返回另外一个null也是同样的含义
        // 判断对应的左右树为空的具体情况  即可以直接将该节点以及之后的拿过来用 
        if(root1==NULL && root2!=NULL) return root2;
        if(root1!=NULL && root2==NULL) return root1;
        // 单层逻辑
        int left_value = root1->val;
        int righ_value = root2->val;
        value = left_value+righ_value;
        cur_root->val = value;
        cur_root->left = mergeTree_recursion(root1->left,root2->left);
        cur_root->right = mergeTree_recursion(root1->right,root2->right);
        return cur_root;
    }
    TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
        // leetcode 617. Merge Two Binary Trees 0109
        // 传入两个树的根节点 同时遍历 保持同步 前序遍历 
        /*
            递归思路：结束遍历时刻？=》两个树都为空的时候 
            单层逻辑：先前序 加和两个节点 注意判断如果其中有一个为空的时候的情况 nullptr if语句 
                    分别得到两个节点的值后 加和 继而往下遍历分析 left right 允许空树木 
            迭代法：
        */
        // return mergeTree_recursion(root1,root2); 递归 
        // 迭代 
        // 迭代法中，一般一起操作两个树都是使用队列模拟类似层序遍历，同时处理两个树的节点，这种方式最好理解，
        // 如果用模拟递归的思路的话，要复杂一些。
        if (t1 == NULL) return t2;
        if (t2 == NULL) return t1;
        std::queue<TreeNode*> que;
        que.push(t1);
        que.push(t2);
        while(!que.empty()) {
            TreeNode* node1 = que.front(); que.pop();
            TreeNode* node2 = que.front(); que.pop();
            // 此时两个节点一定不为空，val相加
            node1->val += node2->val;

            // 如果两棵树左节点都不为空，加入队列
            if (node1->left != NULL && node2->left != NULL) {
                que.push(node1->left);
                que.push(node2->left);
            }
            // 如果两棵树右节点都不为空，加入队列
            if (node1->right != NULL && node2->right != NULL) {
                que.push(node1->right);
                que.push(node2->right);
            }

            // 当t1的左节点 为空 t2左节点不为空，就赋值过去
            if (node1->left == NULL && node2->left != NULL) {
                node1->left = node2->left;
            }
            // 当t1的右节点 为空 t2右节点不为空，就赋值过去
            if (node1->right == NULL && node2->right != NULL) {
                node1->right = node2->right;
            }
        }
        return t1;   
    }
    TreeNode* searchBST(TreeNode* root, int val) {
       // leetcode 700. Search in a Binary Search Tree 0109
       if(root==NULL || root->val == val) return  root; // 合并两类代码 ！！
       TreeNode* cur = NULL;
       // 习惯直接写 searchBST(root->left, val)，却忘了 递归函数还有返回值。
       //递归函数的返回值是什么? 是 左子树如果搜索到了val，要将该节点返回。 
       // 如果不用一个变量将其接住，那么返回值不就没了
       if (root->val>val) cur = searchBST(root->left,val);
       if (root->val<val) cur =  searchBST(root->right,val);
       return cur;
    }
    // 使用栈来模拟深度遍历，使用队列来模拟广度遍历
    // 因为二叉搜索树的有序性，遍历的时候要比普通二叉树简单很多。
    TreeNode* searchBST(TreeNode* root, int val) {
        while (root != NULL) {
            if (root->val > val) root = root->left;
            else if (root->val < val) root = root->right;
            else return root;
        }
        return NULL;
    }
    TreeNode* pre = NULL; // 用来记录前一个节点
    bool isValidBST(TreeNode* root) {
        // leetcode 98. Validate Binary Search Tree
        
        // 注意考虑相等的情况 
        /*
         [5,4,6,null,null,3,7] 
         注意考虑不是单独右节点小于 而是整个右子树小于？？
        =》》中序遍历下，输出的二叉搜索树节点的数值是有序序列。
        验证二叉搜索树，就相当于变成了判断一个序列是不是递增的了。
        陷阱1：
        不能单纯的比较左节点小于中间节点，右节点大于中间节点就完事了。
        要比较的是 左子树所有节点小于中间节点，右子树所有节点大于中间节点
        陷阱2：
        样例中最小节点 可能是int的最小值，如果这样使用最小的int来比较也是不行的。
        此时可以初始化比较元素为longlong的最小值。
        问题可以进一步演进：如果样例中根节点的val 可能是longlong的最小值 又要怎么办呢？文中会解答
        */
        if (root == NULL) return true;
            bool left = isValidBST(root->left);

            if (pre != NULL && pre->val >= root->val) return false;
            pre = root; // 记录前一个节点

            bool right = isValidBST(root->right);
            return left && right;
        }
    int getMinimumDifference_1(TreeNode* root) {
        // leetcode 530. Minimum Absolute Difference in BST 0123
        /*
        二叉搜索树的特征：节点的左子树比节点的值小 节点的右子树比节点的值大 节点的左右子树仍然是二叉搜索树
        给你一棵所有节点为非负值的二叉搜索树，请你计算树中任意两节点的差的绝对值的最小值
        中序遍历得到的数组是递增的 那么取出中序遍历的数组值后 求差值数组 而后找出最小值 
        */
        // 中序遍历 ？ 递归写法 或则是迭代写法 迭代写法 需要对应的栈的帮助 入栈后何时出栈以及出栈的顺序为何
        std::stack<TreeNode*> stIn;
        std::vector<int> vec;
        // 中序遍历
        // 迭代法实现
        TreeNode* cur = root;
        while(cur!=nullptr || !stIn.empty()){
            // 分析 
            if(cur!=nullptr){
                // alaways turn to left until get the finally point of left
                stIn.push(cur);
                cur = cur->left;
            }
            else{
                /*
                错误的点主要在于 循环的过程中的 在弹出的时候重新写一个cur与用原始的cur的问题 
                相当于同名的两个但对应的作用域会覆盖 但仅限于内部 外部仍然未改变？？
                TreeNode* cur = stIn.top();
                当您在else块中对这个内部的cur变量赋值时，这个赋值不会影响外部循环中的cur变量。这可能会导致您的循环逻辑不按预期工作，
                因为外部循环的cur在每次迭代中都不会更新为栈顶节点的右子节点。换句话说，程序可能会反复处理同一个节点，而不是按预期遍历整个树。
                */
                cur = stIn.top();
                stIn.pop();
                vec.push_back(cur->val);
                cur = cur->right;
            }
        }
        // 对vec继续相应的差值序列 并在计算时继续比较
        // 简化步骤 1：递增序列 则必然是正的 循环过程中可以只用i和i-1
        if (vec.size() < 2) {
        // 处理边界情况
        return 0; // 或者其他合适的值
        }
        int min_difference = INT_MAX;
        for(int i=1;i<vec.size();i++){
            min_difference = std::min(min_difference,vec[i]-vec[i-1]);
        }
        return min_difference;
    }
    // 实际运行过程中 不仅记录next还可以记录pre 这样方便在中序遍历的过程中完成对应的min计算 递归写法与迭代写法！！
    int getMinimumDifference_2(TreeNode* root){
        // 迭代写法 中序遍历 选用一个pre节点记录前面的值 而后遍历过程中比较选取值  0125
        std::stack<TreeNode*> stIn;
        TreeNode* cur = root;
        TreeNode* pre = NULL;
        int result=INT32_MAX;
        if(cur!=NULL){stIn.push(cur);}
        // 分析循环的结束条件 cur仍然有值则可以继续遍历 或是stIn为非空 即非遍历完毕
        while(cur!=NULL || !stIn.empty()){
            if(cur!=NULL){
                // 一直进栈 直到左边的为空
                stIn.push(cur);
                cur=cur->left;
            }
            else{
                cur = stIn.top();
                stIn.pop();
                if(pre!=NULL){
                    result = std::min(result,std::abs(cur->val-pre->val));
                }
                pre = cur;
                cur = cur->right;
            }
        }
        return result;
    }
    // 无返回值 依据引用传递方式获取返回值
    void search_BST(TreeNode* cur,std::unordered_map<int,int> &map){
        if(cur==NULL) return;
        else{
            map[cur->val]++; // 如果map中无该值 赋予0 而后++；
            search_BST(cur->left,map);
            search_BST(cur->right,map);
        }
    }
    bool static cmp(const std::pair<int,int> &a,const std::pair<int,int> &b){
        return a.second>b.second; // 从高到低进行排序
    }
    std::vector<int> findMode_1(TreeNode* root) {
        // leetcode 501. Find Mode in Binary Search Tree => 众数【不止一个？】以及对应的二叉搜索树【有序的，故而重复的在一起】 0125
        // 节点的左右节点都可以等于本身节点的值 
        // 非二叉搜索树版本 遍历过程中运用map获取 而后将map转换为vector 进行频率统计 而后取出最前面的值
        std::unordered_map<int,int> map;
        search_BST(root,map);
        std::vector<std::pair<int,int>> vec(map.begin(),map.end());
        sort(vec.begin(),vec.end(),cmp);
        // 选取前几个
        std::vector<int> result;
        result.push_back(vec[0].first);
        for(int i=1;i<vec.size();i++){
            if(vec[i].second==vec[0].second) result.push_back(vec[i].first);
        }
        return result;        
    }
    // 被对应的不需要额外空间困住了 结果集并不是额外的空间
    /*
    因为中序遍历下的二叉搜索树是有序的 故而此处相应的只需要比较相应的节点；
    运用一个count值记录相应的节点值；而后加入结果集 如果出现更大的 则清空结果集 重新添加
    */
    void inorder_Mode(TreeNode* cur,TreeNode*& pre,std::vector<int> &vec,int &count,int &maxCount){
        // 注意此处的pre count maxCount 都需要基于引用 因为其需要全局可见 更改 0125
        if(cur==NULL) return;
        inorder_Mode(cur->left,pre,vec,count,maxCount);
        // 中间节点的处理逻辑
        if(pre == NULL){
            count = 1;
        }
        else if(pre->val==cur->val){
            count ++;
        }
        else if(pre->val!=cur->val){
            count = 1;
        }

        if(count==maxCount){
            vec.push_back(cur->val);
        }
        else if(count>maxCount){
            maxCount = count;
            vec.clear();
            vec.push_back(cur->val);
        }
        pre = cur;
        // pre取值过程中如何操作 是一个问题！！
        inorder_Mode(cur->right,pre,vec,count,maxCount);
        // return;
    }
    std::vector<int> findMode(TreeNode* root){
        // 分析
        std::vector<int> result;
        int count =0;
        int maxCount = 0;
        TreeNode *pre = NULL;
        inorder_Mode(root,pre,result,count,maxCount);
        return result;       
    }
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // leetcode 236. Lowest Common Ancestor of a Binary Tree 0126

    }

};
