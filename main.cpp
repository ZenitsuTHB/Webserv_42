#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080
#define INDEX_FILE "www/index.html"

std::string readFile(const std::string &filename) {
    std::ifstream file(filename.c_str());  // Corregido para C++98
    if (!file) {
        return "<h1>Error 404: Archivo no encontrado</h1>";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Crear socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Error al crear socket\n";
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Enlazar socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Error al enlazar socket\n";
        return 1;
    }

    // Escuchar conexiones
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Error al escuchar\n";
        return 1;
    }

    std::cout << "Servidor escuchando en el puerto " << PORT << "...\n";

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            std::cerr << "Error al aceptar conexión\n";
            continue;
        }

        read(new_socket, buffer, 1024);
        std::cout << "Solicitud recibida:\n" << buffer << "\n";

        // Leer el archivo index.html
        std::string content = readFile(INDEX_FILE);

        // Convertir el tamaño a string con stringstream (en vez de std::to_string)
        std::stringstream ss;
        ss << content.length();
        std::string content_length = ss.str();

        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " +
                               content_length + "\r\n\r\n" + content;

        send(new_socket, response.c_str(), response.length(), 0);
        close(new_socket);
    }

    return 0;
}

