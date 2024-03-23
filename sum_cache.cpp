#include<iostream>
#include<time.h>
using namespace std;
const int N = 1e7;

int a[N];
long long sum1,sum2,sum;
void _sum()
{
    for(int i = 0; i < N; i+=2){
            sum1 += a[i];
            sum2+=a[i+1];
        }
        sum = sum1+sum2;
}
int main()
{
    for(int i = 0; i < N; i++)
            a[i] = i + 1;
    clock_t timesum;
    for(int t = 0;t<10;t++){
        clock_t start, end;
        start = clock();
        _sum();
        end = clock();
        timesum+=end-start;
    }
    double time_taken = double(timesum) * 1000.0 / CLOCKS_PER_SEC;  // Convert to milliseconds
    cout << N << "用时" << time_taken/10.0 << "毫秒" << endl;
    return 0;
}
