#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

class UdpConnect {
public:
    UdpConnect(const char* ip, int port, int dataPieces)
        : ip(ip), port(port), dataPieces(dataPieces) {
        // UDPソケットを作成
        udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (udpSocket < 0) {
            std::cerr << "ソケット作成エラー" << std::endl;
            exit(EXIT_FAILURE);
        }

        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);

        // サーバーモードのときはバインド
        if (strcmp(ip, "0.0.0.0") == 0) {
            if (bind(udpSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                std::cerr << "バインドエラー" << std::endl;
                close(udpSocket);
                exit(EXIT_FAILURE);
            }
        }
    }

    ~UdpConnect() {
        close(udpSocket);
    }

    void udpServer() {
        sockaddr_in clientAddr = {};
        socklen_t addrLen = sizeof(clientAddr);
        char buffer[16]; // バッファサイズ16バイト

        // データ受信
        int receivedBytes = recvfrom(udpSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (receivedBytes > 0) {
            double* data = reinterpret_cast<double*>(buffer);
            for (int i = 0; i < dataPieces; i++) {
                std::cout << "Received: " << data[i] << std::endl;
            }
        } else {
            std::cerr << "受信エラー" << std::endl;
        }
    }

private:
    int udpSocket;
    const char* ip;
    int port;
    int dataPieces;
};

int main() {
    const char* ip = "0.0.0.0";
    int port = 12345;
    int dataPieces = 2;

    UdpConnect udpConnect(ip, port, dataPieces);

    try {
        while (true) {
            udpConnect.udpServer();
        }
    } catch (...) {
        std::cerr << "エラーが発生しました。" << std::endl;
    }

    return 0;
}
