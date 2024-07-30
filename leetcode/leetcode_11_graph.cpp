#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <limits>
#include <list>

using namespace std;

class Solution
{
public:
    // leetcode 797. All Paths From Source to Target
    vector<vector<int>> result; // 收集符合条件的路径
    vector<int> path;           // 0节点到终点的路径
    // x：目前遍历的节点
    // graph：存当前的图
    void dfs_allpath(vector<vector<int>> graph, int node)
    {
        // end condition
        // 要求从节点 0 到节点 n-1 的路径并输出，所以是 graph.size() - 1
        if (node == graph.size() - 1)
        {
            // 该节点已经放进去了
            result.push_back(path); // 找到符合条件的一条路径
            return;
        }
        // dfs
        for (int i = 0; i < graph[node].size(); i++)
        {                                       // 遍历节点n链接的所有节点
            path.push_back(graph[node][i]);     // 遍历到的节点加入到路径中来
            dfs_allpath(graph, graph[node][i]); // 进入下一层递归
            path.pop_back();                    // 回溯，撤销本节点
        }
    }
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>> &graph)
    {
        path.push_back(0);     // 无论什么路径已经是从0节点出发
        dfs_allpath(graph, 0); // 开始遍历
        return result;
        // 图节点的输入形式 有向无环图 邻接表格式
    }
    // bfs 需要注意节点的标记信息  加入队列中的节点需要被立刻标记，这样不会重复访问
    int dir[4][2] = {0, 1, 0, -1, 1, 0, -1, 0}; // 四方图的方向信息
    void bfs(vector<vector<char>> &grid, vector<vector<bool>> visited, int x, int y)
    {
        queue<pair<int, int>> que;
        que.push({x, y});  // pair的形式 {int,int} 直接进行元素的插入
        visited[x][y] = 1; // 起始节点 加入后立刻标记
        while (!que.empty())
        { // 开始遍历队列里的元素
            pair<int, int> cur = que.front();
            que.pop(); // 从最前面取出元素；而后后续需要遍历的元素加进去
            int curx = cur.first;
            int cury = cur.second;
            for (int i = 0; i < 4; i++)
            { // 向四个方向开始遍历
                int nextx = curx + dir[i][0];
                int nexty = cury + dir[i][1]; // 较好的方式去获取周围四个方向的节点情况 获取后加入遍历
                if (nextx < 0 || nextx >= grid.size() || nexty < 0 || nexty >= grid[0].size())
                {
                    continue; // 下标越界 获取不到对应位置的内容
                }
                if (!visited[nextx][nexty])
                {                              // 如果节点没被访问过
                    que.push({nextx, nexty});  // 添加节点到队列的末尾
                    visited[nextx][nexty] = 1; // 加入队列后立刻标记 避免重复访问
                }
            }
        }
    }
    // leetcode 200 岛屿数量：
    void dfs_Islands(vector<vector<char>> &grid, vector<vector<bool>> &visited, int x, int y)
    {
        for (int i = 0; i < 4; i++)
        {
            int nextx = x + dir[i][0];
            int nexty = y + dir[i][1];
            if (nextx < 0 || nextx >= grid.size() || nexty < 0 || nexty >= grid[0].size())
            {
                continue;
            }
            if (!visited[nextx][nexty] && grid[nextx][nexty] == '1')
            {
                // 没有访问过的，同时是陆地的
                // 需要基于该节点可以访问的所有陆地节点全都访问一遍
                visited[nextx][nexty] = true;
                dfs_Islands(grid, visited, nextx, nexty);
            }
        }
    }
    // 广度搜索 同样的道理也是为了获得以该节点出发的所有相邻的节点
    void bfs_ISlands(vector<vector<char>> &grid, vector<vector<bool>> &visited, int x, int y)
    {
        queue<pair<int, int>> que;
        que.push({x, y});
        visited[x][y] = true; // 只要加入队列，立刻标记
        while (!que.empty())
        {
            pair<int, int> cur = que.front();
            que.pop();
            int curx = cur.first;
            int cury = cur.second;
            for (int i = 0; i < 4; i++)
            {
                int nextx = curx + dir[i][0];
                int nexty = cury + dir[i][1];
                if (nextx < 0 || nextx >= grid.size() || nexty < 0 || nexty >= grid[0].size())
                    continue; // 越界了，直接跳过
                if (!visited[nextx][nexty] && grid[nextx][nexty] == '1')
                {
                    que.push({nextx, nexty});
                    visited[nextx][nexty] = true; // 只要加入队列立刻标记
                }
            }
        }
    }
    // 是用遇到一个没有遍历过的节点陆地，计数器就加一，然后把该节点陆地所能遍历到的陆地都标记上。
    int numIslands_dfs(vector<vector<char>> &grid)
    {
        int n = grid.size(), m = grid[0].size();
        vector<vector<bool>> visited = vector<vector<bool>>(n, vector<bool>(m, false));
        int result = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (!visited[i][j] && grid[i][j] == '1')
                {
                    // 遍历grid，针对于每个是陆地节点，基于该陆地节点首先进行dfs搜索，合并成最初的岛屿
                    visited[i][j] = true;
                    result++;                         // 遇到没访问过的陆地 +1
                    dfs_Islands(grid, visited, i, j); // 将其与链接的陆地都标记上 true
                }
            }
        }
        return result;
    }
    // 拓扑排序
    // 给出一个有向图，把这个有向图转成线性的排序，称之为拓扑排序；
    // 拓扑排序也需要检测这个有向图是否有环，即是否存在循环依赖的关系；拓扑排序是图论中判断有向无环图的常用方法
    /*
    1.找到入度为0的节点，加入结果集
    2.将该节点从图中移除【移除后需要将其对应连接的节点的入度都减1】【故而需要有对应的map，存储以该节点为起点的vector】
    循环以上的两部，直到所有节点都在图中被移除了，结果集的顺序，即是我们想要的拓扑排序顺序。
    结果集的顺序其实不确定的，只要同一层的在一起即可
    我们发现结果集元素个数 不等于 图中节点个数，我们就可以认定图中一定有 有向环！
    */
    void Topologicalsorting()
    {
        int n, m, s, t; // n 物体 m个关系
        cin >> n >> m;
        // S，T 依赖关系
        unordered_map<int, vector<int>> umap;
        // 记录文件的依赖关系,后续需要基于该关系将对应的入度进行减少
        // 类似于图里的该节点的next邻居
        vector<int> inDegree(n); // 记录每个文件的入度
        vector<int> result;      // 记录结果
        while (m--)
        { // m 行
            // s->t 先有s才能有t
            cin >> s >> t;
            inDegree[t]++;
            umap[s].push_back(t);
        }
        queue<int> queue;
        for (int i = 0; i < n; i++)
        {
            // 将入度为0的节点，作为开头，加入队列中 如果有多个 则加入多次
            if (inDegree[i] == 0)
                queue.push(i);
        }
        while (queue.size())
        {
            int cur = queue.front();
            queue.pop();
            result.push_back(cur);
            // 移除以该节点为起点的入度节点
            vector<int> files = umap[cur];
            for (int i = 0; i < files.size(); i++)
            { // BFS的思想
                int next = files[i];
                inDegree[next]--; // curr 指向的文件入度-1；
                // 如果入度为0，加入到队列中
                if (inDegree[next] == 0)
                    queue.push(next);
            }
        }
        if (result.size() == n)
        {
            for (int i = 0; i < n - 1; i++)
            {
                cout << result[i] << " ";
            }
        }
        else
        {
            cout << -1 << endl;
        }
    }
    // Dijkstra algorithm
    // 单源最短路径 给出一个有向图，找到从起点到每一个节点的最短路径
    // 不适合具有负权值的:  因为访问过的节点 不能再访问，导致错过真正的最短路
    /*
    open-set/close-set/ 每次更新对应节点的cost
    */
    void Dijkstra()
    {
        // 1、选源点到哪个节点近【全局的】且该节点未被访问过
        // 2、该最近节点被标记访问过
        // 3、更新非访问节点到源点的距离（即更新minDist数组）
        int n, m, p1, p2, val;
        cin >> n >> m; // n个公交汽车站 m条公路
        // M行，每行表示为三个整数，S,E,V；代表了S车站可以单向直达E车站，并且需要花费V单位的时间
        vector<vector<int>> grid(n + 1, vector<int>(n + 1, INT_MAX));
        for (int i = 0; i < m; i++)
        {
            cin >> p1 >> p2 >> val;
            grid[p1][p2] = val; // 单向路径
        }
        int start = 1;
        int end = n;
        // n+1 是多引入了一个0节点，辅助初始节点，比如从0节点到1节点
        vector<int> minDist(n + 1, INT_MAX);
        vector<int> visited(n + 1, false);
        vector<int> parent(n + 1, -1);
        minDist[start] = 0;
        // n 步 每步都需要在n个节点中选最小的，而选完最小的之后又需要依次更新n个节点的next==》 O（n*n）
        for (int i = 1; i <= n; i++) // n
        {
            // 最多n步即迭代n次，即可找到
            int minVal = INT_MAX;
            int cur = 1;
            // 可以基于优先级队列进行修改 如果
            for (int v = 1; v <= n; ++v)
            {
                // 选距离原点最近而且未被访问过的节点
                if (!visited[v] && minDist[v] < minVal)
                {
                    minVal = minDist[v];
                    cur = v; // 挑选出离当前节点最近的节点 并设置为cur
                }
            }
            // 此处visitied中的true等同于前期的closed-set 表示的是周围的节点全部访问过了
            // visitied中的false包含了加入openset和未加入openset两种情况 一种是之前只是简单访问过，另一种是完全没有访问过
            visited[cur] = true; // 标记cur节点为已被访问 加入close-set 这指的是相应的cur的周围节点都被访问过了
            // 更新该节点cur的所有next节点
            for (int v = 1; v <= n; v++) // n
            {
                if (!visited[v] && grid[cur][v] != INT_MAX && minDist[cur] + grid[cur][v] < minDist[v])
                {
                    minDist[v] = minDist[cur] + grid[cur][v]; // 若cur到v有更短的路径，更新minDist[v]
                    parent[v] = cur;                          // 记录cur是v的前驱 父节点
                }
            }
        }
        if (minDist[end] == INT_MAX)
            cout << -1 << endl; // 不能到达终点
        else
            cout << minDist[end] << endl; // 到达终点最短路径
        // 打印最短路径
        // 输出最短情况
        for (int i = 1; i <= n; i++)
        {
            cout << parent[i] << "->" << i << endl;
        }
    }
    // Dijkstra 堆优化版本 O(mlogm)
    void Dijkstra_Heap_optimization()
    {
        /*
        稠密图，n很大，相应的边也很大；
        稀疏图，n很大，但边的数量很小；==》 从边出发 可以进行优化设计
        图的存储：
            邻接矩阵：二维数组，节点的角度
            邻接表：  数组+链表的形式 边的角度；有多少边 邻接表才会申请多少个对应的链表节点。
            优点：对于稀疏图的存储，只需要存储边，空间利用率高；遍历节点链接情况相对容易
            缺点：检查任意两个节点间是否存在边，效率相对低，需要 O(V)时间，V表示某节点链接其他节点的数量。
            vector<list<pair<int,int>>> grid(n + 1);
            权值的存储在对应的pair里面，
        ==》整体的逻辑是一样的，区别只在于：
        邻接表的表示方式不同，构造图的时候可能有些区别
        使用优先级队列（小顶堆）来对新链接的边排序
        不需要两层遍历：==》 理清对应的时间复杂度的计算方法。
        */
        struct Compare
        {
            bool operator()(pair<int, int> p1, pair<int, int> p2)
            {
                return p1.second > p2.second;
            }
        };
        struct Edge
        {
            /* data */
            int to;                               // 邻接顶点
            int val;                              // 边的权重
            Edge(int t, int w) : to(t), val(w) {} // 构造函数
        };
        int n, m, p1, p2, val;
        cin >> n >> m;
        // vector - list(标准库的链表) - Edge(类)
        vector<list<Edge>> grid(n + 1);
        while (m--)
        {
            cin >> p1 >> p2 >> val;
            grid[p1].push_back(Edge(p2, val)); // 类的初始化 构造函数
        }
        int start = 1;
        int end = n;
        vector<int> minDist(n, numeric_limits<int>::max());
        vector<int> visited(n, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, Compare> pq;
        // 类似于回溯以及递归等 需要先加入初始值
        pq.push(make_pair(start, 0));
        minDist[start] = 0;
        while (!pq.empty())
        {
            pair<int, int> current_element = pq.top();
            pq.pop();
            if (visited[current_element.first])
                continue;
            visited[current_element.first] = true;
            for (Edge edge : grid[current_element.first])
            {
                // 邻居节点同样需要判断是否已经被完整的1访问过了
                if (!visited[edge.to] && minDist[current_element.first] + edge.val < minDist[edge.to])
                {
                    minDist[edge.to] = minDist[current_element.first] + edge.val;
                    pq.push(make_pair(edge.to, minDist[edge.to]));
                }
            }
        }
        if (minDist[end] == numeric_limits<int>::max())
            cout << "No " << endl;
        else
            cout << minDist[end] << endl;
    }
    // 最小优先队列版本，closed-set和open-list的结构
    void Dijkstra_priority_queue()
    {
        /*
        O（E*（N+logE））while (!pq.empty()) 时间复杂度为 E ，while 里面 每次取元素 时间复杂度 为 logE，和 一个for循环 时间复杂度 为 N 。
        1. 该写法与后续的A*的整体算法结构是类似的，主要区别在于取值，一个取得是g[cost]z最小，一个取得是f[cost]
        2. 整体的逻辑顺序：
        1） 输入应该包含起点 终点 有向加权图（表征形式为vector<vector）,元素值即为value值
        2） 数据结构应该包含
            最小优先队列priority_queue - openlist 存储接下来需要访问的节点
            无序集合 unordered_set - closed_set   存储已经被访问完的节点
            vector  - g[cost] 存储每个节点的cost 【是否需要】 如果节点的表现形式为pair 同时存储了cost和id的话
            parent 存储每个节点的父路径，用于回溯
        3） 算法流程：
            1.将start放入open-list
            2.while循环：[openlist不为空 ]
                open-list 取top 即最小的 并pop
                判断是否在closed-set中
                判断是不是goal，如果是 则找到了 开始进行回溯
                不是则继续
                for 所有的节点
                更新基于当前curr的next节点的cost的值 父节点等
        */
        int n, m, p1, p2, val, start, goal;
        cout << "input n nodes and m edges";
        cin >> n >> m;
        cout << "input start node and goal node";
        cin >> start >> goal;
        vector<vector<int>> grid(n, vector<int>(n));
        while (m--)
        {
            cin >> p1 >> p2 >> val;
            grid[p1][p2] = val;
        }
        // priority_queue set
        struct Compare
        {
            // true 表示第一个参数应该排在第二个参数的后面
            bool operator()(pair<int, int> const &p1, pair<int, int> const &p2)
            {
                // 如果 p1 的成本大于 p2 的成本，返回 true，这样 p1 就会排在 p2 后面，故而实现的则是最小堆
                return p1.first > p2.first; // 最小堆，费用较小的元素优先
            }
        };

        // 优先队列[基本类型，底层实现类型，自定义比较器]
        priority_queue<pair<int, int>, vector<pair<int, int>>, Compare> open_list;
        // pair<cost,idx> make_pair是一个标准库函数，用于创建一个std::pair对象。std::pair是一个模板类，允许你将两个不同类型的值组合成一个单一的对象
        open_list.push(make_pair(0, start));
        // 存储到达每个节点的最短路径费用
        vector<int> g(n, numeric_limits<int>::max());
        g[start] = 0;
        // 存储访问过的节点
        unordered_set<int> closed_set; // 无序的unordered_set 是以哈希表为基础的，查询和增删的效率都是O(1),比set的O(logn)快
        vector<int> parent(n, -1);     // 存储父节点的信息，以便重构路径
        parent[start] = start;
        pair<vector<int>, int> result;
        // 如果目标节点不可达，则返回空路径和无穷大费用
        // numeric_limits<int>::max()  获取 int 类型的最大值 类型安全的，具有可移植性，可读性强
        result = make_pair(vector<int>(), numeric_limits<int>::max());
        while (!open_list.empty())
        {
            pair<int, int> current_element = open_list.top();
            open_list.pop();
            // 先top取出最前面即最小的元素，然后再pop进行删除。
            int current_cost = current_element.first;
            int current = current_element.second;
            // 该判断需要提前
            if (closed_set.find(current) != closed_set.end())
            {
                // 找不到返回尾后迭代器 => 找到了则说明这个节点已经被完全的访问过了
                continue;
            }
            closed_set.insert(current); // 如果没有找到，则相应的插入current，即该节点要被完全的访问了
            if (current == goal)
            {
                vector<int> path;
                // 开始回溯 借助于前期的parent案例
                while (current != start)
                {
                    path.push_back(current);
                    current = parent[current]; // parent 存储的是每一个节点的父亲节点 循环取即可
                }
                path.push_back(start);
                reverse(path.begin(), path.end());
                result = make_pair(path, g[goal]);
            }
            // 处理当前节点的所有未访问的邻居 ==> 此处是可以优化的，其实并不需要遍历所有的节点，基于高效的数据结构邻接表可以更快N
            for (int next = 0; next < n; ++next)
            {
                // !=0 的引入剪枝了很多
                if (grid[current][next] != 0 && closed_set.find(next) == closed_set.end())
                {
                    int next_cost = current_cost + grid[current][next];
                    // 每次访问都需要更新所有节点的最新最短值，并且将访问的节点加入对应的open-list；并更换当前的父节点。
                    // 如果是第一次 则父节点也是前期初始化的 -1 也需要被替代
                    if (next_cost < g[next])
                    {
                        g[next] = next_cost;
                        open_list.push(make_pair(next_cost, next));
                        parent[next] = current;
                    }
                }
            }
        }
        vector<int> final_path = result.first;
        int final_cost = result.second;
        if (!final_path.empty())
        {
            cout << "Shortest path from" << start << "to" << "goal" << ";";
            for (const int &node : path)
            {
                cout << node << " ";
            }
            cout << "with cost " << final_cost << endl;
        }
        else
        {
            cout << "No path found from " << start << "to " << goal << endl;
        }
    }
    // 具有负权的图最短路径算法
    void BellmanFord()
    {
        // 负权回路是指一系列道路的总权值为负，这样的回路使得通过反复经过回路中的道路，理论上可以无限地减少总成本或无限地增加总收益。
        /*
        正权重环/0环:只需考虑不超过n-1条边的，无重复顶点的简单路径
        负权重环： 最短路径是不良定义的；
        有向无环图：基于拓扑排序和动态规划即可解答出 d(v)={0,如果v=s；min{d(u)+w(u,v)},否则；}
        一般有向图：需要检测负权重环，并在无负权重环时输出最短路径
        ==》当存在负环的时候，顶点之间会互相依赖，拓扑排序不再可靠
        此时需要定义更细一步的子问题: d(v) => d(v,k) 计算从s到v的，最多包含k条边的最短路径
        d(v,k) = min{d(v,k-1),min{d(u,k-1)+w(u,v)}} u是v的前驱节点，至多k条边，则可能还是k-1条，k-2条；
        k轮循环，每次循环都对所有边进行一次松弛；
        for k=1 to |V| do: [相当于最外围的min]
            for each edge(u,v) 属于 E do: [相当于内部的min，考虑每一条顶点是v的节点，其实相当于更新每个节点]
                if d(v) > d(u) + w(u,v) then
                    d(v) = d(u) + w(u,v);
        基于迭代的方式计算d(v,k)==>BellmanFord 算法
        基于边！！ 不是基于节点的顺序遍历
        【【对所有边松弛一次，相当于计算 起点到达 与起点一条边相连的节点 的最短距离】】
        对所有边松弛 n-1 次 就一定能得到 起点到达 终点的最短距离。
        时间复杂度：O(N*E)
        ==> 松弛其实是为了应对可能的环，因为某一个节点可能在存在环的情况下会被多次访问，
        而这可以运用边来衡量，即经过多少条边（类似于松弛多少次）到这的最短距离
        */
        int n, m, p1, p2, val;
        cin >> n >> m;
        vector<vector<int>> grid;
        while (m--)
        {
            cin >> p1 >> p2 >> val;
            grid.push_back({p1, p2, val}); // 存储的是边，边的表达形式 Edge 不是原本的密集的二维图
        }
        int start = 1; // 起点
        int end = n;   // 终点
        vector<int> minDist(n + 1, numeric_limits<int>::max());
        minDist[start] = 0;
        for (int i = 1; i < n; i++)
        { // 对所有的边进行n-1次的松弛
            for (vector<int> &side : grid)
            {
                int from = side[0];
                int to = side[1];
                int price = side[2];
                // 松弛操作
                // minDist[from] != INT_MAX 防止从未计算过的节点出发
                if (minDist[from] != numeric_limits<int>::max() && minDist[to] > minDist[from] + price)
                {
                    minDist[to] = minDist[from] + price;
                }
            }
        }
        if (minDist[end] == numeric_limits<int>::max())
        {
            cout << "unconnected" << endl;
        }
        else
        {
            cout << minDist[end] << endl;
        }
    }
    // 2024.07.15 Bellman_ford 队列优化算法 解决第一类冗余
    void SPFA()
    {

        /*
         Bellman算法存在两类冗余：
         1.第一类是每一次都对所有的边进行松弛操作，松弛操作最终的目的还是修改节点to的值，
          但如果节点from在上一轮迭代中，没有变化，那么这轮其实是不需要更新节点from对应的end的，
          因为他们的值本身也不会变化。
          ==》 只需要对 上一次松弛的时候更新过的节点作为出发节点所连接的边 进行松弛就够了。
          ==》 基于队列优化的Bellmanford算法 ：SPFA
         2.第二类
          即每次都对上次更新过的所有from节点在下一次都进行更新，但其实依据最短路径原理
          每次只需要选上一轮迭代结束后 from最小的出来即可 因为选一些不是最小的，现在即使更新了，
          也不可能是最小值，后续也会被再次更新，故而本次更新是不必要的
        1）每个节点链接了哪些边，邻接表
        2）mindist 存储各个最小值
        3）队列 在极端情况下，即：所有节点都与其他节点相连，每个节点的入度为 n-1 （n为节点数量），所以每个节点最多加入 n-1 次队列
        */
        int n, m, p1, p2, val;
        struct Edge
        {
            int to;
            int val;
            // 初始化构造函数
            Edge(int t, int w) : to(t), val(w) {}
        };
        vector<list<Edge>> grid;
        while (m--)
        {
            cin >> p1 >> p2 >> val;
            grid[p1].push_back(Edge(p2, val));
        }
        queue<int, vector<int>> open_pq;
        vector<int> minDist(n, numeric_limits<int>::max());
        int start = 1;
        int end = n;
        minDist[start] = 0;
        open_pq.push(start);
        while (!open_pq.empty())
        {
            int current = open_pq.front();
            open_pq.pop();
            for (const Edge &side : grid[current])
            {
                // 松弛操作 判断大小
                if (minDist[side.to] > minDist[current] + side.val)
                {
                    minDist[side.to] = minDist[current] + side.val;
                    open_pq.push(side.to);
                }
            }
        }
        if (minDist[end] == numeric_limits<int>::max())
            cout << "Not have shortest path from start to end";
        else
            cout << "the shortest path is " << minDist[end];
    }
    // Bellmanford 判断负权回路
    /*
    在没有负权回路的图中，松弛 n 次以上 ，结果不会有变化。
    但本题有 负权回路，如果松弛 n 次，结果就会有变化了，因为 有负权回路 就是可以无限最短路径（一直绕圈，就可以一直得到无限小的最短距离）。
    那么每松弛一次，都会更新最短路径，所以结果会一直有变化。
    基于SPFA队列优化的:
        如果使用 SPFA 那么节点都是进队列的，那么节点进入队列几次后 足够判断该图是否有负权回路呢？
        在 0094.城市间货物运输I-SPFA 中，我们讲过 在极端情况下，即：所有节点都与其他节点相连，每个节点的入度为 n-1 （n为节点数量），所以每个节点最多加入 n-1 次队列。
        那么如果节点加入队列的次数 超过了 n-1次 ，那么该图就一定有负权回路。
    */
    // TODO:Bellmanford 单源有限最短路,之后再做
    /*
    最多经过 k 个城市的条件下，而不是一定经过k个城市，也可以经过的城市数量比k小，但要最短的路径。
    */
    // Folyd算法
    
    // A*算法
    // D*算法
};