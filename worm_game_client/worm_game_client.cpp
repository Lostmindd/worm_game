#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>
#include <winsock2.h>
#include <iostream>
#include <string>

std::string* ReceiveNicknames(SOCKET connection, int players_num)
{
    char* nicknames_in_char = new char[players_num * 4];
    recv(connection, nicknames_in_char, players_num * 4, NULL);
    std::string* nicknames = new std::string[players_num];
    for (int i = 0, j = 0; i < players_num; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            nicknames[i].push_back(nicknames_in_char[j]);
            j++;
        }
    }
    delete[] nicknames_in_char;
    return nicknames;
}

int main()
{
    //
    system("mode con cols=160 lines=32");
    auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO struct_cursor_info;
    GetConsoleCursorInfo(handle, &struct_cursor_info);
    struct_cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &struct_cursor_info);
    //

    std::string adress = "127.0.0.1:1111";
    std::cout << ">> enter your nickname(maximum 3 characters) <<\n";
    std::string str;
    str = "asd"; //убрать
    //std::cin >> str;
    std::string name = str.substr(0, 3);
    auto pos = adress.find(":"); //убрать
    /*std::string::size_type pos ;
    do 
    {
        system("cls");
        std::cout << ">> Enter ip and port to connect to the server (ip:port) <<\n";
        std::cin >> adress;
        pos = str.find(":");

        
    } while (pos == std::string::npos);*/
    std::string ip = adress.substr(0, pos); //ip
    std::string port = adress.substr(pos + 1); //port


    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 2);
    if (WSAStartup(DLLVersion, &wsaData) != 0)
    {
        std::cout << "Error" << std::endl;
        exit(1);
    }
    in_addr ip_to_num;
    inet_pton(AF_INET, ip.c_str(), &ip_to_num);

    SOCKADDR_IN addr;
    int size_of_addr = sizeof(addr);
    addr.sin_addr = ip_to_num; 
    addr.sin_port = htons(std::stoi(port));
    addr.sin_family = AF_INET; 
    SOCKET connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
        system("cls");
        std::cout << "Error: failed connect to server.\n";
        system("pause");
        return 1;
    }
    std::cout << "Connected to " << adress << ".\n";

    send(connection, name.c_str(), 4, NULL);

    int players_num = 0;
    recv(connection, (char*)&players_num, 4, NULL);
    std::string* nicknames = ReceiveNicknames(connection, players_num);


    system("pause");
    delete[] nicknames;
}

