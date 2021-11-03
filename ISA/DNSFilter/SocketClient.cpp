/**
 * @file SocketClient.hpp
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 * @date 2020-11-17
 * 
 * @brief Implementacni soubor
 * 
 */

#include "SocketClient.hpp"


SocketClient::SocketClient(std::string server)
{
    auto tmp = gethostbyname(server.c_str());
    std::string tmpServer = inet_ntoa( *( struct in_addr*)(tmp->h_addr_list[0]));

    _server_addr.sin_addr.s_addr = inet_addr(tmpServer.c_str());
    _server_addr.sin_port = htons(53);

    _server_addr.sin_family = AF_INET;

    _sFD = socket(AF_INET, SOCK_DGRAM, 0);
    if(_sFD == -1)
        err(3, "socket()");
}


void SocketClient::Close()
{
    close(_sFD);
}


int SocketClient::SendRequest(char* buffer, int buffer_size, int msg_size)
{
    socklen_t len = sizeof(_server_addr);
    int rc = sendto(_sFD, buffer, msg_size, 0, (struct sockaddr *) &_server_addr, len);
    if(rc == -1)
        err(3, "sendto()");

    len = sizeof(_from);
    rc = getsockname(_sFD, (struct sockaddr *) &_from, &len);
    if(rc == -1)
        err(3, "getsockname()");
    
    len = sizeof(_from);
    rc = recvfrom(_sFD, buffer, buffer_size, 0, (struct sockaddr *) &_from, &len);
    if(rc == -1)
        err(3, "recvfrom()");

    return rc;
}