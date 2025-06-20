import afl
import sys

def uridecode(s):
    if not isinstance(s, str):
        raise TypeError("Input must be a string")
     
    ret = []
    i = 0
    while i < len(s):
        if s[i] == '%':
            # Check if we have enough characters left
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
    
if __name__ == '__main__':
    afl.init()
    print(uridecode(sys.stdin.read()))