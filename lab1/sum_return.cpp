#include<iostream>
#include<time.h>
using namespace std;
const int N = 1e7;

int a[N];
int sum;
void  recursion(int n)
{
    if (n == 1)
        return;
    else
    {
        for (int i = 0; i < n / 2; i++)
            a[i] += a[n-i-1];
            n = n / 2;
        recursion(n);
    }
}

int main()
{
    for(int i = 0; i < N; i++)
        a[i] = i + 1;
    
    clock_t timesum;
    for(int i = 0;i<10;i++){
        clock_t start, end;
        start = clock();
        recursion(N);
        end = clock();
        timesum+=end-start;
    }
    double time_taken = double(timesum) * 1000.0 / CLOCKS_PER_SEC;  // Convert to milliseconds
    cout << N << "用时" << time_taken/10.0 << "毫秒" << endl;
    return 0;
}
