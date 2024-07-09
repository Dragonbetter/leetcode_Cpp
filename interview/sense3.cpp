// 
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


// 1 2 2 3 2 2 2 4 5 2 6 
int main()
{
    vector<int> a = {1 ,2, 2, 3, 2, 2, 2, 4, 5, 2, 6};   
    int value = 2;
    bool found =true;
    std::vector<int> a = {1, 2, 2, 3, 2, 2, 2, 4, 5, 2, 6};
    int target = 2;

    // 使用双指针法，一个指针用于遍历 vector，另一个指针用于记录非目标元素应该存放的位置
    int pos = 0; // 记录非目标元素应该存放的位置
    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != target) {
            a[pos++] = a[i];
        }
    }

    // 调整 vector 大小以删除多余的元素
    a.resize(pos);

    // 打印删除后的 vector
    for (int num : a) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
    
    
    for(auto it =a.begin();it!=a.end();){
        if(*it == value){
            it = a.erase(it);
        }
        else{
            it++;
        }
    }
    for(int i=0;i<a.size();i++){     
        cout<<a[i]<<endl;
    }
    return 0;
}
