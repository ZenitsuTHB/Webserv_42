#!/usr/bin/env python3
import os
import urllib.parse
import html

print("Content-Type: text/html\n")

print("<html><body>")
print("<h1>CGI GET Python</h1>")

query_string = os.environ.get("QUERY_STRING", "")
print(f"<p><strong>Query:</strong> {html.escape(query_string)}</p>")

# Aquí parseamos con urllib.parse.parse_qs
params = urllib.parse.parse_qs(query_string)

print("<ul>")
for key in params:
    print(f"<li>{html.escape(key)}: {html.escape(params[key][0])}</li>")
print("</ul>")

print("</body></html>")

