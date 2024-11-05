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
        udp_lib::UdpConnect udpConnection("100.116.206.21", 4000, 3);

        // 送信するデータ
        std::vector<double> dataToSend = {20, 10, 0};

        /*
         パルス波形を出力初期化
        */
        GpioPulseGenerator pulseGenerator("gpiochip0", 16, 250, 500);

        // データ送信を無限ループで行う
        while (true) {
            //現在時刻取得
            std::chrono::high_resolution_clock::time_point server_clock = std::chrono::high_resolution_clock::now();
            std::chrono::nanoseconds nano_system_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(server_clock.time_since_epoch());
            // UDP送信
            udpConnection.udp_send(dataToSend, nano_system_clock); // std::chrono::nanoseconds　はint64_tのサイズ数
            
            //パルス波形を出力
            pulseGenerator.start();
            // 出力
            std::cout << "Data sent: " ;
            std::copy(dataToSend.begin(), dataToSend.end(), std::ostream_iterator<double>(std::cout, " "));
            std::cout << std::endl;

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
        /*
         UDP通信初期化
        */
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection("0.0.0.0", 4000, 3);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる
        // バインド（サーバーとして動作するために必要）
        udpConnection.udp_bind();
        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;
        std::chrono::nanoseconds nano_server_clock;
        std::chrono::nanoseconds delay_time;
        
        /*
         CSVクラス初期化
        */
        // 出力するcsvファイルを指定して，インスタンスを作成
        csv_lib::Csvedit csvWriter("compare_time.csv");
        // csv_ヘッダを設定
        csvWriter.csv_write_headers({"server_time", "client_time", "delay_time"});
        // 送信データの型定義
        std::vector<std::chrono::nanoseconds>  send_data;        

        /*
         パルス波形を出力初期化
        */
        GpioPulseGenerator pulseGenerator("gpiochip0", 16, 250, 500);
        // データ受信を無限ループで行う
        while (true) {
            // UDP受信
            std::pair<std::vector<double>, std::chrono::nanoseconds> receivedData = udpConnection.udp_recv();// pairはfirst, secondで抽出可能
            nano_server_clock = receivedData.second;
            // 現在時刻取得
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());
            
            //パルス波形を出力
            pulseGenerator.start();
            
            // delay計算
            delay_time = nano_receive_clock - nano_server_clock;            

            // 出力
            std::cout << "delay time: " <<  delay_time.count() << std::endl;
            //csv出力
            send_data = {nano_server_clock, nano_receive_clock, delay_time};
            // データをペア型にして書き込み
            csvWriter.csv_write_data(send_data);

            // 少し待機して次の受信へ（必要であれば待機時間を調整可能）
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(){
    std::thread th1(server_1);
    //std::thread th2(receive_1);

    th1.join();
    //th2.join();
    return 0;
}