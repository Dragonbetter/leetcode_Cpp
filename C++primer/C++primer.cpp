#include <iostream> 
#include <fstream> // 读文件
#include <sstream> // 读string
#include <string> // 各种迭代器类型
#include <vector>  
#include <map>
#include <set>
#include <memory> // 智能指针 
#include <algorithm>
using namespace std;
// 有些时候 尽量使用std:: 因为可以避免冲突 
/* C++ 12章 12.05 
文本查询程序设计  
目的：允许用户在一个给定的文件中 查询单词 查询结果是单词在文件中出现的次数以及其所在行的列表 行会按照升序输出
==》分析：
1：按行读取文件(ifstream) 保存文件 vector<string> 保存文件后 
2：拆分行为单词列表 运用map 保存对应的word 和 行的集合set（升序 非重复）
3：程序具备功能 ：输入查询 （map） 输出对应值（string，次数，行集合）
*/

class QueryResult;  //先声明 后定义 
class TextQuery{
    public:
    // 构造函数 在外部定义具体的含义 构造函数的几种形式 类似于IO的一般都写在外面 
    TextQuery(ifstream&);
    // 声明行号的类型 因为行号可能无限大 用int long等都不适合 应该用size_type成员
    using line_no = vector<string>::size_type;
    // query 操作 输入word 返回set行号等
    QueryResult query(const string&) const;
    private:
    // 读入文件与输出之间需要共享的数据结构为 行号集合 set 以及完整的行内容 数据成员都定义在private内 
    shared_ptr<vector<string>> file;
    map<string,shared_ptr<set<line_no>>> wm; // 模版类型 需要注明类型 map<键type，值type>,shared_ptr<Type>,set<type>
};
TextQuery::TextQuery(ifstream &is):file(new vector<string>){
    // 因为file是shared_ptr指针 故而需要运用new vector ；而其在自由空间分配的内存是无名的，因此file是一个指针 指向该对象（无名空间） 故而后续运用-> 相当于*p.而且该file是
    // 构造函数-》 按行读取文件数据--保存行和行号--分割行数据--对每个word调用map--存取行号
    string text;
    while(getline(is,text)){
        file->push_back(text); // 保存文件此处为何要用->?
        int n=file->size()-1; // 保存行号 
        istringstream line(text); // 保存text的一个拷贝到line对象中 后续运用cin>>逐个读取
        string word;
        while(line>>word){
            // 将单词添加到map中 如果存在则加入到value中 并返回地址 如果不存在则新建key -word
            auto &lines = wm[word];
            // 对map进行下标操作时 得到的是是一个mapped-type对象 对于map而言 即值的类型 此处即shared-ptr智能指针类型
            // &lines 表示对指针的引用；改变lines也会改变wm中的元素 如果存在word则返回 相应的地址 如果不存在 则相应的返回空指针 
            if(!lines){
                // 不存在word的情况下 为key对应的value建立一个set
                // 除了在初始化情况下 对其shared-ptr进行赋值 其他情况下 运用reset赋值 令lines指向new新建的无名空间 
                lines.reset(new set<line_no>);
            }
            // （*lines）.insert 先解shared——ptr获得对象set迭代器，而后插入n set默认有序
            lines->insert(n);
        }    
    }
}
class QueryResult{
    // 作为输出 输出包括单词 次数 行号 行内容
    // 友元函数 使得print可以调用内部成员 
    // print  在QueryResult之中 此处只是声明 为了相应的能使用 数据成员 
    friend ostream& print(ostream&,const QueryResult&);
    public:
    // 构造函数 传给类这三类数据 从而构造对应的类对象
    // 声明行号的类型 因为行号可能无限大 用int long等都不适合 应该用size_type成员
    using line_no = vector<string>::size_type;
    QueryResult(string s,shared_ptr<set<line_no>> p,shared_ptr<vector<string>> f):sought(s),lines(p),file(f){}  
    using Iter = std::set<line_no>::iterator; // 定义一个迭代器类型 
    Iter begin() const {return lines->begin();}
    Iter end() const {return lines->end();}
    // TODO 此处直接返回file会有问题吗？ 此处返回的其实是file的一个副本 两个智能指针的引用计数互相独立 
    shared_ptr<vector<string>> get_file() const {return std::make_shared<vector<string>>(file);}
    private:
    string sought;
    shared_ptr<set<line_no>> lines;
    shared_ptr<vector<string>> file;
};
QueryResult TextQuery::query(const string&sought) const{
    // 返回类型为QueryResult  依据查询结果返回
    static shared_ptr<set<line_no>> nodata(new set<line_no>);
    // 查找word 运用find 返回迭代器 first为key second为值 set
    auto loc = wm.find(sought);
    if (loc!=wm.end()){
        return QueryResult(sought,nodata,file);
    }
    else{
        return QueryResult(sought,loc->second,file);
    }
}
string make_plural(size_t ctr,const string &word,const string &ending){
    // 依据ctr值的大小 决定time单复数
    return (ctr>1)?word+ending:word;
}
ostream &print(ostream &os,const QueryResult &qr){
    // 如果找到了单词 打印单词 和出现的次数
    os<<qr.sought<<"occurs"<<qr.lines->size()<<""<<make_plural(qr.lines->size(),"times","s")<<endl;
    // 打印相应的单词出现的每一行 先打印行号 再打印内容
    for (auto num:*qr.lines){ 
        // 先.取出lines成员为shared——ptr指针，而后基于此*解引用得到对应的对象值
        os<<"\t (line"<<num+1<<")"<<*(qr.file->begin()+num)<<endl;
        // qr.file得到shared-ptr指针 而->begin() 相当于(*(qr.file)).begin 先取对应的vector 再取vector的begin成员 继而+num 移动迭代器 再解引用迭代器    
    }
    return os;
}
void runQueries(ifstream &infile){
    // 首先定义类需要的操作,在构造函数阶段进行数据输入处理；query成员用于查询s；通用print成员用于输出
    // infile指向要处理的文件 TextQuery获取文件 构造map(string,set) 
    TextQuery tq(infile); 
    // 交互式设计 用户输入数据 
    while (true){
        cout<<"enter word to look for ,or q to quit:";
        string s;
        // 文件尾部 或用户输入q
        if (!(cin>>s) || s=="q"){break;}
        // 输入完毕 进行查询 print自定义的函数 
        print(cout,tq.query(s))<<endl;
    }
}
/* C++ 15章 文本查询存程序扩充 12.12
接口类 Query 调用相应的Querybase以及相应的重载运算符
基础类QueryBase
派生类WordQuery NotQuery BinaryQuery AndQuery OrQuery
重载运算符 | & ~
==============================================
实际的执行过程为
Query andq = Query(s1) * Query(s2)
cout<<andq<<endl;
1：首先由Query(std::string)的构造函数版本 构造对象 =》 初始对象为WordQuery对象
2：*执行重载操作 返回对应的andquery对象 
3：<<andq 调用重载运算符 输出andq.rep => 依据andq对象的类型 动态选用调用q->rep()为何 q由query决定 

*/
class Query_base{
    // 抽象基类 
    friend class Query;
    // Query接口类需要调用对应的eval和rep函数 故而需要声明成友元
    protected:
        using line_no = TextQuery::line_no;
        // 虚析构函数 便于后续在删除基类指针时 能正确删除基类指针正确指定的动态类型 
        virtual ~Query_base()=default;
    private:
    // 两个纯虚函数 
        virtual QueryResult eval(const TextQuery&) const=0;
        virtual std::string rep() const =0;
};
class Query{
    // 声明对应的重载运算符函数为友元 因为每个都需要相应的构造函数 将一个shareptr指针转换为Query对象 利用构造函数
    // 三个重载函数在return阶段都调用了构造函数 虽然实际指得不一样 但都是基于Querybase
    friend Query operator~(const Query &);
    friend Query operator|(const Query &,const Query &);
    friend Query operator&(const Query &,const Query &);
    public:
        Query(const std::string&); // 构建一个新的word-query 用于查找一个给定的单词 其他的可以由其构建？
        // 接口函数 调用对应的Query-base操作
        QueryResult eval(const TextQuery &t) const {return q->eval(t);}
        std::string rep() const {return q->rep();}
    private:
        // TODO 为什么要用shared——ptr 用普通的指针会带来什么不同呢 
        Query(std::shared_ptr<Query_base> query):q(query){} // 接受指针对象的构造函数
        std::shared_ptr<Query_base> q; // 指向query-base基类的指针，在后期的eval于rep中依据其实际的动态对象选择调用的派生类
};

std::ostream& operator<<(std::ostream &os,const Query &query){
    // Query:rep()通过他的Query-bsae指针对rep()进行虚调用
    return os<<query.rep();
}
class WordQuery:public Query_base{
    // 全为private TODO 
    friend Query; // Query需要调用
    WordQuery(const std::string &s):query_word(s){}
    // 两个纯函数 默认派生类都需要去覆盖 不然该类则同样为抽象基类 无法创建对象
    // queryresult 基础查询功能 
    QueryResult eval(const TextQuery &t) const {return t.query(query_word);}
    std::string rep() const {return query_word;}
    std::string query_word; // 要查找的单词
};
// Query对应的构造函数 如果此处最终没有定义成基类指针的类型 则相应的重载运算符无法直接确定两边的对象都是query 而需要分类讨论
// 但指定为querybase类的指针后 其具体的类可以由其自己确定 但都是基于query的指针成员 q；q不需要指向不同的类 只需要指向基类query-base==》多态
// 内联 优化规模较小 流程直接 频繁调用的函数 
inline Query::Query(const std::string &s):q(new WordQuery(s)){}
class NotQuery:public Query_base{
    friend Query operator~(const Query &); // 需要使用其构造函数 
    NotQuery(const Query &q):query(q){}
    QueryResult eval(const TextQuery &) const; // 放到外部去定义 
    std::string rep() const {return "~("+query.rep()+")";}
    Query query;
};
inline Query operator~(const Query& operand){
    // 1.~运算符接受一个query对象 eg：~Query(s1)
    // 2.将该query对象传递给notQuery对象 新建该对象 并返回 一个基于query-base的指针指向它
    // 3.后续依据query对象中的query-base指针的实际指向 选择对应的eval何rep函数 此处实际指向为Notquery
    return std::shared_ptr<Query_base>(new NotQuery(operand));
}
QueryResult NotQuery::eval(const TextQuery&text) const{
    // 取出对应的结果 后续再在全集中进行反除
    auto result = query.eval(text);
    auto ret_lines = shared_ptr<set<line_no>>();
    auto beg = result.begin(),end=result.end();
    auto sz =result.get_file()->size();
    // 对于输入的每一行 如果该行不在result当中，将其添加道ret-lines中
    for(size_t n=0;n!=sz;++n){
        // 因为两个set都是有序的 故而同步增长 
        // 依次遍历行 判断其是否存在于result当中 
        // 双指针 
        if(beg==end||*beg!=n){
            ret_lines->insert(n);
        }
        else if (beg!=end){
            // 相等时 说明该行不需要 而后继续向后 
            ++beg;
        }
    }
    return QueryResult(rep(),ret_lines,result.get_file());
}


class BinaryQuery:public Query_base{
    // 抽象基类 
    // 构造函数 分析获得对应的代码
    protected:// 此类还有派生类 故而所有成员定义成protected 类似于其他上述类 无派生类了 故而定义为private
        // 注意此处的s没定义成引用形式 一方面也是因为传入的字符较为的简单
        BinaryQuery(const Query &l,const Query &r,std::string s):lhs(l),rhs(r),opSym(s){}
    // eval()函数没有进行覆盖 直接继承纯虚函数 故而也是抽象基类
        std::string rep() const {return "("+lhs.rep()+""+opSym+""+rhs.rep()+")";}
        Query lhs,rhs; // 定义对应的binary 两者的对象 
        std::string opSym; // 定义对应的运算符
};


class AndQuery:public BinaryQuery{
    friend Query operator&(const Query&,const Query&);
    // 构造函数 AndQueryp派生类运用 BinaryQuery类进行构造 
    AndQuery(const Query &left,const Query &right):BinaryQuery(left,right,"&"){}
    // 继承binaryQuery的rep()函数 并定义对应的eval函数
    QueryResult eval(const TextQuery&) const;   
};
inline Query operator&(const Query &lhs,const Query &rhs){
    return std::shared_ptr<Query_base>(new AndQuery(lhs,rhs));
}
QueryResult AndQuery::eval(const TextQuery&text) const{
    auto left=lhs.eval(text),right=rhs.eval(text);
    auto ret_lines = make_shared<set<line_no>>();
    // set_intersection 合并两个set =》需要两个输入迭代器的begin1 end1 begin2 end2； 以及一个输出迭代器dest 
    // 用的是插入迭代器 由*ret-lines与其对应迭代器位置组成 合并后的元素相应的放到这个迭代器对应的地方
    // 返回递增后的dest迭代器 表示写入dest的最后一个元素之后的位置 
    // 当set-intersection向inserter写入数据时 实际上是向ret-lines插入一个新元素
    set_intersection(left.begin(),left.end(),right.begin(),right.end(),inserter(*ret_lines,ret_lines->begin()));
    return QueryResult(rep(),ret_lines,left.get_file());
}


class OrQuery:public BinaryQuery{
    friend Query operator|(const Query&,const Query&);
    OrQuery(const Query &left,const Query &right):BinaryQuery(left,right,"|"){}
    QueryResult eval(const TextQuery&) const;
};
inline Query operator|(const Query&lhs,const Query&rhs){
    return std::shared_ptr<Query_base>(new OrQuery(lhs,rhs));
}
QueryResult OrQuery::eval(const TextQuery& text) const{
    // lhs与rhs应该是binaryQuery的成员对象 直接继承了
    // 通过query成员lhs,rhs进行的虚调用 query.eval->(query.q)->eval() 依据query.q实际指得类型 去调用派生类中的eval
    auto right=rhs.eval(text),left=lhs.eval(text);
    //将左侧运算对象的行号拷贝道结果set中=>
    // make_shared<T>(args)=>args用来初始话T类型的数据 即set<T>(args) 运用args的数据来初始话set
    // set的初始话的 默认构造函数 用一个同类型容器的拷贝，或一个值范围来初始话关联容器 
    auto ret_lines=make_shared<set<line_no>>(left.begin(),right.end());
    // 插入右侧运算对象所得的行号
    ret_lines->insert(right.begin(),right.end());
    return QueryResult(rep(),ret_lines,left.get_file());
}



