
# Lab 4 - Fuzzing  
**Secure System Development - Spring 2025**  

In this lab, we will practice WebApp and binary fuzzing.  

Create a `.md` step-by-step report of the actions you took with screenshots of key results.  

## Task 1 - WebApp Fuzzing  
**Guide:** [ffuf Documentation](https://github.com/ffuf/ffuf#example-usage)  

1. Install `ffuf` or `wfuzz` and `SecLists`.  
2. Run **DVWA** locally with Docker:  
   ```sh
   docker run -d -p 127.0.0.1:80:80 vulnerables/web-dvwa
   ```
3. Fuzz for endpoints using wordlists from `SecLists`. Example with `ffuf`:  
   ```sh
   ffuf -u http://localhost:80/FUZZ -w <WORDLIST>
   ```
4. Answer the following by fuzzing with wordlists from:  
   - `<SECLISTS_DIR>/Discovery/Web-Content`  
   - Filter results appropriately.  

### Questions to Answer:  
1. **Which endpoints/files from `big.txt` were accessible? Which ones gave interesting error codes (not 404)?**  
2. **What file extensions from `web-extensions.txt` are available for the index page?**  
3. **Which directories from `raft-medium-directories.txt` are accessible? Which ones gave interesting error codes (not 404)?**  

### Report Requirements:  
- **Command used** to answer each question.  
- **Text explanation** of what the command does.  
- **Screenshot** from **your terminal** showing the command result.  

---  

## Task 2 - Python Fuzzing  
**Guide:** [AFL++ User Guide](https://afl-1.readthedocs.io/en/latest/user_guide.html)  

Given this intentionally buggy Python code that performs URI decoding:  

```python
import afl
import sys

def uridecode(s):
    ret = []
    i = 0
    while i < len(s):
        # Translate %xx to its corresponding ASCII character
        if s[i] == '%':
            a = s[i + 1]
            b = s[i + 2]
            char_code = (int(a, 16) * 16) + int(b, 16)
            ret.append(chr(char_code))
            i += 3
        
        # Translates '+' into space
        elif s[i] == '+':
            ret.append(' ')
        
        # Leave other characters unchanged
        else:
            ret.append(s[i])
            i += 1
    return ''.join(ret)
    
if __name__ == '__main__':
    afl.init()
    print(uridecode(sys.stdin.read()))
```

### Instructions:  
1. **Install AFL++** (locally or via Docker):  
   ```sh
   docker run --name afl -ti -v .:/src/ aflplusplus/aflplusplus
   pip install python-afl
   ```
2. **Prepare input directory** with test cases (seed corpus).  
3. **Run the fuzzer**:  
   ```sh
   py-afl-fuzz -i input -o output -- /usr/bin/python3 main.py
   ```
4. **Wait** for at least **one crash** and **one hang** detection.  

### Analysis:  
- Show `fuzzer_stats` and **inputs that caused crashes/hangs**.  
- **Reproduce** a crash and a hang case.  
- **Explain** the problems and propose fixes.  

### Questions to Answer (Briefly & Concisely):  
1. **Will the fuzzer ever terminate in the above experiment? Why/Why not?**  
2. **How do coverage-guided fuzzers work? Is AFL coverage-guided?**  
3. **How can you optimize a fuzzing campaign?**  
