# Web Application Firewall (WAF) Lab Report

## Task 1 - Blocking SQLi with WAF

### 1. Deploying Juice Shop
```bash
docker run -d -p 3000:3000 bkimminich/juice-shop
```

<a href="https://ibb.co/QvfvqC9D"><img src="https://i.ibb.co/F4K4f06H/image1.png" alt="image1" border="0"></a>

### 2. Demonstrating SQL Injection
**Exploit:**  
Used SQLi in login form with payload: `' OR TRUE--`

<a href="https://ibb.co/bM04TsfX"><img src="https://i.ibb.co/DHJBh9qQ/image3.png" alt="image3" border="0"></a>

### 3. WAF Deployment with ModSecurity
Created `docker-compose.yml`:
```yaml
version: '3'
services:
  juice-shop:
    image: bkimminich/juice-shop
    networks:
      - app-network

  waf:
    image: owasp/modsecurity-crs:nginx
    ports:
      - "8080:8080"
    environment:
      - BACKEND=http://juice-shop:3000 
    networks:
      - app-network

networks:
  app-network:
    driver: bridge
```

### 4. Testing WAF Protection

1. Deploy `bkimminich/juice-shop` and `owasp/modsecurity-crs:nginx` using a `docker-compose.yaml` file

   ```bash
   docker-compose up
   ```

Attempting same SQLi through WAF (port 8080):

<a href="https://ibb.co/7t37MFPf"><img src="https://i.ibb.co/DfB3cnhJ/image5.png" alt="image5" border="0"></a>

---

## Task 2 - Bypassing WAF

### 1. Developing Bypass Technique
Original blocked payload: `' OR TRUE--`  
**Successful bypass:** `'/? or true --`

<a href="https://ibb.co/Y7B3wQ8m"><img src="https://i.ibb.co/1fGLjQK1/image6.png" alt="image6" border="0"></a>

### 2. Creating Custom WAF Rule and modified `docker-compose.yaml`
- Added to `custom-rules.conf`:
    ```bash
    # Block basic SQLi patterns
    SecRule REQUEST_BODY|ARGS|ARGS_NAMES "@rx (?i)(['\"].*?(\/\?|or|true|--))" \
        "id:1005,\
        phase:2,\
        deny,\
        status:403,\
        msg:'SQLi Bypass Attempt',\
        logdata:'Matched Data: %{TX.0} found within %{MATCHED_VAR_NAME}'"

    # Block obfuscated versions
    SecRule REQUEST_BODY|ARGS|ARGS_NAMES "@rx (?i)(['\"][^a-z0-9]*(\/\?|or|true|--))" \
        "id:1006,\
        phase:2,\
        deny,\
        status:403,\
        msg:'Advanced SQLi Bypass Attempt'"
    ```
- Modified `docker-compose.yaml`:

    ```bash
    version: '3'
    services:
    juice-shop:
        image: bkimminich/juice-shop
        networks:
        - app-network

    waf:
        image: owasp/modsecurity-crs:nginx
        ports:
        - "8080:8080"
        volumes:
        - ./custom-rules.conf:/etc/modsecurity.d/owasp-crs/rules/custom-rules.conf:ro
        environment:
        - BACKEND=http://juice-shop:3000
        - PARANOIA=2 

        networks:
        - app-network

    networks:
    app-network:
        driver: bridge
    ```

### 3. Testing Enhanced Protection
After redeploying, bypass attempt is now blocked:

<a href="https://ibb.co/n8PNmF68"><img src="https://i.ibb.co/BKn539cK/image7.png" alt="image7" border="0"></a>


<a href="https://ibb.co/ZzxKf1kR"><img src="https://i.ibb.co/F4zJhqjk/image8.png" alt="image8" border="0"></a>

### 4. Automated Testing with SQLMap
```bash
sqlmap -u "http://localhost:8080/rest/user/login" \   
  --data='{"email":"*","password":"x"}' \
  -p email \
  --headers="Content-Type: application/json" \
  --risk=3 --level=5 \
  --batch \
  --flush-session
```

**Results:**
- 100% of 312 test requests blocked
- 0 successful injections
- Multiple CRS rules triggered

<a href="https://ibb.co/ZpxBPfHz"><img src="https://i.ibb.co/pBdf81Wj/image9.png" alt="image9" border="0"></a>*