#include <CL/sycl.hpp>
#include <iostream>
#include <vector>

using namespace sycl;

int main() {
    const int N = 1024;  // 数组大小
    queue q;  // 创建一个队列

    // STEP 1: 分配USM设备内存
    int* arrayA = malloc_device<int>(N, q);
    int* arrayB = malloc_device<int>(N, q);
    int* arrayC = malloc_device<int>(N, q);

    // STEP 2: 初始化数组A和B
    q.parallel_for(range<1>(N), [=](id<1> i) {
        arrayA[i] = 1;  // 将所有元素设为1
        arrayB[i] = 2;  // 将所有元素设为2
    }).wait();  // 确保初始化完成

    // STEP 3: 计算数组A和B的和，存储到数组C
    q.parallel_for(range<1>(N), [=](id<1> i) {
        arrayC[i] = arrayA[i] + arrayB[i];
    }).wait();  // 确保计算完成

    // STEP 4: 读取并打印结果
    auto result = malloc_shared<int>(N, q);
    q.memcpy(result, arrayC, sizeof(int)*N).wait();  // 将结果从设备内存复制到共享内存
    for (int i = 0; i < N; i++) {
        std::cout << "C[" << i << "] = " << result[i] << std::endl;
    }

    // 释放内存
    free(arrayA, q);
    free(arrayB, q);
    free(arrayC, q);
    free(result, q);

    return 0;
}