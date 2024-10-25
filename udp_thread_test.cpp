#include "udp_connect.hpp"
#include <iostream>
#include <chrono>
#include <thread>


int server_1(){
    /*
    server用のコード
    */

    try {
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection("100.91.239.110", 4000);

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

int receive_1(){
    try {
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection("0.0.0.0", 8080);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる

        // バインド（サーバーとして動作するために必要）
        udpConnection.udp_bind();

        // データ受信を無限ループで行う
        while (true) {
            double receivedData = udpConnection.udp_recv();
            std::cout << "Data received: " << receivedData << std::endl;

            // 少し待機して次の受信へ（必要であれば待機時間を調整可能）
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(){
    std::thread th1(server_1);
    std::thread th2(receive_1);

    th1.join();
    th2.join();
    return 0;
}