
#include <iostream>
#include <chrono>
#include <thread>
#include "udp_connect.hpp"
#include "select_location.hpp"
#include "csv_edit.hpp"

int receive(std::vector<std::string> send_selected_ips, std::vector<int> send_selected_port){
    try {
        /*
         UDP通信初期化
        */
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection_receive("0.0.0.0", 60000, 6);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる
        //udp_lib::UdpConnect udpConnection_send_monitor(send_selected_ips[1], send_selected_port[1], 0);
        udp_lib::UdpConnect udpConnection_send_copy(send_selected_ips[0], send_selected_port[0], 6);
        udp_lib::UdpConnect udpConnection_send_raspi("100.77.38.204", 63000, 6);
        // バインド（サーバーとして動作するために必要）
        udpConnection_receive.udp_bind();
        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;

        // データ受信を無限ループで行う
        while (true) {
            // UDP受信
            std::pair<std::vector<double>, int64_t> receivedData = udpConnection_receive.udp_recv();// pairはfirst, secondで抽出可能
            
            // 現在時刻取得
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());
            udpConnection_send_copy.udp_send(receivedData.first, nano_receive_clock.count());
            udpConnection_send_raspi.udp_send(receivedData.first, nano_receive_clock.count());
            // 出力
            std::cout << "roop_count : " << nano_receive_clock.count() << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int receive_test_from_BBB_1(){
    try {
        /*
         UDP通信初期化
        */
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection_receive("0.0.0.0", 65000, 6);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる
        // バインド（サーバーとして動作するために必要）
        udpConnection_receive.udp_bind();
        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;
        
        // CSVファイルの初期化
        csv_lib::Csvedit csvWriter("from_BBB_1.csv");
        csvWriter.csv_write_headers({"send_time","recive_time"});
        // csvデータの型定義  
        std::vector<int64_t> csv_data;

        // データ受信を無限ループで行う
        while (true) {
            // UDP受信
            std::pair<std::vector<double>, int64_t> receivedData = udpConnection_receive.udp_recv();// pairはfirst, secondで抽出可能
            
            // 現在時刻取得
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());

            // 出力
            //std::cout << "delay_time : " << (nano_receive_clock.count() - receivedData.second) << std::endl;
            //csv出力
            csv_data = {receivedData.second, nano_receive_clock.count()};
            // データをペア型にして書き込み
            csvWriter.csv_write_data(csv_data);
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int receive_test_from_BBB_2(){
    try {
        /*
         UDP通信初期化
        */
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection_receive("0.0.0.0", 64000, 6);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる
        // バインド（サーバーとして動作するために必要）
        udpConnection_receive.udp_bind();
        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;
        
        // CSVファイルの初期化
        csv_lib::Csvedit csvWriter("from_BBB_2.csv");
        csvWriter.csv_write_headers({"send_time","recive_time"});
        // csvデータの型定義  
        std::vector<int64_t> csv_data;

        // データ受信を無限ループで行う
        while (true) {
            // UDP受信
            std::pair<std::vector<double>, int64_t> receivedData = udpConnection_receive.udp_recv();// pairはfirst, secondで抽出可能
            
            // 現在時刻取得
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());

            // 出力
            std::cout << "delay_time : " << ( receivedData.second) << std::endl;
            //csv出力
            csv_data = {receivedData.second, nano_receive_clock.count()};
            // データをペア型にして書き込み
            csvWriter.csv_write_data(csv_data);
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int receive_test_from_raspi(){
    try {
        /*
         UDP通信初期化
        */
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection_receive("0.0.0.0", 63000, 6);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる
        // バインド（サーバーとして動作するために必要）
        udpConnection_receive.udp_bind();
        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;
        
        // CSVファイルの初期化
        csv_lib::Csvedit csvWriter("from_raspi.csv");
        csvWriter.csv_write_headers({"send_time","recive_time"});
        // csvデータの型定義  
        std::vector<int64_t> csv_data;

        // データ受信を無限ループで行う
        while (true) {
            // UDP受信
            std::pair<std::vector<double>, int64_t> receivedData = udpConnection_receive.udp_recv();// pairはfirst, secondで抽出可能
            
            // 現在時刻取得
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());

            // 出力
            std::cout << "delay_time : " << ( receivedData.second) << std::endl;
            //csv出力
            csv_data = {receivedData.second, nano_receive_clock.count()};
            // データをペア型にして書き込み
            csvWriter.csv_write_data(csv_data);
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


int main(int argc, char* argv[]){
    // argc[1]に選択したいlocationを渡す
    // argc[2]に監視用PCのIPアドレスを渡す
    // argc[3]に監視用PCのポート番号を渡す
    // 対象locationを渡す
    select_location::SelectLocation selectlocation(argc, argv);
    selectlocation.set_locationip();
    std::cout << "send target location: "<< selectlocation.target_location << std::endl;
    std::cout << "my location: " << selectlocation.my_location << std::endl;
    std::cout << "copy robot IPAddress: " << selectlocation.target_copy_robot << std::endl;
    std::cout << "monitored IPAddress: " << selectlocation.monitored_pc.first << std::endl;
    std::cout << "motitored Port: " << selectlocation.monitored_pc.second<< std::endl;

    std::thread th1(receive, selectlocation.send_selected_ips, selectlocation.send_selected_port);
    std::thread th2(receive_test_from_BBB_1); // 実際に処理に使用する際の遅延
    std::thread th3(receive_test_from_BBB_2); // udp受信の遅延を確認するためのスレッド
    std::thread th4(receive_test_from_raspi);
    th1.join();
    th2.join();
    th3.join();
    th4.join();

    return 0;
}