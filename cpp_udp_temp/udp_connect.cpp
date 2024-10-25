#include "udp_connect.hpp"

namespace udp_lib {

// コンストラクタ
UdpConnect::UdpConnect(std::string address, int port) {
    /*
    UDP通信する相手のIPアドレスとポート番号が引数
    */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address.c_str());
    addr.sin_port = htons(port);    
}

// UDP送信関数（double型データを送信）
void UdpConnect::udp_send(double value) {
    sendto(sock, &value, sizeof(value), 0, (struct sockaddr *)&addr, sizeof(addr));
}

// UDPバインド関数
void UdpConnect::udp_bind() {
    if (bind(sock, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

// UDP受信関数（double型データを受信）
double UdpConnect::udp_recv() {
    double value;
    recv(sock, &value, sizeof(value), 0);
    return value;
}

// UDP受信関数（バッファにdouble型データを格納）
void UdpConnect::udp_recv(char *buf, int size) {
    recv(sock, buf, size * sizeof(double), 0);  // sizeは要素数と仮定
}

// デストラクタ
UdpConnect::~UdpConnect() {
    close(sock);
}


}
