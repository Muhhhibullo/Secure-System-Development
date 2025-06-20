# Lab 3 - Memory Safety  
**Secure System Development - Spring 2025**  

In this lab, we will practice debugging memory-related issues with Valgrind.  

Create a `.md` step-by-step report of the actions you took with screenshots of key results.  

**Recommended reading:** [Memory Safety (Wikipedia)](https://en.wikipedia.org/wiki/Memory_safety)  

## Task 1 - Getting Started  
**Guide:** [Valgrind Quick Start](https://valgrind.org/docs/manual/quick-start.html)  

1. Install needed tools:  
   ```sh
   sudo apt install gcc valgrind  # For Debian/Ubuntu
   ```

2. Create `program1.c` with the following content:  
   ```c
   #include<stdio.h>
   #include<stdlib.h>

   void program1(int N) {
       int *arr = malloc(N);
       for(int i = 0; i < N; i++) {
           arr[i] = i * i;
           printf("arr[%d] = %d\n", i, arr[i]);
       }
   }

   int main() {
       program1(4); // Should print [0, 1, 4, 9]
   }
   ```

3. Compile with debugging flags:  
   ```sh
   gcc -Wall -Werror -g -std=c99 -o program1 -O0 program1.c
   ```
   - `-Wall`: Enable all warnings  
   - `-Werror`: Treat warnings as errors  
   - `-g`: Include debug symbols  
   - `-std=c99`: Use C99 standard  
   - `-O0`: Disable optimizations  

4. Run normally:  
   ```sh
   ./program1
   ```

5. Run with Valgrind:  
   ```sh
   valgrind --leak-check=full ./program1
   ```

6. **Report Requirements:**  
   - Explain Valgrind output (memory leaks/errors?)  
   - Reference relevant CWEs (if any)  
   - Propose fixes  
   - Verify fixes with Valgrind  

---  

## Task 2 - More Programs  
For each program below:  
1. Compile with debugging flags  
2. Run normally and with Valgrind  
3. Analyze output for memory issues  
4. Propose fixes  
5. Reference relevant CWEs  

### Program 2  
```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void work(int* arr, unsigned N) {
    for(int i=1; i<N; i++) {
        arr[i] = arr[i-1] * 2;
    }
    free(arr);
}

void program2(unsigned N) {
    int* arr = (int*)malloc(N * sizeof(*arr));
    memset(arr, 0, sizeof(*arr));
    arr[0] = 1;
    work(arr, N);
    for(int i=0; i<N; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
}

int main() {
    program2(4); // Should print [1, 2, 4, 8]
}
```

### Program 3  
```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void* program3(unsigned N) {
    void *arr = malloc(N * sizeof(*arr));
    if((N < 1) || (arr = NULL)) {
        printf("%s\n", "Memory allocation falied!");
        return NULL;
    }
    printf("%s\n", "Memory allocation success!");
    return arr;
}

int main() {
    int* arr = (int*)program3(4); // Should typically succeed
    free(arr);
}
```

### Program 4  
```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* getString() {
    char message[100] = "Hello World!";
    char* ret = message;
    return ret;
}

void program4() {
    printf("String: %s\n", getString());
}

int main() {
    program4();
}
```

---  

## Task 3 - Vulnerable HashMap Library  
Given `hash.c` and `hash.h` (not shown here):  

1. **Identify and fix at least 4 CWEs**  
   - Include: CWE number, name, and code snippet  
   - Refer to [CERT Secure Coding](https://securecoding.cert.org/)  

2. **Improve bad practices**  
   - Fix wrong data types  
   - Handle unchecked return values  

3. **Verify with Valgrind**  
   - Ensure no memory leaks/errors  
   - Include terminal screenshot  

**Example CWE Findings:**  
- **CWE-476**: NULL Pointer Dereference  
- **CWE-401**: Missing Memory Deallocation  
- **CWE-787**: Out-of-bounds Write  
- **CWE-125**: Out-of-bounds Read  
