import socket
import time

HOST = "127.0.0.1"
PORT = 8080
NUM_REQUESTS = 100

request_template = (
    "GET / HTTP/1.1\r\n"
    "Host: localhost\r\n"
    "Connection: keep-alive\r\n"
    "\r\n"
)

def main():
    with socket.create_connection((HOST, PORT)) as sock:
        sock.settimeout(2)  # Pour éviter blocages si plus de réponse

        for i in range(NUM_REQUESTS):
            print(f"[{i+1}] Envoi requête...")
            sock.sendall(request_template.encode())

            response = b""
            try:
                while True:
                    chunk = sock.recv(4096)
                    if not chunk:
                        break
                    response += chunk
                    if b"</html>" in response or b"404" in response:
                        break
            except socket.timeout:
                print(f"[{i+1}] TIMEOUT réception")
            print(f"[{i+1}] Réponse reçue, {len(response)} octets")
            time.sleep(0.01)  # petite pause pour simuler un client humain
            print("[CLIENT] Requêtes terminées. Fermeture du socket.")

if __name__ == "__main__":
    main()

