# Fuzzing Report

## Author: Muhibullo Khujabekov (CS-02)

## Task 1 - WebApp Fuzzing

### 1. Accessible Endpoints/Files from big.txt

**Command Used:**
```bash
ffuf -u http://localhost/FUZZ -w /path/to/big.txt -fc 404
```
**Explanation:**
This command:
- Tests each word in big.txt as a path (`/FUZZ`) against the DVWA instance
- Filters out 404 responses (`-fc 404`) to only show accessible or interesting endpoints
- Uses the big.txt wordlist from SecLists

**Findings:**
- **Accessible files:**
  - `favicon.ico` (200 OK)
  - `robots.txt` (200 OK)

- **Interesting responses (non-404):**
  - `.htaccess` (403 Forbidden)
  - `.htpasswd` (403 Forbidden)
  - `config` (301 Redirect)
  - `docs` (301 Redirect)
  - `external` (301 Redirect)
  - `server-status` (403 Forbidden)

<a href="https://ibb.co/XZwRTwcw"><img src="https://i.ibb.co/wNvm1v5v/image1.png" alt="image1" border="0"></a>

### 2. Available File Extensions for Index Page

**Command Used:**
```bash
ffuf -u http://localhost/indexFUZZ -w /path/to/web-extensions.txt
```

**Explanation:**
This command:
- Tests different file extensions appended to "index"
- Uses the web-extensions.txt wordlist
- Shows which extensions return valid responses for the index page

**Findings:**
- `index.php` (302 Redirect) - This is the main entry point
- `index.phps` (403 Forbidden) - Indicates the server blocks access to PHP source files

<a href="https://ibb.co/27B2znkR"><img src="https://i.ibb.co/tM5f0mJn/image2.png" alt="image2" border="0"></a>

<a href="https://ibb.co/F9X4g0z"><img src="https://i.ibb.co/shC9y6q/image3.png" alt="image3" border="0"></a>

### 3. Accessible Directories from raft-medium-directories.txt

**Command Used:**
```bash
ffuf -u http://localhost/FUZZ -w /path/to/raft-medium-directories.txt -fc 404
```

**Explanation:**
This command:
- Tests each directory name from raft-medium-directories.txt
- Filters out 404 responses (`-fc 404`) to only show accessible or interesting endpoints
- Helps identify directory structure

**Findings:**
- **Accessible directories:**
  - `config` (301 Redirect)
  - `docs` (301 Redirect)
  - `external` (301 Redirect)
  
- **Interesting response:**
  - `server-status` (403 Forbidden)

<a href="https://ibb.co/chJwbkML"><img src="https://i.ibb.co/why7JB3s/image4.png" alt="image4" border="0"></a>

## Task 2 - Python Fuzzing

### Fuzzer Statistics (fuzzer_stats):

<a href="https://ibb.co/Qydhth4"><img src="https://i.ibb.co/T9LJdJs/image5.png" alt="image5" border="0"></a>

### Crash Reproduction
**Crash Case (malformed % encoding):**
```python
print(uridecode("%GG"))  # Invalid hex digits
```
**Error:**
```
ValueError: invalid literal for int() with base 16: 'G'
```

**Hang Case (incomplete % encoding):**
```python
print(uridecode("%"))    # Missing hex digits
```
Function enters infinite loop as it keeps checking for non-existent hex digits

<a href="https://ibb.co/5h1M8yQ2"><img src="https://i.ibb.co/F4hxwMQn/image6.png" alt="image6" border="0"></a>

## Identified Problems and Fixes

### Problems:
1. **Crash Cause:** No validation of hex digits after % sign (accepts invalid hex chars)
2. **Hang Cause:** No bounds checking when accessing s[i+1] and s[i+2] (buffer overflow risk)
3. **General Issues:** No error handling for malformed input

### Proposed Fix:
```python
def uridecode(s):
    if not isinstance(s, str):
        raise TypeError("Input must be a string")
     
    ret = []
    i = 0
    while i < len(s):
        if s[i] == '%':
            if i + 2 >= len(s):
                raise ValueError("Incomplete % encoding")
            
            try:
                char_code = int(s[i+1:i+3], 16)
                ret.append(chr(char_code) if char_code <= 127 else '')
                i += 3
            except ValueError:
                raise ValueError(f"Invalid hex digits: {s[i+1:i+3]}")
                
        elif s[i] == '+':
            ret.append(' ')
            i += 1
        else:
            ret.append(s[i])
            i += 1
    return ''.join(ret)
```

## Questions & Answers

### 1. Will the fuzzer ever terminate?
No, AFL++ is designed to run continuously until manually stopped. It will keep generating new test cases to explore more code paths and find deeper bugs. The fuzzer only stops if it hits a user-defined timeout or is manually interrupted.

### 2. How coverage-guided fuzzers work? Is AFL coverage-guided?
Coverage-guided fuzzers:
1. Instrument the target program to track code coverage
2. Generate test cases that exercise new code paths
3. Prioritize inputs that increase coverage
4. Mutate interesting cases to find more paths

Yes, AFL is coverage-guided. It uses lightweight instrumentation to track basic block transitions and favors inputs that discover new execution paths.

### 3. How to optimize a fuzzing campaign?
Key optimization strategies:
1. **Quality Seed Corpus:** Start with diverse, valid inputs
2. **Dictionary:** Provide known interesting tokens/patterns
3. **Parallelization:** Run multiple fuzzer instances with -M/-S flags
4. **CPU Affinity:** Pin instances to specific cores
5. **Memory Limits:** Prevent OOM crashes with -m none
6. **Timeouts:** Set reasonable timeout values
7. **Target Optimization:** Minimize startup overhead in the target
8. **Corpus Minimization:** Use afl-cmin to reduce input set
9. **Persistent Mode:** For fast targets, reuse process state



---

**After some fixes:**

<a href="https://ibb.co/vb0M2W9"><img src="https://i.ibb.co/hk3TpN0/image7.png" alt="image7" border="0"></a>

