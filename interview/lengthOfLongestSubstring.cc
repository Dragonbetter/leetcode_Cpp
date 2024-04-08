#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

int lengthOfLongestSubstring_map(std::string a) {
    /*
        leetcode 
        面试问题 相应的map.find出现问题 ！
        目前的程序效率较低，因为它是一个嵌套循环，而且在内层循环中每次都重新创建了一个 map。
        可以通过使用双指针技术（也称为滑动窗口技术）来改进算法的时间复杂度。
        即不用每次创建map，而是在移动到下一格后将对应的a[i]移除即可 ==》 map.erase(a[i]);
        // 每次的j其实不用重复的去进行赋值 因为依据滑动窗口 如果从1-5没有重复 加上6重复，则此时i=0，j=5；
        当i=1时，可知从i=1到j=5仍然是非重复的；故而只需要继续判断j++后是否相同即可；
    ==> 其实更进一步 不需要要map，用set即可 因为此处只需要可以find，可以存储非重复的数据
    */
    
    std::vector<int> result(a.size(),0);
    std::map<char, int> b;
    int count = 0;
    int j = 0;

    for(int i=0;i<a.size();i++){
        while (j < a.size() && b.find(a[j]) == b.end()) {
            b[a[j]] = 1;
            count++;
            j++;
        }
        result[i] = count;
        // 不用清零 重新配对 基于已有的匹配结果继续进行分析
        b.erase(a[i]);
        count--; 
    }
    int max = 0;
    for (int i = 0; i < result.size(); ++i) {
        if (result[i] > max) {
            max = result[i];
        }
    }
    return max;
    // 输出数据的 
    /*
    int max = 0;
    int max_index = 0;
    for (int i = 0; i < result.size(); ++i) {
        if (result[i] > max) {
            max = result[i];
            max_index = i;
        }
    }

    for (int i = max_index; i < max_index + max; ++i) {
        std::cout << a[i];
    }
    std::cout << std::endl;
    
    */ 
    }

int lengthOfLongestSubstring(std::string s) {
    /*
    时间复杂度：O(N)O(N)O(N)，其中 NNN 是字符串的长度。左指针和右指针分别会遍历整个字符串一次。

    空间复杂度：O(∣Σ∣)，其中 Σ 表示字符集（即字符串中可以出现的字符），
    ∣Σ∣ 表示字符集的大小。在本题中没有明确说明字符集，
    因此可以默认为所有 ASCII 码在 [0,128)内的字符，即 ∣Σ∣=128。我们需要用到哈希集合来存储出现过的字符，
    而字符最多有 ∣Σ∣ 个，因此空间复杂度为 O(∣Σ∣)O(|\Sigma|)O(∣Σ∣)。 
    */
    if(s.size() == 0) return 0;
        // 用set即可 
    std::unordered_set<char> lookup;
    int maxStr = 0;
    int left = 0;
    for(int i = 0; i < s.size(); i++){
        // 滑动窗口 i为终点，left为起点
        while (lookup.find(s[i]) != lookup.end()){
            lookup.erase(s[left]);
            left ++;
        }
        maxStr = std::max(maxStr,i-left+1);
        lookup.insert(s[i]);
    }
    return maxStr;
}

int main(){
    return 0;
}
