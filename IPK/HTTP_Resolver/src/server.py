#IPK projekt1 - HTTP resolver domenovych jmen
#autor: Radek Svec (xsvecr01@stud.fit.vutbr.cz)

import socket
import re
import sys

e200 = "HTTP/1.1 200 OK\r\n\r\n"
e400 = "HTTP/1.1 400 Bad Request\r\n\r\n"
e404 = "HTTP/1.1 404 Not Found\r\n\r\n"
e405 = "HTTP/1.1 405 Method Not Allowed\r\n\r\n"
e500 = "HTTP/1.1 500 Internal Server Error\r\n\r\n"

# funkce pro kontrolu a prelozeni vstupu
# xlist - vstupni data, ktera maji byt zkontrolovana a prelozena
# method - metoda POST nebo GET
def resolf(xlist, method):
    response = e200
    for i in xlist:
        url = re.sub(r"\s*", "", i[0])
        
        if(i[1] == "A" and not re.findall("([0-9]{1,3}.){3}[0-9]{1,3}", url)):
            try:
                if(response == e400 or response == e404):
                    response = e200
                addr = socket.gethostbyname(url)
                response += (url + ":" + i[1] + "=" + addr + "\n")
            except:
                if (method == "GET" or response == e200):
                    response = e404
            
        elif(i[1] == "PTR" and re.findall("([0-9]{1,3}.){3}[0-9]{1,3}", url)):
            try:
                if(response == e400 or response == e404):
                    response = e200
                addr = socket.gethostbyaddr(url)[0]
                response += (url + ":" + i[1] + "=" + addr + "\n")
            except:
                if (method == "GET" or response == e200):
                    response = e404

        elif(i[1] == '' or (method == "GET" or response == e200 or response == e404)):
            response = e400
    return(response)

# kontrola argumentu
if not (1024 <= int(sys.argv[1]) <= 65535 and len(sys.argv) == 2):
    print("Error 1: Port must be in 1024 - 65635\n")
    exit(1)

HOST, PORT = '', int(sys.argv[1])

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(5)

while True:
    clientsocket, address = s.accept()
    request_data = clientsocket.recv(1024)
    request_data = request_data.decode('utf-8')

    try:
        get = re.findall("GET ", request_data)
        post = re.findall("POST ", request_data)

        if(not (get or post)):
            server_resp = e405

        else:
            get = re.findall(" /resolve\\?name=(.*)\\&type=(A|PTR) ", request_data)
            post = re.findall(" /dns-query ", request_data)
            if(not (get or post)):
                server_resp = e400
            #========================== G E T ============
            if(get):
                server_resp = resolf(get, "GET")

            #========================== P O S T ===========
            x = re.findall(r"(.*)\s*:\s*([A-z]*)", request_data)
            if(post):
                server_resp = resolf(x, "POST")
    except:
        server_resp = e500

    clientsocket.sendall(bytes(server_resp, "utf-8"))
    clientsocket.close()