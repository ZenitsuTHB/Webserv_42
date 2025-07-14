#!/usr/bin/env python3
import os
import sys

print("Content-Type: text/plain\r\n")

data = sys.stdin.read(int(os.environ.get("CONTENT_LENGTH", 0)))

#upload_dir = os.environ.get("UPLOAD_PATH", "../uploads")

upload_dir = os.environ.get("UPLOAD_PATH")

if not upload_dir:
    scritp_dir = os.path.dirname(os.path.abspath(__file__))
    upload_dir = os.path.join(script_dir, "uploads")

os.makedirs(upload_dir, exist_ok=True)

filename = os.path.join(upload_dir, "uploaded_file.txt")
with open(filename, "w") as f:
    f.write(data)

print(f"File uploaded successfully\nSaved to: {filename}")
