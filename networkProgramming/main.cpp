#include <bits/stdc++.h>
using namespace std;
template <class val>
void swapVal(val &a, val &b)
{
    val temp;
    temp = a;
    a = b;
    b = temp;
}
int main()
{
    int intX = 2, intY = 3;
    double doubleX = 1.2, doubleY = 3.1;
    string str1 = "Code", str2 = "Learn";
    // gọi hàm swapVal() đối với 2 số nguyên
    swapVal(intX, intY);
    cout << "intX = " << intX << "\tintY = " << intY << endl;
    // gọi hàm swapVal() đối với 2 số thực
    swapVal(doubleX, doubleY);
    cout << "doubleX = " << doubleX << "\tdoubleY = " << doubleY << endl;
    // gọi hàm swapVal() đối với 2 chuỗi
    swapVal(str1, str2);
    cout << "Str1 = " << str1 << "\tstr2 = " << str2 << endl;
    return 0;
}