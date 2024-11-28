#ifndef UDP_CONNECT_HPP
#define UDP_CONNECT_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>  // EXIT_FAILURE, exitのため
#include <cstdio>   // perrorのため
#include <iostream>
#include <vector>
#include <iterator>
#include <chrono>

namespace udp_lib{

class UdpConnect{
    /*
    UDP送受信を行うクラス
    */
    int sock;
    struct sockaddr_in addr;
    size_t buffer_size;
    size_t total_buffer_size;
    char* buffer;

    public:

        UdpConnect(std::string address, int port); // UDPコンストラクタ       
        
        void udp_send(const int values , std::chrono::nanoseconds nano_system_clock); // UDP送信関数

        void udp_bind(); 

        int udp_recv();

        
        ~UdpConnect();
};

}

#endif // UDP_CONNECT_HPP  // 3. ここでガードを閉じます