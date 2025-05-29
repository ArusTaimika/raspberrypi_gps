
#include <iostream>
#include <chrono>
#include <thread>
#include "udp_connect.hpp"
#include "select_location.hpp"
#include "csv_edit.hpp"

int receive(std::vector<std::string> send_selected_ips, std::vector<int> send_selected_port, char my_location) {
    try {
        /*
         UDP通信初期化
        */
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection_receive("0.0.0.0", 60000, 18);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる
        //udp_lib::UdpConnect udpConnection_send_monitor(send_selected_ips[1], send_selected_port[1], 0);
        udp_lib::UdpConnect udpConnection_send_copy(send_selected_ips[0], send_selected_port[0], 6);
        //udp_lib::UdpConnect udpConnection_send_raspi("100.77.38.204", 63000, 6);
        // バインド（サーバーとして動作するために必要）
        udpConnection_receive.udp_bind();
        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;
        // 送信データの定義
        std::vector<double> send_data{0,0,0,0,0,0};

        // CSVファイルの初期化
        std::string csv_filename = "Location_" + std::string(1, my_location) + "info"+".csv";
        csv_lib::Csvedit csvWriter(csv_filename);
        csvWriter.csv_write_headers({"Time[ns]", "Master_Px[m]", "Master_Py[m]", "Master_Pt[rad]","Master_Vx[m/s]", "Master_Vy[m/s]", "Master_Vt[rad/s]", 
                                     "Copy_1_Px[m]", "Copy_1_Py[m]", "Copy_1_Pt[rad]", "Copy_1_Vx[m/s]", "Copy_1_Vy[m/s]", "Copy_1_Vt[rad/s]",
                                     "Copy_2_Px[m]", "Copy_2_Py[m]", "Copy_2_Pt[rad]", "Copy_2_Vx[m/s]", "Copy_2_Vy[m/s]", "Copy_2_Vt[rad/s]",});
        // csvデータの型定義  
        std::pair<std::int64_t, std::vector<double>> csv_data;

        // データ受信を無限ループで行う
        while (true) {
            // UDP受信
            std::pair<std::vector<double>, int64_t> receivedData = udpConnection_receive.udp_recv();// pairはfirst, secondで抽出可能
            
            // 現在時刻取得
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());
            send_data.assign(receivedData.first.begin(), receivedData.first.begin()+6);
            udpConnection_send_copy.udp_send(send_data, nano_receive_clock.count());
            //udpConnection_send_raspi.udp_send(receivedData.first, nano_receive_clock.count());
            // 出力
            std::cout << "roop_count : " << nano_receive_clock.count() << std::endl;
            //csv出力
            csv_data = {nano_receive_clock.count(), receivedData.first};
            // データをペア型にして書き込み
            csvWriter.csv_write_data(csv_data);
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
            //csv_data = {receivedData.second, nano_receive_clock.count()};
            // データをペア型にして書き込み
            //csvWriter.csv_write_data(csv_data);
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

    std::thread th1(receive, selectlocation.send_selected_ips, selectlocation.send_selected_port, selectlocation.my_location);
    //std::thread th2(receive_test_from_BBB_1); // 実際に処理に使用する際の遅延
    th1.join();
    //th2.join();

    return 0;
}