%%cu
#include <stdio.h>
#include <cuda.h>
#include <time.h>

// function to initialize vector
void init_array(float *a, const int N) {
        int i;
        for(i=0; i<N; i++)
                a[i] = rand() % 4 + 1;
}

// function to initialize matrix
void init_mat(float *a, const int N, const int M) {
    int i, j;
    for(i=0; i<N; i++)
        for(j=0; j<M; j++)
                a[i*M+j] = rand() % 4 + 1;
}

//function to print vector
void print_vector(float *a, const int N, char *d) {
    int i;
    printf("Vector\n");
    for(i=0; i<N; i++)
        printf("%f  ",d, i, a[i]);
    printf("\n\n");
}

//function to print matrix
void print_matrix(float *a, const int N, const int M, char *d) {
        int i, j;
        printf("Matrix:\n");
        for(i=0; i<N; i++){
          for (j=0; j<M; j++)
              printf("%6.4f  ", a[i*M+j]);
          printf("\n");
    }
}

__global__ void kernel(float *vec, float *mat, float *out, const int N, const int M){
    int tid=threadIdx.x+blockIdx.x*blockDim.x;
    float sum=0;
    if(tid<M){
        for(int i=0; i<N; i++)
            sum += vec[i]*mat[(i*M)+tid];
        out[tid]=sum;
    }
}

int main (void) {
    float *host_a, *host_b, *host_c;
    float *dev_a, *dev_b, *dev_c;

    // vector is 1x3
    // matrix is 3x4
    // resultant will be 1x4
    int N=3;
    int M=4;
    host_a=(float*)malloc(sizeof(float)*N);
    host_b=(float*)malloc(sizeof(float)*N*M);
    host_c=(float*)malloc(sizeof(float)*M);
    
    init_array(host_a, N);
    init_mat(host_b, N, M);

    printf("<<<<<<<<<< initial data:\n");
    print_vector(host_a, N, "in-vector");
    print_matrix(host_b, N, M, "matrix");

    cudaMalloc((void**)&dev_a, sizeof(float)*N);
    cudaMalloc((void**)&dev_b, sizeof(float)*N*M);
    cudaMalloc((void**)&dev_c, sizeof(float)*M);

    cudaMemcpy(dev_a, host_a, sizeof(float)*N, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, host_b, sizeof(float)*N*M, cudaMemcpyHostToDevice);

    printf("\nRunning Kernel...\n\n");
    kernel<<<1, M>>>(dev_a, dev_b, dev_c, N, M);
    //printf("error code: %s\n",cudaGetErrorString(cudaGetLastError()));

    cudaMemcpy(host_c, dev_c, sizeof(float)*M, cudaMemcpyDeviceToHost);

    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    printf("\n\n>>>>>>>>>> final data:\n");
    print_vector(host_c, M, "out-vector");

    return 0;
};