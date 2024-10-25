#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

namespace udp_lib{

class UdpConnect{
    /*
    UDP送受信を行うクラス
    */
    int sock;
    struct sockaddr_in addr;
    
    public:
        UdpConnect(std::string address, int port); // UDPコンストラクタ       
        
        void udp_send(double value); // UDP送信関数

        void udp_bind(); 

        double udp_recv();

        void udp_recv(char *buf, int size); // UDP受信関数
        
        ~UdpConnect();
};

}