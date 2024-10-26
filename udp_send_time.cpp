#include "udp_connect.hpp"
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

        // データ送信を無限ループで行う
        while (true) {
            //現在時刻取得
            std::chrono::high_resolution_clock::time_point server_clock = std::chrono::high_resolution_clock::now();
            std::chrono::nanoseconds nano_system_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(server_clock.time_since_epoch());
            // UDP送信
            udpConnection.udp_send(dataToSend, nano_system_clock); // std::chrono::nanoseconds　はint64_tのサイズ数
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
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection("0.0.0.0", 4000, 3);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる

        // バインド（サーバーとして動作するために必要）
        udpConnection.udp_bind();

        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;
        std::chrono::nanoseconds nano_server_clock;
        std::chrono::nanoseconds delay_time;
        
        // データ受信を無限ループで行う
        while (true) {
            // UDP受信
            std::pair<std::vector<double>, std::chrono::nanoseconds> receivedData = udpConnection.udp_recv();// pairはfirst, secondで抽出可能
            nano_server_clock = receivedData.second;
            // 現在時刻取得
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());
            
            // delay計算
            delay_time = nano_receive_clock - nano_server_clock;
            
            // 出力
            std::cout << "delay time: " <<  delay_time.count() << std::endl;

            //csv出力
            
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
    //std::thread th2(receive_1);

    th1.join();
    //th2.join();
    return 0;
}