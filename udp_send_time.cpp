#include "udp_connect.hpp"
#include "csv_edit.hpp"
#include "gpio_pulse.hpp"

#include <iostream>
#include <chrono>
#include <thread>


int server_1(){
    /*
    server用のコード
    */

    try {
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection("100.116.206.21", 4000);

        // 送信するデータ
        int dataToSend = 0;

        // データ送信を無限ループで行う
        while (true) {
            //現在時刻取得
            std::chrono::high_resolution_clock::time_point server_clock = std::chrono::high_resolution_clock::now();
            std::chrono::nanoseconds nano_system_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(server_clock.time_since_epoch());
            // UDP送信
            udpConnection.udp_send(dataToSend, nano_system_clock); // std::chrono::nanoseconds　はint64_tのサイズ数

            // 出力
            std::cout << "Data sent: " ;
            std::cout << dataToSend;
            std::cout << std::endl;

            // 少し待機してから次を送信（例えば1秒間隔）
            std::this_thread::sleep_for(std::chrono::seconds(1));
            dataToSend = dataToSend + 1;
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int receive_1(){
    try {
        /*
         UDP通信初期化
        */
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection("0.0.0.0", 4000);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる
        // バインド（サーバーとして動作するために必要）
        udpConnection.udp_bind();
        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;
        
        /*
         CSVクラス初期化
        */
        // 出力するcsvファイルを指定して，インスタンスを作成
        csv_lib::Csvedit csvWriter("compare_time.csv");
        // csv_ヘッダを設定
        csvWriter.csv_write_headers({"count", "client_time"});
        // csvデータの型定義
        std::pair<int, int>  send_data;

        // データ受信を無限ループで行う
        while (true) {
            // UDP受信
            int receivedData = udpConnection.udp_recv();// pairはfirst, secondで抽出可能

            // 現在時刻取得
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());    
            std::cout << "Data sent: " ;
            std::cout << receivedData;
            std::cout << std::endl;
            //csv出力
            send_data = std::make_pair(receivedData, nano_receive_clock.count());
            // データをペア型にして書き込み
            csvWriter.csv_write_data(send_data);

        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(){
    //std::thread th1(server_1);
    std::thread th2(receive_1);

    //th1.join();
    th2.join();
    return 0;
}