// 二维平面两个点 第三个点在二维平面上的投影点 
#include <vector>
#include <iostream>
/*
1.计算的方法是什么？基本的数学原理为啥
2.C++写的过程中 如何表示点 如何表示点的相乘 
3.C++中如何使用矩阵 ？


*/
int main(){
    std::vector<int> P1={1,2};
    std::vector<int> P2={3,4};
    std::vector<int> P3={2,5};
    // 比例因子k=（P3-P1）*（P2-P1）/(|P2-P1|*|P2-P1|)
    int a=(P1[1]-P2[1])/(P1[0]-P2[0]);
    int b=P1[1]-a*P1[0];
    int z1=a*P1[0]+b*P2[0]+P3[0];
    int z2=a*P1[1]+b*P2[1]+P3[1];
    int z3=sqrt(z1*z1+z2*z2);
    int sq=sqrt(a*a+b*b);
    int final = z3/sq;
    return final;
}

#include <iostream>
#include <cmath>

using namespace std;

// Calculate dot product of two vectors
double dotProduct(double x1, double y1, double x2, double y2) {
    return x1 * x2 + y1 * y2;
}

// Calculate length of a vector
double vectorLength(double x, double y) {
    return sqrt(x * x + y * y);
}

// Calculate projection point
pair<double, double> projectionPoint(double x1, double y1, double x2, double y2, double xq, double yq) {
    // Calculate direction vector of line P1P2
    double vx = x2 - x1;
    double vy = y2 - y1;

    // Calculate vector from P1 to Q
    double pq_x = xq - x1;
    double pq_y = yq - y1;

    // Calculate dot product of vectors P1Q and v
    double dot_pv = dotProduct(pq_x, pq_y, vx, vy);

    // Calculate length of vector v
    double v_length_square = vx * vx + vy * vy;

    // Calculate scalar projection of P1Q onto v
    double scalar_proj = dot_pv / v_length_square;

    // Calculate projection point P
    double px = x1 + scalar_proj * vx;
    double py = y1 + scalar_proj * vy;

    return make_pair(px, py);
}

int main() {
    double x1 = 1.0, y1 = 1.0; // Coordinates of point P1
    double x2 = 4.0, y2 = 4.0; // Coordinates of point P2
    double xq = 2.0, yq = 2.0; // Coordinates of point Q

    pair<double, double> proj_point = projectionPoint(x1, y1, x2, y2, xq, yq);
    
    cout << "Projection point coordinates: (" << proj_point.first << ", " << proj_point.second << ")" << endl;

    return 0;
}


// 已经有一个函数可以生成1-7的等概率随机整数【1/7】；用这个函数完成一个1-10之间等概率的随机数【1/10】
#include <iostream>
int random_7(){return;}
int random_10(){
    int num1=random_7();
    int num2=random_7();
    int num3=(num1-1)*7+num2; // 0-6 *7+7
    return num3%10+1;
}
