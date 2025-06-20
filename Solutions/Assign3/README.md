# Memory Safety Report


## Author: Muhibullo Khujabekov (CS-02)

### Task 1
#### 1. Compiled the program using the following command:

```bash
gcc -Wall -Werror -g -std=c99 -o program1 -O0 program1.c
```

- **Explanation of Flags:**
  - `-Wall`: Enables all compiler warnings.
  - `-Werror`: Treats all warnings as errors, stopping compilation if any warnings are present.
  - `-g`: Generates debug information for use with tools like `gdb` and `valgrind`.
  - `-std=c99`: Specifies the C standard to use (C99 in this case).
  - `-O0`: Disables optimization, making debugging easier.
  - `-o program1`: Specifies the output executable name (`program1`).



#### 2. Run the Program with Valgrind

```bash
valgrind --leak-check=full ./program1
```

**Explanation of Valgrind Flags:**
- `--leak-check=full`: Provides detailed information about memory leaks.

#### 3. Analyze Valgrind Output
Valgrind will produce output similar to the following:

```
==21161== Memcheck, a memory error detector
==21161== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==21161== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==21161== Command: ./program1
==21161== 
arr[0] = 0
==21161== Invalid write of size 4
==21161==    at 0x1091AC: program1 (program1.c:7)
==21161==    by 0x1091FE: main (program1.c:13)
==21161==  Address 0x4a85044 is 0 bytes after a block of size 4 alloc'd
==21161==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==21161==    by 0x109184: program1 (program1.c:5)
==21161==    by 0x1091FE: main (program1.c:13)
==21161== 
==21161== Invalid read of size 4
==21161==    at 0x1091C2: program1 (program1.c:8)
==21161==    by 0x1091FE: main (program1.c:13)
==21161==  Address 0x4a85044 is 0 bytes after a block of size 4 alloc'd
==21161==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==21161==    by 0x109184: program1 (program1.c:5)
==21161==    by 0x1091FE: main (program1.c:13)
==21161== 
arr[1] = 1
arr[2] = 4
arr[3] = 9
==21161== 
==21161== HEAP SUMMARY:
==21161==     in use at exit: 4 bytes in 1 blocks
==21161==   total heap usage: 2 allocs, 1 frees, 1,028 bytes allocated
==21161== 
==21161== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1
==21161==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==21161==    by 0x109184: program1 (program1.c:5)
==21161==    by 0x1091FE: main (program1.c:13)
==21161== 
==21161== LEAK SUMMARY:
==21161==    definitely lost: 4 bytes in 1 blocks
==21161==    indirectly lost: 0 bytes in 0 blocks
==21161==      possibly lost: 0 bytes in 0 blocks
==21161==    still reachable: 0 bytes in 0 blocks
==21161==         suppressed: 0 bytes in 0 blocks
==21161== 
==21161== For lists of detected and suppressed errors, rerun with: -s
==21161== ERROR SUMMARY: 7 errors from 3 contexts (suppressed: 0 from 0)
```

**Explanation of Valgrind Output:**
- The program has a memory leak. Specifically, 4 bytes allocated by `malloc` are not freed before the program exits.
- The issue is related to **CWE-401: Missing Release of Memory after Effective Lifetime**.

<a href="https://ibb.co/1fm2GSYb"><img src="https://i.ibb.co/RpDHT5G7/image1.png" alt="image1" border="0"></a>


<a href="https://ibb.co/BHYRf8M3"><img src="https://i.ibb.co/35PHTtZd/image2.png" alt="image2" border="0"></a>

#### 4. Propose a Fix
The memory leak occurs because the allocated memory is not freed.So, here modified `program1` function to free the allocated memory:

```c
#include<stdio.h>
#include<stdlib.h>

void program1(int N) {
    int *arr = malloc(N * sizeof(int)); // Allocate memory for N integers
    for(int i = 0; i < N; i++) {
        arr[i] = i * i;
        printf("arr[%d] = %d\n", i, arr[i]);
    }
    free(arr); // Free the allocated memory
}

int main() {
    program1(4); // Should print the array [0, 1, 4, 9]
}
```

#### 5. Recompile and Verify
Recompile the program and run it again with `valgrind`:

```bash
gcc -Wall -Werror -g -std=c99 -o program1 -O0 program1.c
valgrind --leak-check=full ./program1
```

**Expected Valgrind Output:**
```
==22038== Memcheck, a memory error detector
==22038== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==22038== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==22038== Command: ./program1
==22038== 
arr[0] = 0
arr[1] = 1
arr[2] = 4
arr[3] = 9
==22038== 
==22038== HEAP SUMMARY:
==22038==     in use at exit: 0 bytes in 0 blocks
==22038==   total heap usage: 2 allocs, 2 frees, 1,040 bytes allocated
==22038== 
==22038== All heap blocks were freed -- no leaks are possible
==22038== 
==22038== For lists of detected and suppressed errors, rerun with: -s
==22038== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
- The memory leak has been fixed. Valgrind no longer reports any memory-related issues.

<a href="https://ibb.co/q3pnpsbh"><img src="https://i.ibb.co/jPD5DM7q/image3.png" alt="image3" border="0"></a>

### Task 2

---

### **Program 2**

#### Code:
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
    program2(4); // Should print the array [1, 2, 4, 8]
}
```

#### Steps

1. **Run the Program with Valgrind:**
    ```bash
    valgrind --leak-check=full ./program2
    ```
    **Valgrind Output:**
    ```
    ==33396== Memcheck, a memory error detector
    ==33396== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
    ==33396== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
    ==33396== Command: ./program2
    ==33396== 
    ==33396== Invalid read of size 4
    ==33396==    at 0x10927E: program2 (program2.c:18)
    ==33396==    by 0x1092BA: main (program2.c:23)
    ==33396==  Address 0x4a85040 is 0 bytes inside a block of size 16 free'd
    ==33396==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==33396==    by 0x10920A: work (program2.c:9)
    ==33396==    by 0x109260: program2 (program2.c:16)
    ==33396==    by 0x1092BA: main (program2.c:23)
    ==33396==  Block was alloc'd at
    ==33396==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==33396==    by 0x10922B: program2 (program2.c:13)
    ==33396==    by 0x1092BA: main (program2.c:23)
    ==33396== 
    arr[0] = 1
    arr[1] = 2
    arr[2] = 4
    arr[3] = 8
    ==33396== 
    ==33396== HEAP SUMMARY:
    ==33396==     in use at exit: 0 bytes in 0 blocks
    ==33396==   total heap usage: 2 allocs, 2 frees, 1,040 bytes allocated
    ==33396== 
    ==33396== All heap blocks were freed -- no leaks are possible
    ==33396== 
    ==33396== For lists of detected and suppressed errors, rerun with: -s
    ==33396== ERROR SUMMARY: 4 errors from 1 contexts (suppressed: 0 from 0)

   ```

<a href="https://ibb.co/RpXrLkBn"><img src="https://i.ibb.co/qMHqb3WG/image4.png" alt="image4" border="0"></a>

2. **Analysis:**
   - The program has a **use-after-free** issue. The `work` function frees the memory allocated for `arr`, but `program2` tries to access it afterward.
   - This is related to **CWE-416: Use After Free**.

3. **Fix:**
   Move the `free(arr)` call to the end of `program2` after the array is printed.

   **Fixed Code:**
   ```c
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>

    void work(int* arr, unsigned N) {
        for(int i=1; i<N; i++) {
            arr[i] = arr[i-1] * 2;
        }
    }

    void program2(unsigned N) {
        int* arr = (int*)malloc(N * sizeof(*arr));
        if (arr == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }
        memset(arr, 0, N * sizeof(*arr)); // Initialize the entire array
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
   ```

4. **Verify the Fix:**
   Recompile and run with Valgrind:
   ```bash
   gcc -Wall -Werror -g -std=c99 -o program2 -O0 program2.c
   valgrind --leak-check=full ./program2
   ```
   **Valgrind Output:**
   ```
   ==36594== Memcheck, a memory error detector
   ==36594== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
   ==36594== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
   ==36594== Command: ./program2
   ==36594== 
   arr[0] = 1
   arr[1] = 2
   arr[2] = 4
   arr[3] = 8
   ==36594== 
   ==36594== HEAP SUMMARY:
   ==36594==     in use at exit: 0 bytes in 0 blocks
   ==36594==   total heap usage: 2 allocs, 2 frees, 1,040 bytes allocated
   ==36594== 
   ==36594== All heap blocks were freed -- no leaks are possible
   ==36594== 
   ==36594== For lists of detected and suppressed errors, rerun with: -s
   ==36594== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
   ```

<a href="https://ibb.co/67Qb0qnm"><img src="https://i.ibb.co/Kpn7x80X/image5.png" alt="image5" border="0"></a>

---

### **Program 3**

#### Code:
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

#### Steps:

1. **Run the Program with Valgrind:**
   ```bash
   valgrind --leak-check=full ./program3
   ```
   **Valgrind Output:**
   ```
   ==45799== Memcheck, a memory error detector
   ==45799== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
   ==45799== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
   ==45799== Command: ./program3
   ==45799== 
   Memory allocation success!
   ==45799== 
   ==45799== HEAP SUMMARY:
   ==45799==     in use at exit: 4 bytes in 1 blocks
   ==45799==   total heap usage: 2 allocs, 1 frees, 1,028 bytes allocated
   ==45799== 
   ==45799== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1
   ==45799==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
   ==45799==    by 0x1091A2: program3 (program3.c:6)
   ==45799==    by 0x1091FC: main (program3.c:16)
   ==45799== 
   ==45799== LEAK SUMMARY:
   ==45799==    definitely lost: 4 bytes in 1 blocks
   ==45799==    indirectly lost: 0 bytes in 0 blocks
   ==45799==      possibly lost: 0 bytes in 0 blocks
   ==45799==    still reachable: 0 bytes in 0 blocks
   ==45799==         suppressed: 0 bytes in 0 blocks
   ==45799== 
   ==45799== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
   ```

<a href="https://ibb.co/HpTcSrdL"><img src="https://i.ibb.co/vv4NFJYC/image6.png" alt="image6" border="0"></a>

2. **Analysis:**
   - The program has a logical error in the condition `(arr = NULL)`. It should be `(arr == NULL)` to check if `malloc` failed.
   - This is related to **CWE-480: Use of Incorrect Operator**.

3. **Fix:**
   

   **Fixed Code:**
   ```c
   #include<stdio.h>
   #include<stdlib.h>
   #include<string.h> 
   
   void* program3(unsigned N) {
    void *arr = malloc(N * sizeof(*arr));
      if((N < 1) || (arr == NULL)) {
        printf("%s\n", "Memory allocation failed!");
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

4. **Verify the Fix:**
   Recompile and run with Valgrind:
   ```bash
   gcc -Wall -Werror -g -std=c99 -o program3 -O0 program3.c
   valgrind --leak-check=full ./program3
   ```
   **Valgrind Output:**
   ```
   ==46254== Memcheck, a memory error detector
   ==46254== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
   ==46254== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
   ==46254== Command: ./program3
   ==46254== 
   Memory allocation success!
   ==46254== 
   ==46254== HEAP SUMMARY:
   ==46254==     in use at exit: 0 bytes in 0 blocks
   ==46254==   total heap usage: 2 allocs, 2 frees, 1,028 bytes allocated
   ==46254== 
   ==46254== All heap blocks were freed -- no leaks are possible
   ==46254== 
   ==46254== For lists of detected and suppressed errors, rerun with: -s
   ==46254== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
   ```

<a href="https://ibb.co/7t9F0RTB"><img src="https://i.ibb.co/23bBpsLR/image7.png" alt="image7" border="0"></a>

---

### **Program 4**

#### Code:
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

#### Steps:

2. **Run the Program with Valgrind:**
   ```bash
   valgrind --leak-check=full ./program4
   ```
   **Valgrind Output:**
   ```
   ==51048== Memcheck, a memory error detector
   ==51048== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
   ==51048== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
   ==51048== Command: ./program4
   ==51048== 
   ==51048== Conditional jump or move depends on uninitialised value(s)
   ==51048==    at 0x484F229: strlen (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
   ==51048==    by 0x48DADA7: __printf_buffer (vfprintf-process-arg.c:435)
   ==51048==    by 0x48DB73A: __vfprintf_internal (vfprintf-internal.c:1544)
   ==51048==    by 0x48D01B2: printf (printf.c:33)
   ==51048==    by 0x10923C: program4 (program4.c:12)
   ==51048==    by 0x109251: main (program4.c:16)
   ==51048== 
   ==51048== Conditional jump or move depends on uninitialised value(s)
   ==51048==    at 0x484F238: strlen (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
   ==51048==    by 0x48DADA7: __printf_buffer (vfprintf-process-arg.c:435)
   ==51048==    by 0x48DB73A: __vfprintf_internal (vfprintf-internal.c:1544)
   ==51048==    by 0x48D01B2: printf (printf.c:33)
   ==51048==    by 0x10923C: program4 (program4.c:12)
   ==51048==    by 0x109251: main (program4.c:16)
   ==51048== 
   ==51048== Conditional jump or move depends on uninitialised value(s)
   ==51048==    at 0x4902E59: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:783)
   ==51048==    by 0x4905678: _IO_default_xsputn (genops.c:399)
   ==51048==    by 0x4905678: _IO_default_xsputn (genops.c:370)
   ==51048==    by 0x4903AE9: _IO_new_file_xsputn (fileops.c:1265)
   ==51048==    by 0x4903AE9: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
   ==51048==    by 0x48D0CC8: __printf_buffer_flush_to_file (printf_buffer_to_file.c:59)
   ==51048==    by 0x48D0CC8: __printf_buffer_to_file_done (printf_buffer_to_file.c:120)
   ==51048==    by 0x48DB742: __vfprintf_internal (vfprintf-internal.c:1545)
   ==51048==    by 0x48D01B2: printf (printf.c:33)
   ==51048==    by 0x10923C: program4 (program4.c:12)
   ==51048==    by 0x109251: main (program4.c:16)
   ==51048== 
   ==51048== Syscall param write(buf) points to uninitialised byte(s)
   ==51048==    at 0x498C574: write (write.c:26)
   ==51048==    by 0x4903974: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1181)
   ==51048==    by 0x4902570: new_do_write (fileops.c:449)
   ==51048==    by 0x4902570: _IO_do_write@@GLIBC_2.2.5 (fileops.c:426)
   ==51048==    by 0x4902EF2: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:784)
   ==51048==    by 0x4905678: _IO_default_xsputn (genops.c:399)
   ==51048==    by 0x4905678: _IO_default_xsputn (genops.c:370)
   ==51048==    by 0x4903AE9: _IO_new_file_xsputn (fileops.c:1265)
   ==51048==    by 0x4903AE9: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
   ==51048==    by 0x48D0CC8: __printf_buffer_flush_to_file (printf_buffer_to_file.c:59)
   ==51048==    by 0x48D0CC8: __printf_buffer_to_file_done (printf_buffer_to_file.c:120)
   ==51048==    by 0x48DB742: __vfprintf_internal (vfprintf-internal.c:1545)
   ==51048==    by 0x48D01B2: printf (printf.c:33)
   ==51048==    by 0x10923C: program4 (program4.c:12)
   ==51048==    by 0x109251: main (program4.c:16)
   ==51048==  Address 0x4a85048 is 8 bytes inside a block of size 1,024 alloc'd
   ==51048==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
   ==51048==    by 0x48F51B4: _IO_file_doallocate (filedoalloc.c:101)
   ==51048==    by 0x4905523: _IO_doallocbuf (genops.c:347)
   ==51048==    by 0x4902F8F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
   ==51048==    by 0x4903AAE: _IO_new_file_xsputn (fileops.c:1244)
   ==51048==    by 0x4903AAE: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
   ==51048==    by 0x48D0CC8: __printf_buffer_flush_to_file (printf_buffer_to_file.c:59)
   ==51048==    by 0x48D0CC8: __printf_buffer_to_file_done (printf_buffer_to_file.c:120)
   ==51048==    by 0x48DB742: __vfprintf_internal (vfprintf-internal.c:1545)
   ==51048==    by 0x48D01B2: printf (printf.c:33)
   ==51048==    by 0x10923C: program4 (program4.c:12)
   ==51048==    by 0x109251: main (program4.c:16)
   ==51048== 
   String: Hello World!
   ==51048== 
   ==51048== HEAP SUMMARY:
   ==51048==     in use at exit: 0 bytes in 0 blocks
   ==51048==   total heap usage: 1 allocs, 1 frees, 1,024 bytes allocated
   ==51048== 
   ==51048== All heap blocks were freed -- no leaks are possible
   ==51048== 
   ==51048== Use --track-origins=yes to see where uninitialised values come from
   ==51048== For lists of detected and suppressed errors, rerun with: -s
   ==51048== ERROR SUMMARY: 26 errors from 4 contexts (suppressed: 0 from 0)
   ```

<a href="https://ibb.co/jvjV8hxZ"><img src="https://i.ibb.co/gMcvryXF/image8.png" alt="image8" border="0"></a>


<a href="https://ibb.co/1J6gnhPL"><img src="https://i.ibb.co/HpdS21bV/image9.png" alt="image9" border="0"></a>


2. **Analysis:**
   - The program returns a pointer to a local variable (`message`), which goes out of scope after `getString` returns. This leads to **undefined behavior**.
   - This is related to **CWE-562: Return of Stack Variable Address**.

3. **Fix:**
   Allocate memory dynamically for the string or use a static/global variable.

   **Fixed Code:**
   ```c
   #include<stdio.h>
   #include<stdlib.h>
   #include<string.h>
   
   char* getString() {
       char* message = malloc(100 * sizeof(char));
       if (message == NULL) {
           return NULL;
       }
       strcpy(message, "Hello World!");
       return message;
   }
   
   void program4() {
       char* str = getString();
       if (str != NULL) {
           printf("String: %s\n", str);
           free(str);
       }
   }
   
   int main() {
       program4();
   }
   ```

4. **Verify the Fix:**
   Recompile and run with Valgrind:
   ```bash
   gcc -Wall -Werror -g -std=c99 -o program4 -O0 program4.c
   valgrind --leak-check=full ./program4
   ```
   **Valgrind Output:**
   ```
   ==51509== Memcheck, a memory error detector
   ==51509== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
   ==51509== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
   ==51509== Command: ./program4
   ==51509== 
   String: Hello World!
   ==51509== 
   ==51509== HEAP SUMMARY:
   ==51509==     in use at exit: 0 bytes in 0 blocks
   ==51509==   total heap usage: 2 allocs, 2 frees, 1,124 bytes allocated
   ==51509== 
   ==51509== All heap blocks were freed -- no leaks are possible
   ==51509== 
   ==51509== For lists of detected and suppressed errors, rerun with: -s
   ==51509== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
   ```

<a href="https://ibb.co/Vcj7fbfr"><img src="https://i.ibb.co/JwBfD8D0/image10.png" alt="image10" border="0"></a>


---

### Task 3

### **Identified CWEs in the Code**


---

### **1. CWE-457: Use of Uninitialized Variable**
- **Description:** The variable `sum` in the `HashIndex` function was used without being initialized, leading to undefined behavior.
- **Relevant Code:**
  ```c
  int HashIndex(char* key) {
      int sum; // Uninitialized variable
      for (char* c = key; c; c++) {
          sum += *c;
      }
      return sum;
  }
  ```
- **Fix:** Initialize `sum` to `0`.
  ```c
  size_t HashIndex(const char* key) {
      size_t sum = 0; // Initialized to 0
      for (const char* c = key; *c; c++) {
          sum += *c;
      }
      return sum % MAP_MAX;
  }
  ```

---

### **2. CWE-476: NULL Pointer Dereference**
- **Description:** The code did not check if `map` or `value` was `NULL` before accessing their members, which could lead to a NULL pointer dereference.
- **Relevant Code:**
  ```c
  void HashAdd(HashMap *map, PairValue *value) {
      int idx = HashIndex(value->KeyName);
      if (map->data[idx]) // No NULL check for map or value
          value->Next = map->data[idx]->Next;
      map->data[idx] = value;	
  }
  ```
- **Fix:** Add NULL checks for `map` and `value`.
  ```c
  void HashAdd(HashMap *map, PairValue *value) {
      if (!map || !value) return; // NULL check
      size_t idx = HashIndex(value->KeyName);
      if (map->data[idx])
          value->Next = map->data[idx]->Next;
      map->data[idx] = value;	
  }
  ```

---

### **3. CWE-120: Buffer Overflow**
- **Description:** The loop condition in `HashIndex` was incorrect (`for (char* c = key; c; c++)`), which could lead to reading beyond the bounds of the string.
- **Relevant Code:**
  ```c
  int HashIndex(char* key) {
      int sum = 0;
      for (char* c = key; c; c++) { // Incorrect loop condition
          sum += *c;
      }
      return sum;
  }
  ```
- **Fix:** Change the loop condition to check `*c` instead of `c`.
  ```c
  size_t HashIndex(const char* key) {
      size_t sum = 0;
      for (const char* c = key; *c; c++) { // Check *c instead of c
          sum += *c;
      }
      return sum % MAP_MAX;
  }
  ```

---

### **4. CWE-415: Double Free**
- **Description:** The `HashDelete` function attempted to free memory that was not dynamically allocated (stack-allocated `PairValue` objects), leading to a "double free or corruption" error.
- **Relevant Code:**
  ```c
  void HashDelete(HashMap *map, const char* key) {
      unsigned idx = HashIndex(key);
      for (PairValue* val = map->data[idx], *prev = NULL; val != NULL; prev = val, val = val->Next) {
          if (strcmp(val->KeyName, key)) {
              if (prev)
                  prev->Next = val->Next;
              else
                  map->data[idx] = val->Next;
              free(val); // Attempting to free stack-allocated memory
          }
      }
  }
  ```
- **Fix:** Remove the `free(val)` call and ensure that only dynamically allocated memory is freed.
  ```c
  void HashDelete(HashMap *map, const char* key) {
      if (!map || !key) return; // NULL check
      size_t idx = HashIndex(key);
      PairValue* val = map->data[idx];
      PairValue* prev = NULL;
      while (val) {
          if (strcmp(val->KeyName, key) == 0) {
              if (prev)
                  prev->Next = val->Next;
              else
                  map->data[idx] = val->Next;
              // Do not free val here, as it may be stack-allocated
              return;
          }
          prev = val;
          val = val->Next;
      }
  }
  ```

---
### **Fianl code**
```c
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// Returns the index where the key should be stored
size_t HashIndex(const char* key) {
    size_t sum = 0;
    for (const char* c = key; *c; c++) {
        sum += *c;
    }
    return sum % MAP_MAX; // Ensure index is within bounds
}

// Allocates memory for the HashMap
HashMap* HashInit() {
    HashMap* map = malloc(sizeof(HashMap));
    if (!map) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // Initialize all pointers in the data array to NULL
    for (size_t i = 0; i < MAP_MAX; i++) {
        map->data[i] = NULL;
    }
    return map;
}

// Inserts PairValue into the map, if the value exists, increase ValueCount
void HashAdd(HashMap *map, PairValue *value) {
    if (!map || !value) return; // NULL check
    size_t idx = HashIndex(value->KeyName);
    if (map->data[idx])
        value->Next = map->data[idx]->Next;
    map->data[idx] = value;	
}

// Returns PairValue from the map if a given key is found
PairValue* HashFind(HashMap *map, const char* key) {
    if (!map || !key) return NULL; // NULL check
    size_t idx = HashIndex(key);
    for (PairValue* val = map->data[idx]; val != NULL; val = val->Next) {
        if (strcmp(val->KeyName, key) == 0) // Compare with == 0
            return val;
    }
    return NULL; 
}

// Deletes the entry with the given key from the map
void HashDelete(HashMap *map, const char* key) {
    if (!map || !key) return; // NULL check
    size_t idx = HashIndex(key);
    PairValue* val = map->data[idx];
    PairValue* prev = NULL;
    while (val) {
        if (strcmp(val->KeyName, key) == 0) { // Compare with == 0
            if (prev)
                prev->Next = val->Next;
            else
                map->data[idx] = val->Next;
            return;
        }
        prev = val;
        val = val->Next;
    }
}

// Prints all content of the map
void HashDump(HashMap *map) {
    if (!map) return; // NULL check
    for (size_t i = 0; i < MAP_MAX; i++) {
        for (PairValue* val = map->data[i]; val != NULL; val = val->Next) {
            printf("%s\n", val->KeyName); // Safe format string
        }
    }
}

int main() {
    HashMap* map = HashInit();
    printf("%s\n", "HashInit() Successful");
    
    // Dynamically allocate PairValue objects
    PairValue* pv1 = malloc(sizeof(PairValue));
    PairValue* pv2 = malloc(sizeof(PairValue));
    if (!pv1 || !pv2) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(pv1->KeyName, "test_key", KEY_STRING_MAX);
    pv1->ValueCount = 1;
    pv1->Next = NULL;
    strncpy(pv2->KeyName, "other_key", KEY_STRING_MAX);
    pv2->ValueCount = 1;
    pv2->Next = NULL;
    
    printf("HashAdd(map, '%s')\n", pv1->KeyName);
    HashAdd(map, pv1);

    printf("HashAdd(map, '%s')\n", pv1->KeyName);
    HashAdd(map, pv1);

    printf("HashAdd(map, '%s')\n", pv2->KeyName);
    HashAdd(map, pv2);

    printf("HashFind(map, %s) = ", pv1->KeyName);
    PairValue* result = HashFind(map, pv1->KeyName);
    if (result) {
        printf("{'%s': %d}\n", result->KeyName, result->ValueCount);
    }
    else {
        printf("%s\n", "Not found");
    }
    
    printf("%s", "HashDump(map) = ");
    HashDump(map);

    printf("HashDelete(map, '%s')\n", pv1->KeyName);
    HashDelete(map, pv1->KeyName);

    printf("HashFind(map, %s) = ", pv1->KeyName);
    result = HashFind(map, pv1->KeyName); 
    if (result) {
        printf("{'%s': %d}\n", result->KeyName, result->ValueCount);
    }
    else {
        printf("%s\n", "Not found");
    }

    printf("%s", "HashDump(map) = ");
    HashDump(map);

    // Free dynamically allocated memory
    free(pv1);
    free(pv2);
    free(map);
}
```
---

### **Verification**


```bash
valgrind --leak-check=full ./hash1
```

**Valgrind Output:**
```
==64608== Memcheck, a memory error detector
==64608== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==64608== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==64608== Command: ./hash1
==64608== 
HashInit() Successful
HashAdd(map, 'test_key')
HashAdd(map, 'test_key')
HashAdd(map, 'other_key')
HashFind(map, test_key) = {'test_key': 1}
HashDump(map) = other_key
test_key
HashDelete(map, 'test_key')
HashFind(map, test_key) = Not found
HashDump(map) = other_key
==64608== 
==64608== HEAP SUMMARY:
==64608==     in use at exit: 0 bytes in 0 blocks
==64608==   total heap usage: 4 allocs, 4 frees, 2,592 bytes allocated
==64608== 
==64608== All heap blocks were freed -- no leaks are possible
==64608== 
==64608== For lists of detected and suppressed errors, rerun with: -s
==64608== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

<a href="https://ibb.co/FGB6dxZ"><img src="https://i.ibb.co/L4CSskj/image11.png" alt="image11" border="0"></a>




