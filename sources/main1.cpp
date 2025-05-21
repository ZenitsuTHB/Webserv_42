#include "../includes/Server.hpp"
#include <vector>
#include <csignal>
#include <iostream>
#include <pthread.h>

static std::vector<Server*> servers;

void signalHandler(int signum) {
    std::cout << "\n[MAIN] Signal received, shutting down all servers...\n";
    for (size_t i = 0; i < servers.size(); ++i) {
        if (servers[i]) servers[i]->stop();
    }
    exit(signum);
}

void* launchServer(void* arg) {
    Server* server = reinterpret_cast<Server*>(arg);
    server->start();
    return NULL;
}

int main() {
    std::vector<std::pair<in_addr_t, in_port_t> > configs;
    configs.push_back(std::make_pair(INADDR_ANY, 8080));
    configs.push_back(std::make_pair(INADDR_ANY, 8081));
    configs.push_back(std::make_pair(INADDR_ANY, 8082));

    std::vector<pthread_t> threads;

    signal(SIGINT, signalHandler);

    for (size_t i = 0; i < configs.size(); ++i) {
        Server* server = new Server(configs[i].first, configs[i].second);
        servers.push_back(server);

        pthread_t tid;
        if (pthread_create(&tid, NULL, &launchServer, server) != 0) {
            std::cerr << "[MAIN] Failed to create thread for server on port " << configs[i].second << std::endl;
            delete server;
            continue;
        }
        threads.push_back(tid);
    }

    for (size_t i = 0; i < threads.size(); ++i) {
        pthread_join(threads[i], NULL);
    }

    std::cout << "[MAIN] All servers shut down.\n";
    return 0;
}
