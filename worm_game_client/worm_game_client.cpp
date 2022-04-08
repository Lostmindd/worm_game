#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>
#include <winsock2.h>
#include <iostream>
#include <string>
#include <iomanip>

#include "game_score.h"
#include "move_x_y.h"
#include "constants.h"
#include "worm.h"
const std::string kWormColors[8] = { "1;1","2;1", "3;1", "4;1", "5;1", "6;1", "3", "1" };
const int kFieldSizeByX = 119;
const int kFieldSizeByY = 30;

HANDLE std_out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
void MoveXY(int x, int y)
{
    COORD coord = { x, y };
    SetConsoleCursorPosition(std_out_handle, coord);
}

void CreateGameField(GameScore &score)
{
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
}

std::string* ReceiveNicknames(SOCKET connection, int players_num)
{
    std::string* nicknames = new std::string[players_num];
    char* nickname_in_char = new char[4];
    for (int i = 0; i < players_num; i++)
    {
        recv(connection, nickname_in_char, players_num * 4, NULL);
        for (int j = 0; j < 4; j++)
        {
            nicknames[i].push_back(nickname_in_char[j]);
        }
    }
    delete[] nickname_in_char;
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
    system("cls");
    GameScore score = { players_num, nicknames };
    CreateGameField(score);
    Apples apples;
    Worm* worms = new Worm[players_num];
    for (int i = 0; i < players_num; i++)
    {
        worms[i] = Worm{ &apples,&score,i };
        worms[i].WormShowOnScreen();
    }
    enum PackeType { YOUTURN, PLAYERSMOVEMENTS, WORMDESTROYED, RANDOMSEED, GAMEOVER };



    system("pause");
    delete[] nicknames;
    delete[] worms;
}

