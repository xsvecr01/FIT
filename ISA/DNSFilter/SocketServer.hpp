/**
 * @file SocketServer.hpp
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 * @date 2020-11-17
 * 
 * @brief Hlavickovy soubor
 * 
 */
#pragma once

#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <mutex>
#include <thread>
#include <err.h>
#include "FileParser.hpp"
#include "SocketClient.hpp"

/**
 * @brief Trida pro serverovou cast programu
 * 
 */
class SocketServer
{
    std::mutex mtx;
    bool startNew = true;
    int waiting = 0;

public:
    
    /**
     * @brief Defaultni konstruktor
     * 
     */
    SocketServer();

    /**
     * @brief Nahrazuje konstruktor, inicializuje a binduje socket
     * 
     * @param port Port, na kterem program pobezi
     * @param server DNS server, na ktery se odesilaji pozadavky
     * @param file Soubor se zakazanyma domenama
     */
    int Init(int port, std::string server, std::string file);

    /**
     * @brief Kontrolni funkce pro spousteni novych threadu
     * 
     * @return true Pokud se ma vytvorit novy thread
     * @return false Pokude se novy thread vytvaret nema
     */
    bool shouldStart();

    /**
     * @brief Funkce pro kontrolu aktualne cekajicich threadu
     * 
     * @return int Pocet threadu, ktere cekaji na request
     */
    int Waiting();

    /**
     * @brief Hlavni funkce, zpracovava, kontroluje a pripadne preposila requesty na DNS server. Nasledne klientovi vrati odpovidajici response.
     * 
     */
    void ProcessRequest();

    /**
     * @brief Blokujici funkce, ktera ceka na request
     * 
     * @param buffer Buffer paketu
     * @param size Velikost bufferu
     * @param cli_addr Struktura obsahujici adresu klienta
     * @param len Velikost struktury adresy klienta
     * @return int Vraci pocet prectenych bytu
     */
    int GetRequest(char* buffer, int size, struct sockaddr_in* cli_addr, socklen_t* len);

    /**
     * @brief Funkce odesilajici odpoved klientovi
     * 
     * @param buffer Buffer paketu
     * @param size Velikost bufferu
     * @param cli_addr Struktura obsahujici adresu klienta
     * @param len Velikost struktury adresy klienta
     * @return int Vraci pocet odeslanych bytu
     */
    int SendResponse(char* buffer, int size, struct sockaddr_in* cli_addr, socklen_t len);

private:

    /**
     * @brief Blok privatnich promennych
     * 
     */
    int _port;
    std::string _server;
    struct sockaddr_in _serv_addr;
    int _sFD;
    std::string _file;
    FileParser _fileParser;
    int _active = 0;
    int _busy = 0;

    /**
     * @brief Funkce modifikujici navratovy kod requestu
     * 
     * @param buffer Zpracovavany paket
     * @param rcode Navratovy kod requestu, 4 pro NOTIMP, 5 pro REFUSED
     */
    void ModifyRcode(char* buffer, int rcode);

    /**
     * @brief Parsuje domenu z obsahu paketu
     * 
     * @param buffer Zpracovavany paket
     * @param typeA Vystupni parametru, ma hodnotu true, pokud je dotaz typu A, jinak ma hodnotu false
     * @return std::string Adresa domeny
     */
    std::string ParseDomain(char* buffer, bool* typeA);

    /**
     * @brief Pomocna funkce pro prevod binarnich cisel na dekadicka
     * 
     * @param bin Vektor reprezentujici binarni cilso
     * @return int Dekadicka hodnota binarniho cisla
     */
    int Bin2Dec(std::vector<int> bin);

    /**
     * @brief Pomocna funkce prevadejici dekadicka cisla na binarni
     * 
     * @param num Cislo v dekadickem tvaru
     * @return std::vector<int> Vektor reprezentujici prevedene cislo v binarnim tvaru
     */
    std::vector<int> Dec2Bin(int num);

};