/**
 * @file SocketClient.hpp
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 * @date 2020-11-17
 * 
 * @brief Hlavickovy soubor
 * 
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <err.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

/**
 * @brief Trida pro klienta
 * 
 */
class SocketClient
{
public:
    /**
     * @brief Konstruktor pro klient soket
     * 
     * @param server DNS server, na ktery se budou preposilat pozadavky
     */
    SocketClient(std::string server);

    /**
     * @brief Uzavreni socket file descriptoru
     * 
     */
    void Close();

    /**
     * @brief Odesila pozadavek na DNS server a zaroven prijima odpoved
     * 
     * @param buffer Buffer, kde je ulozen pozadavek, nasledne se tam ulozi odpoved
     * @param buffer_size Velikost bufferu
     * @param msg_size Velikost zpravy
     * 
     * @return int Vraci velikost odpovedi
     */
    int SendRequest(char* buffer, int buffer_size, int msg_size);

private:
    /**
     * @brief Privatni promenne
     * 
     */
    int _sFD;
    std::string _server;
    struct sockaddr_in _server_addr, _from;
    socklen_t len;

};