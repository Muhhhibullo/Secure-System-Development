#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void work(int* arr, unsigned N) {
    for(int i=1; i<N; i++) {
        arr[i] = arr[i-1] * 2;
    }
    // Do not free(arr) here!
}

void program2(unsigned N) {
    int* arr = (int*)malloc(N * sizeof(*arr));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    memset(arr, 0, N * sizeof(*arr)); // Fix: Initialize the entire array
    arr[0] = 1;
    work(arr, N);
    for(int i=0; i<N; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
    free(arr); // Free memory after use
}

int main() {
    program2(4); // Should print the array [1, 2, 4, 8]
}