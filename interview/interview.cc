#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>


// 面试题  
// 导入输入输出包 以及using指示 
// 定义并确定数据结构 
// 选择对应的算法
// ===============================================================================
// 绑定两个数据结构
void interview_12_22_vector()
{
    // 一个列表里面保存着3个列表，每个列表都有一个字符和一个数字组成，我需要依据每个列表里面的第二个数字的大小将这个列表进行重新排序。python如何实现
    // 由于 C++ 的 vector 不能像 Python 那样直接存储不同类型的元素，所以这里我使用了 vector<int> 并存储了字符的 ASCII 值
    // 定义一个包含3个vector的vector，每个内部vector包含一个字符和一个数字
    std::vector<std::vector<int>> nested_list = {{'a', 2}, {'b', 1}, {'c', 3}};

    // 使用std::sort和lambda表达式进行排序
    // [capture list](parameter list)->return type {function body}
    std::sort(nested_list.begin(), nested_list.end(), 
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  return a[1] < b[1];
              });

    // 打印排序后的结果
    for (const auto & item : nested_list) {
        std::cout << static_cast<char>(item[0]) << ", " << item[1] << std::endl;
    }
}
void interview_12_22_pair()
{
    // 定义一个包含 pair 的 vector，每个 pair 包含一个字符和一个数字
    std::vector<std::pair<char, int>> list = {{'a', 2}, {'b', 1}, {'c', 3}};

    // 使用 std::sort 和 lambda 表达式进行排序
    std::sort(list.begin(), list.end(),
              [](const std::pair<char, int>& a, const std::pair<char, int>& b) {
                  return a.second < b.second;
              });

    // 打印排序后的结果
    for (const auto & item : list) {
        std::cout << item.first << ", " << item.second << std::endl;
    }
}
class Item {
public:
    char key;
    int value;

    Item(char k, int v) : key(k), value(v) {}

    // 重载 '<' 运算符，用于比较 隐式this指针 
    bool operator<(const Item& other) const {
        return value < other.value;
    }
};
void interview_12_22_class()
{
    /*
      push:push_back,push_front 拷贝已有对象加入到vector中 故而先得定义成Items
      insert(p,t):在迭代器p指向的元素  之前 插入一个值为t的元素
      emplace:emplace_back,emplace_front 接受对应的参数去直接构造对象 可以直将参数传递给元素类型的构造函数
    */
    std::vector<Item> items;
    items.emplace_back('a', 2);
    items.emplace_back('b', 1);
    items.emplace_back('c', 3);

    // 排序不需要提供比较函数，因为我们已经重载了 '<' 运算符
    std::sort(items.begin(), items.end());

    for (const auto& item : items) {
        std::cout << item.key << ", " << item.value << std::endl;
    }
}
void interview_12_22_tuple()
{
// 创建一个 tuple 的 vector，每个 tuple 包含一个字符和一个数字
    std::vector<std::tuple<char, int>> items;
    items.emplace_back('a', 2);
    items.emplace_back('b', 1);
    items.emplace_back('c', 3);

    // 使用 std::sort 进行排序，依据 tuple 的第二个元素
    std::sort(items.begin(), items.end(),
              [](const std::tuple<char, int>& a, const std::tuple<char, int>& b) {
                  return std::get<1>(a) < std::get<1>(b);
              });

    // 打印排序后的 tuple
    for (const auto& item : items) {
        std::cout << std::get<0>(item) << ", " << std::get<1>(item) << std::endl;
    }

}
int main(){
    interview_12_22_vector();
    return 0;
}
