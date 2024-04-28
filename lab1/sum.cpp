#include<iostream>
#include<time.h>
using namespace std;
const int N = 1e7;

int a[N];
int sum;
void _sum()
{
    for(int i = 0; i < N; i++){
            sum += a[i];
        }
}
int main()
{
    for(int i = 0; i < N; i++)
        a[i] = i + 1;
    clock_t sumtime;
    for(int t = 0;t<10;t++){
        clock_t start, end;
        start = clock();
        _sum();
        end = clock();
        sumtime += end-start;
    }
    double time_taken = double(sumtime) * 1000.0 / CLOCKS_PER_SEC;  // Convert to milliseconds
    cout << N << "用时" << time_taken/10.0 << "毫秒" << endl;
    return 0;
}
