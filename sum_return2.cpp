#include<iostream>
#include<time.h>
using namespace std;
const int N = 1e7;

int a[N];
int sum;

int main()
{
    for(int i = 0; i < N; i++)
        a[i] = i + 1;
    clock_t timesum;
    for(int t =0;t<10;t++){
        clock_t start, end;
        start = clock();
        for (int m = N; m > 1; m /= 2) // log(n)个步骤
            for (int i = 0; i < m / 2; i++)
                a[i] = a[i*2] + a[i*2 + 1];
        end = clock();
        timesum+=end-start;
    }
    double time_taken = double(timesum) * 1000.0 / CLOCKS_PER_SEC;  // Convert to milliseconds
    cout << N << "用时" << time_taken/10.0 << "毫秒" << endl;
    return 0;
}
