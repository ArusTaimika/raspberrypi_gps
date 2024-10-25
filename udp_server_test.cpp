#include "udp_connect.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    /*
    server用のコード
    */

    try {
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection("127.0.0.1", 8080);

        // 送信するデータ
        double dataToSend = 123.456;

        // データ送信を無限ループで行う
        while (true) {
            udpConnection.udp_send(dataToSend);
            std::cout << "Data sent: " << dataToSend << std::endl;

            // 少し待機してから次を送信（例えば1秒間隔）
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
