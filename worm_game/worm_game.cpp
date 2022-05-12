#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>
#include <winsock2.h>
//
#include <iostream>


SOCKET AcceptPlayer(SOCKET listen_socket, SOCKADDR_IN addr)
{
    int size_of_addr = sizeof(addr);
    SOCKET сonnection = accept(listen_socket, (SOCKADDR*)&addr, &size_of_addr);
    char nickname[4];
    recv(сonnection, nickname, 4, MSG_PEEK);
    std::cout << nickname << " joined the game\n"<< char(7);
    return сonnection;
}

void SendMessageToHosts(int host_num, SOCKET* сonnections, char* msg, int msg_size, int msg_sender = -1)
{
    for (int i = 0; i < host_num; i++)
    {
        if (i == msg_sender)
            continue;
        send(сonnections[i], msg, msg_size, NULL);
    }
}

char** ReceiveNicknames(SOCKET* сonnections, int players_num)
{
    char** nicknames_in_char = new char* [players_num];
    for (int i = 0; i < players_num; i++)
    {
        nicknames_in_char[i] = new char[4];
        recv(сonnections[i], nicknames_in_char[i], 4, NULL);
    }
    return nicknames_in_char;
}

void StartGame()
{
    std::cout << "Game will start in: ";
    for (int i = 5; i > 0; i--)
    {
        std::cout << i;
        Sleep(500);
        std::cout << ".";
        Sleep(250);
        std::cout << ".";
        Sleep(250);
        std::cout << " ";
    }
    std::cout << "\nThe game started\n";
}

int main()
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 2); 
    if (WSAStartup(DLLVersion, &wsaData) != 0)  
    {
        std::cout << "Error" << std::endl;
        exit(1);
    }

    in_addr ip_to_num;
    inet_pton(AF_INET, "127.0.0.1", &ip_to_num); 

    SOCKADDR_IN addr; 
    addr.sin_addr = ip_to_num; 
    addr.sin_port = htons(1111); 
    addr.sin_family = AF_INET; 

    SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, NULL);
    bind(listen_socket, (SOCKADDR*)&addr, sizeof(addr));
    listen(listen_socket, 8);

    int players_num;
    std::cout << "Enter number of players: ";
    std::cin >> players_num;
    std::cout << "Server started...\n";

    SOCKET* сonnections = new SOCKET[players_num];
    for (int i = 0; i < players_num; i++)
        сonnections[i] = AcceptPlayer(listen_socket, addr);

    closesocket(listen_socket);
    std::cout << "All players joined.\n";

    char** nicknames_in_char = ReceiveNicknames(сonnections, players_num);

    SendMessageToHosts(players_num, сonnections, (char*)&players_num, sizeof(int));
    for (int i = 0; i < players_num; i++)
    {
        send(сonnections[i], (char*)&i, sizeof(int), NULL);
    }
    for (int i = 0; i < players_num; i++)
        SendMessageToHosts(players_num, сonnections, nicknames_in_char[i], sizeof(nicknames_in_char[i]));


    StartGame();
    enum class PackeType { YOUTURN, PLAYERMOVEMENTS, RANDOMSEED, GAMEOVER };


    int random_seed = time(0);
    PackeType packet_type = PackeType::RANDOMSEED;
    SendMessageToHosts(players_num, сonnections, (char*)&packet_type, sizeof(packet_type));
    SendMessageToHosts(players_num, сonnections, (char*)&random_seed, sizeof(int));

    int destroyed_worms_num = 0;
    bool* worm_is_destroyed = new bool[players_num]{};
    while (destroyed_worms_num < players_num)
    {
        for (int i = 0; i < players_num; i++)
        {
            if (worm_is_destroyed[i])
                continue;
            packet_type = PackeType::YOUTURN;
            send(сonnections[i], (char*)&packet_type, sizeof(packet_type), NULL);
            char move;
            recv(сonnections[i], &move, sizeof(move), NULL);
            if (move == -1)
            {
                worm_is_destroyed[i] = true;
                destroyed_worms_num++;
                continue;
            }
            packet_type = PackeType::PLAYERMOVEMENTS;
            SendMessageToHosts(players_num, сonnections, (char*)&packet_type, sizeof(packet_type), i);
            SendMessageToHosts(players_num, сonnections, (char*)&i, sizeof(int), i);
            SendMessageToHosts(players_num, сonnections, &move, sizeof(move), i);

        }
    }
    std::cout << "Game over.\n";
    packet_type = PackeType::GAMEOVER;
    SendMessageToHosts(players_num, сonnections, (char*)&packet_type, sizeof(packet_type));
    system("pause");
    for (int i = 0; i < players_num; i++)
    {
        closesocket(сonnections[i]);
        delete nicknames_in_char[i];
    }
    delete[] сonnections;
    delete[] nicknames_in_char;
    delete worm_is_destroyed;
    WSACleanup();
}