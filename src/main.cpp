
#include <iostream>
#include <chrono>
#include <thread>
#include "udp_connect.hpp"
#include "select_location.hpp"


int receive(int port, std::vector<std::string> send_selected_ips, std::vector<int> send_selected_port){
    try {
        /*
         UDP通信初期化
        */
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection_receive("0.0.0.0", 60000, 6);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる
        //udp_lib::UdpConnect udpConnection_send_monitor(send_selected_ips[1], send_selected_port[1], 0);
        udp_lib::UdpConnect udpConnection_send_copy(send_selected_ips[0], send_selected_port[0], 6);
        // バインド（サーバーとして動作するために必要）
        udpConnection_receive.udp_bind();
        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;
        

        // データ受信を無限ループで行う
        while (true) {
            // UDP受信
            std::pair<std::vector<double>, int> receivedData = udpConnection_receive.udp_recv();// pairはfirst, secondで抽出可能
            
            // 現在時刻取得
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());
            udpConnection_send_copy.udp_send(receivedData.first, receivedData.second);

            // 出力
            std::cout << "roop_count : " << receivedData.second << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


int main(int argc, char* argv[]){
    
    //対象locationを渡す
    select_location::SelectLocation selectlocation(argc, argv);
    selectlocation.set_locationip();
    std::cout << "send target location: "<< selectlocation.target_location << std::endl;
    std::cout << "my location: " << selectlocation.my_location << std::endl;
    std::cout << "monitored IPAddress: " << argv[2] << std::endl;
    std::cout << "motitored Port: " << argv[3] << std::endl;

    return 0;
}