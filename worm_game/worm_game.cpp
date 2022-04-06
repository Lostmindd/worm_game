#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>
#include <winsock2.h>
//
#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>
#include "game_score.h"
#include "apples.h"
#include "worm.h"
#include "move_x_y.h"
#include "constants.h"

const std::string kWormColors[8] = { "1;1","2;1", "3;1", "4;1", "5;1", "6;1", "3", "1" }; 
const int kFieldSizeByX = 119;
const int kFieldSizeByY = 30;

HANDLE std_out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
void MoveXY(int x, int y)
{
    COORD coord = { x, y };
    SetConsoleCursorPosition(std_out_handle, coord);
}
//////////////////////

SOCKET AcceptPlayer(SOCKET listen_socket, SOCKADDR_IN addr)
{
    int size_of_addr = sizeof(addr);
    SOCKET сonnection = accept(listen_socket, (SOCKADDR*)&addr, &size_of_addr);
    char nickname[4];
    recv(сonnection, nickname, 4, MSG_PEEK);
    std::cout << nickname << " joined the game\n";
    return сonnection;
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

    SOCKADDR_IN addr; //хранит адрес
    addr.sin_addr = ip_to_num; //ip
    addr.sin_port = htons(1111); //порт
    addr.sin_family = AF_INET; //семейство протоколов (const)

    SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, NULL);
    bind(listen_socket, (SOCKADDR*)&addr, sizeof(addr));
    listen(listen_socket, 8);

    int players_num;
    std::cout << "Enter number of players: ";
    std::cin >> players_num;
    system("cls");
    std::cout << "Server started...\n";

    SOCKET* сonnections = new SOCKET[players_num];
    for (int i = 0; i < players_num; i++)
    {
        сonnections[i] = AcceptPlayer(listen_socket, addr);
    }

    closesocket(listen_socket);

    std::string* nicknames = new std::string[players_num];// убрать
    char* nicknames_in_char = new char[4 * players_num];
    for (int i = 0, j = 0; i < players_num; i++)
    {
        char nickname[4];
        recv(сonnections[i], nickname, 4, NULL);
        for (int k = 0; k < 4; k++)
        {
            nicknames_in_char[j] = nickname[k];
            j++;
        }
        nicknames[i] = std::string(nickname);// убрать
    }

    for (int i = 0; i < players_num; i++)
    {
        send(сonnections[i], (char*)&players_num, sizeof(int), NULL);
        send(сonnections[i], nicknames_in_char, players_num *4, NULL);
    }

    //




 
    

//	WORD dll_version = MAKEWORD(2, 2); //запрашиваем версию библиотеки
//	if (WSAStartup(dll_version, &wsa_data) != 0)  //библиотека загрузилась удачно (проверка)
//	{
//		std::cout << "Error" << std::endl;
//		exit(1);
//	}




    /*
    std::string names[8] = {"vas","dgh","dgh" ,"dgh" ,"dgh" ,"dgh" ,"dgh" ,"dgh" };
    GameScore score = { 8, names };
    system("mode con cols=160 lines=32");

    auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO struct_cursor_info;
    GetConsoleCursorInfo(handle, &struct_cursor_info);
    struct_cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &struct_cursor_info);

        int current_score_line = 3 + 2 * score.GetPlayerNum();
        std::cout << "\x1b[30;1m";
        for (int i = 0; i < kFieldSizeByX + 4; i++)
        {
            std::cout << char(219);
        }
        std::cout << "\x1b[0m";
        std::cout << "\n";
        for (int i = 0; i < kFieldSizeByY; i++)
        {
            std::cout << "\x1b[30;1m";
            std::cout << char(219) << char(221) << std::setw(kFieldSizeByX + 1) << char(222) << char(219);
            std::cout << "\x1b[0m";
            score.TableCreate(&current_score_line);
            std::cout << "\n";
        }
        std::cout << "\x1b[30;1m";
        for (int i = 0; i < kFieldSizeByX + 4; i++)
        {
            std::cout << char(219);
        }
        std::cout << "\x1b[0m";
        Apples apples;
        apples.CreateApples(5);
        Worm worm = {&apples,&score,0 };
        Worm worm2 = {&apples,&score,3 };
        worm.WormShowOnScreen();
        worm2.WormShowOnScreen();
        while (!worm.IsDestroyed())
        {
            worm.WormMove();
        }
        while (!worm2.IsDestroyed())
        {
            worm2.WormMove();
        }
        score.GameOver();*/
    system("pause");
    for (int i = 0; i < players_num; i++)
    {
        closesocket(сonnections[i]);
    }
    delete[] сonnections;
    delete[] nicknames;
    WSACleanup();
}