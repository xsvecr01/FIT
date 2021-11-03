/**
 * @file SocketServer.cpp
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 * @date 2020-11-17
 * 
 * @brief Implementacni soubor
 * 
 */
#include "SocketServer.hpp"

SocketServer::SocketServer(){}

int SocketServer::Init(int port, std::string server, std::string file)
{
    _port = port;
    _server = server;
    _file = file;

    _fileParser = FileParser(_file);
    if(_fileParser.isEmpty())
    {
        return 12;
    }

    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_addr.s_addr = INADDR_ANY;
    _serv_addr.sin_port = htons(_port);

    if((_sFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        err(4, "socket()");
    if (bind(_sFD, (struct sockaddr *) &_serv_addr, sizeof(_serv_addr)) == -1) 
        err(4, "bind()");
    return 0;
}


bool SocketServer::shouldStart()
{
    std::lock_guard<std::mutex> lock(mtx);
    return startNew;
}


int SocketServer::Waiting()
{
    std::lock_guard<std::mutex> lock(mtx);
    return waiting;
}


void SocketServer::ProcessRequest()
{
    mtx.lock();
    if(startNew)
    {
        startNew = false;
        waiting++;
    }
    else
    {
        mtx.unlock();
        return;
    }
    mtx.unlock();
    

    int BUFFER = 1024;
    char buffer[BUFFER];
    struct sockaddr_in cli_addr;
    socklen_t len = sizeof(cli_addr);
    
    int msg_size = GetRequest(buffer, BUFFER, &cli_addr, &len);
    mtx.lock();
    startNew = true;
    waiting--;
    mtx.unlock();

    if(buffer[0] == 0 && buffer[1] == 0)
    {
        SendResponse(buffer, msg_size, &cli_addr, len);
        return;
    }

    bool isTypeA;
    std::string domain = ParseDomain(buffer, &isTypeA);

    if(!isTypeA)
    {
        ModifyRcode(buffer, 4);
    }
    else if(_fileParser.isBlacklisted(domain))
    {
        ModifyRcode(buffer, 5);
    }
    else
    {
        SocketClient client = SocketClient(_server);
        msg_size = client.SendRequest(buffer, BUFFER, msg_size);
        client.Close();
    }
    SendResponse(buffer, msg_size, &cli_addr, len);
    return;
}


int SocketServer::GetRequest(char* buffer, int size, struct sockaddr_in* cli_addr, socklen_t* len)
{
    int msg_size = recvfrom(_sFD, buffer, size, 0, ( struct sockaddr *) cli_addr, len);
    if(msg_size == -1)
        err(4, "recvfrom()");
    return msg_size;
}


int SocketServer::SendResponse(char* buffer, int size, struct sockaddr_in* cli_addr, socklen_t len)
{
    int msg_size = sendto(_sFD, buffer, size, 0, (struct sockaddr *) cli_addr, len);        
    if(msg_size == -1)
        err(4, "sendto()");
    return msg_size;
}

void SocketServer::ModifyRcode(char* buffer, int rcode)
{
    auto bin = Dec2Bin((int)buffer[2]);
    bin.back() = 0;
    bin.front() = 1;
    buffer[2] = Bin2Dec(bin);

    bin = Dec2Bin((int)buffer[3]);
    
    for(int i = 0; i < 4; i++)
    {
        if(!bin.empty())
            bin.pop_back();
    }
    if(rcode == 4)
        bin.insert(bin.end(), {0,1,0,0});
    if(rcode == 5)
        bin.insert(bin.end(), {0,1,0,1});

    buffer[3] = Bin2Dec(bin);
}


std::string SocketServer::ParseDomain(char* buffer, bool* typeA)
{
    int pos = 12;
    std::string tmp_domain = "";
    for(int cnt = (int)buffer[pos]; cnt != 0; cnt = (int)buffer[pos])
    {
        if(pos != 12)
        {
            tmp_domain.push_back('.');
        }
        pos++;
        for(int i = pos; i < pos+cnt; i++)
        {
            tmp_domain.push_back(buffer[i]);
        }
        pos += cnt;
    }
    if(buffer[pos+1] == 0 && buffer[pos+2] == 1)
        *typeA = true;
    else
        *typeA = false;
    return tmp_domain;
}


int SocketServer::Bin2Dec(std::vector<int> bin)
{
    int res = 0;
    for(int i = 1; !bin.empty(); i *= 2)
    {
        res += i * bin.back();
        bin.pop_back();
    }
    return res;
}


std::vector<int> SocketServer::Dec2Bin(int num)
{
    std::vector<int> arr;
    
    int i = 0;
    while(num > 0)
    {
        arr.push_back(num % 2);
        num = num / 2;
        i++;
    }
    while(i < 8)
    {
        arr.push_back(0);
        i++;
    }

    std::vector<int> res;

    while(!arr.empty())
    {
        res.push_back(arr.back());
        arr.pop_back();
    }
    return res;
}