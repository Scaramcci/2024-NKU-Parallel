#include <cuda.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

__global__ void assign_clusters(float *data, int *clusters, float *centroids,
                                int n_points, int n_clusters, int dim) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    if (index < n_points) {
        int best_cluster = 0;
        float min_distance = FLT_MAX;
        for (int c = 0; c < n_clusters; c++) {
            float distance = 0.0;
            for (int d = 0; d < dim; d++) {
                float diff = data[index * dim + d] - centroids[c * dim + d];
                distance += diff * diff;
            }
            if (distance < min_distance) {
                min_distance = distance;
                best_cluster = c;
            }
        }
        clusters[index] = best_cluster;
    }
}

__global__ void update_centroids(float *data, int *clusters, float *centroids,
                                 int *counts, int n_points, int n_clusters, int dim) {
    extern __shared__ float shared_data[];
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    int stride = blockDim.x * gridDim.x;
    int thread_id = threadIdx.x;

    for (int c = thread_id; c < n_clusters * dim; c += blockDim.x) {
        shared_data[c] = 0;
    }
    __syncthreads();

    for (int i = index; i < n_points; i += stride) {
        int cluster_id = clusters[i];
        for (int d = 0; d < dim; d++) {
            atomicAdd(&shared_data[cluster_id * dim + d], data[i * dim + d]);
        }
    }
    __syncthreads();

    if (thread_id < n_clusters * dim) {
        atomicAdd(&centroids[thread_id], shared_data[thread_id]);
    }
    if (thread_id < n_clusters) {
        atomicAdd(&counts[thread_id], 1);
    }
}

void kmeans(float *h_data, int n_points, int dim, int n_clusters, int max_iter) {
    float *d_data, *d_centroids;
    int *d_clusters, *d_counts;

    cudaMalloc(&d_data, n_points * dim * sizeof(float));
    cudaMalloc(&d_centroids, n_clusters * dim * sizeof(float));
    cudaMalloc(&d_clusters, n_points * sizeof(int));
    cudaMalloc(&d_counts, n_clusters * sizeof(int));

    cudaMemcpy(d_data, h_data, n_points * dim * sizeof(float), cudaMemcpyHostToDevice);

    // Initialize centroids randomly
    for (int i = 0; i < n_clusters; i++) {
        int idx = rand() % n_points;
        cudaMemcpy(&d_centroids[i * dim], &d_data[idx * dim],
                   dim * sizeof(float), cudaMemcpyDeviceToDevice);
    }

    int threadsPerBlock = 256;
    int blocksPerGrid = (n_points + threadsPerBlock - 1) / threadsPerBlock;

    for (int iter = 0; iter < max_iter; iter++) {
        cudaMemset(d_counts, 0, n_clusters * sizeof(int));
        assign_clusters<<<blocksPerGrid, threadsPerBlock>>>(d_data, d_clusters,
                                                            d_centroids, n_points,
                                                            n_clusters, dim);
        update_centroids<<<blocksPerGrid, threadsPerBlock, n_clusters * dim * sizeof(float)>>>(
            d_data, d_clusters, d_centroids, d_counts, n_points, n_clusters, dim);
    }

    // Copy back results
    float *h_centroids = new float[n_clusters * dim];
    cudaMemcpy(h_centroids, d_centroids, n_clusters * dim * sizeof(float), cudaMemcpyDeviceToHost);

    // Cleanup
    cudaFree(d_data);
    cudaFree(d_centroids);
    cudaFree(d_clusters);
    cudaFree(d_counts);
    delete[] h_centroids;
}

int main() {
    int n_points = 1024;
    int dim = 2;
    int n_clusters = 3;
    int max_iter = 300;
    float *data = new float[n_points * dim];

    // Generate random data
    srand(time(0));
    for (int i = 0; i < n_points * dim; i++) {
        data[i] = rand() / (float)RAND_MAX;
    }

    kmeans(data, n_points, dim, n_clusters, max_iter);

    delete[] data;
    return 0;
}