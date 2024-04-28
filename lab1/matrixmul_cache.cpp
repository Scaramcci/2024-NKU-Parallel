#include<iostream>
#include<time.h>
using namespace std;
const int N = 10000;

int a[N];
int b[N][N];
int sum[N];

int main()
{
    for(int i = 0; i < N; i++)
        a[i] = i + 1;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            b[i][j] = i + j + 2;  // i+j+1 改为 i+j+2，因为两者都加1了
    clock_t timesum=0;
    for(int t = 0;t<50;t++){
        clock_t start, end;
        start = clock();
        for(int i = 0; i < N; i++)
            sum[i] = 0;
        for(int j = 0;j<N;j++)
            for(int i= 0; i < N; i++)
                sum[i] += b[j][i] * a[j];
    
    end = clock();
    timesum+=end-start;
    }
    double time_taken = (double(timesum) * 1000.0 / CLOCKS_PER_SEC)/50.0;  // Convert to milliseconds
    cout << N << "优化算法用时" << time_taken << "毫秒" << endl;
    return 0;
}
