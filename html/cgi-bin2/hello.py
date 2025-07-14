#!/usr/bin/env python3

print("Content-Type: text/html\n")  # Encabezado obligatorio para CGI

print("""
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>CGI en Python</title>
</head>
<body>
    <h1>Hola desde un script CGI en Python!</h1>
</body>
</html>
""")

