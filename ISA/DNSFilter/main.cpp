/**
 * @file main.cpp
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 * @date 2020-11-17
 * 
 * @brief Hlavni soubor obsahujici funkci main
 * 
 */

#include <iostream>
#include <unistd.h>
#include <thread>
#include <future>
#include <mutex>
#include <signal.h>
#include "FileParser.hpp"
#include "SocketServer.hpp"


#define BUFFER 4096

bool loop = true;
std::mutex t_mtx;
std::vector<std::thread> threads;

/**
 * @brief Funkce pro parsovani parametru programu
 * 
 * @param argc Pocet parametru
 * @param argv Pole parametru
 * @param server_out Vystupni parametr pro server
 * @param port_out Vystupni parametr pro port
 * @param file_out Vystupni parametr pro nazev souboru
 * @return int Vraci 0 kdyz jsou parametry programu zadany spravne, jinak vraci 1
 */
int parseArgs(int argc, char** argv, std::string* server_out, int* port_out, std::string* file_out)
{
    int opt;
    bool s_flag = false;
    bool f_flag = false;

    while((opt = getopt(argc, argv, "s:p:f:")) != -1)
    {
        switch(opt)
        {
            case 's':
                s_flag = true;
                *server_out = optarg;
                break;
            case 'p':
                *port_out = atoi(optarg);
                break;
            case 'f':
                f_flag = true;
                *file_out = optarg;
                break;
            case '?':
                return 1;
            default:
                return 1;
        }
    }

    if(!s_flag && !f_flag)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief Funkce pro odchyceni CTRL + C
 * 
 * @param s Dummy parametr
 */
void sigHandler(int s)
{
    loop = false;
}

/**
 * @brief Funkce pro odebirani threadu z vectoru
 * 
 * @param id Identifikator odebiraneho threadu
 */
void removeThread(std::thread::id id)
{
    std::lock_guard<std::mutex> lock(t_mtx);
    if(!threads.empty())
    {
        for(unsigned long int i = 0; i < threads.size(); i++)
        {
            if(threads[i].get_id() == id)
            {
                threads[i].detach();
                threads[i].~thread();
                threads.erase(threads.begin() + i);
            }
        }
    }
}

/**
 * @brief Hlavni funkce main, stara se o spousteni a beh programu
 * 
 * @param argc Pocet vstupnich parametru z prikazove radky
 * @param argv Pole vstupnich parametru
 * @return int Navratovy kod programu. Vraci 0, kdyz beh probehl v poradku
 */
int main(int argc, char **argv) 
{
    std::string server;
    int port = 53;
    std::string file;

    signal(SIGINT, sigHandler);

    if(parseArgs(argc, argv, &server, &port, &file) != 0)
    {
        std::cout << "./dns -s server [-p port] -f filter_file" << std::endl << std::endl;
        std::cout << "-s: IP adresa nebo doménové jméno DNS serveru (resolveru), kam se má zaslat dotaz." << std::endl;
        std::cout << "-p port: Číslo portu, na kterém bude program očekávat dotazy. Výchozí je port 53." << std::endl;
        std::cout << "-f filter_file: Jméno souboru obsahující nežádoucí domény." << std::endl;
        return 1;
    }

    FileParser domains = FileParser(file);

    if(domains.isEmpty()){
        std::cerr << "filter_file error." << std::endl;
        return 2;
    }

    SocketServer sockServer;
    sockServer.Init(port, server, file);

    while(loop)
    {
        if(sockServer.shouldStart() && sockServer.Waiting() == 0)
        {
            std::lock_guard<std::mutex> lock(t_mtx);
            threads.push_back(std::thread([&sockServer]{
                sockServer.ProcessRequest();
                std::thread(removeThread, std::this_thread::get_id()).detach();
            }));
        }
        std::this_thread::sleep_for(std::chrono::microseconds(5));
    }

    for(auto& t : threads)
    {
        if(t.joinable())
        {
            t.detach();
            t.~thread();
        }
    }

    return 0;
}