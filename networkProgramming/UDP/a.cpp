#include <bits/stdc++.h>

using namespace std;

class Solution
{
public:
    string gen(string a, int len)
    {
        for (int i = 1; i < len; i++)
            if (a[a.length() - 1] == '0')
                a = a + '1';
            else
                a = a + '0';
        return a;
    }
    int check(string a, string s)
    {
        int num = 0;
        for (int i = 0; i < a.length(); i++)
            num += !(a[i] != s[i]);
        return num;
    }
    int minOperations(string s)
    {
        string a = gen("0", s.length());
        string b = gen("1", s.length());
        cout << a << " " << b << endl;
        return min(check(a, s), check(b, s));
    }
};

int main()
{
    Solution a;
    cout << a.minOperations("0100");
    return 0;
}