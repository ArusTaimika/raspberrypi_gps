
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
        std::string csv_filename = "output_file/PCA.csv";
        csv_lib::Csvedit csvWriter(csv_filename);
        csvWriter.csv_write_headers({"NaN","RT","PMRx", "PMRx", "AMRx","VgMRx", "VgMRy", "WgMR", 
                                     "PCR1x", "PCR1y", "ACR1", "VgCR1x", "VgCR1y", "WgCR1",
                                     "PCR2x", "PCR2y", "ACR2", "VgCR2x", "VgCR2y", "WgCR2"});
        // csvデータの型定義  
        std::pair<std::vector<int64_t>, std::vector<double>> csv_data;

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
            //std::cout << "roop_count : " << nano_receive_clock.count() << std::endl;
            //csv出力
            csv_data = {{0,nano_receive_clock.count()}, receivedData.first};
            // データをペア型にして書き込み
            csvWriter.csv_write_data(csv_data);
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int receive_test_from_BBB_1(std::pair<std::string,int> monitored_pc,char my_location){
    try {
        /*
         UDP通信初期化
        */
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection_receive("0.0.0.0", 65000, 26);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる
        udp_lib::UdpConnect udpConnection_send_monitore(monitored_pc.first, monitored_pc.second, 7);
        // バインド（サーバーとして動作するために必要）
        udpConnection_receive.udp_bind();
        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;
        
        // CSVファイルの初期化
        std::string csv_filename = "output_file/CRB.csv" /*+ std::string(1, my_location)*/;
        csv_lib::Csvedit csvWriter(csv_filename);
        csvWriter.csv_write_headers({"TT","RT","TD","Perrx","Perry","Aerr", // send time , receive time, delay time, position error x, position error y, angle error
                                     "Vcx","Vcy","Wc",
                                     "Ww1","Ww2","Ww3",
                                     "Vm1","Vm2","Vm3",
                                     "FEactM","FEactA",
                                     "Force_Virtual_Mag","Force_Virtual_Angle",
                                     "Force_Ideal_Mag","Force_Ideal_Angle"});
        // csvデータの型定義  
        std::pair<std::vector<int64_t>,std::vector<double>>  csv_data;
         // 送信データの定義
        std::vector<double> send_data{0,0,0,0,0,0,0};
        std::vector<double> merged_data;
        double delay_time = 0.0; // 遅延時間の初期化
        // データ受信を無限ループで行う
        while (true) {
            // UDP受信
            std::pair<std::vector<double>, int64_t> receivedData = udpConnection_receive.udp_recv();// pairはfirst, secondで抽出可能
            
            // 現在時刻取得v
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());
            delay_time = (nano_receive_clock.count() - receivedData.second) / 1000000.0; // ミリ秒単位に変換
            send_data.clear();
            send_data.insert(send_data.end(), receivedData.first.begin()+12, receivedData.first.begin()+14);
            send_data.insert(send_data.end(), receivedData.first.begin()+17, receivedData.first.begin()+19);
            send_data.insert(send_data.end(), receivedData.first.begin()+23, receivedData.first.begin()+25);
            send_data.push_back(delay_time); 
            udpConnection_send_monitore.udp_send(send_data, nano_receive_clock.count());
            // 出力
            std::cout << "delay_time : " <<send_data[5]  << std::endl;
            //csv出力
            merged_data.clear();
            merged_data.push_back(delay_time);
            merged_data.insert(merged_data.end(), receivedData.first.begin(), receivedData.first.begin()+14);
            merged_data.insert(merged_data.end(), receivedData.first.begin()+18, receivedData.first.begin()+19);
            merged_data.insert(merged_data.end(), receivedData.first.begin()+24, receivedData.first.begin()+25);
            csv_data = {{receivedData.second, nano_receive_clock.count()},merged_data};
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

    std::thread th1(receive, selectlocation.send_selected_ips, selectlocation.send_selected_port, selectlocation.my_location);
    std::thread th2(receive_test_from_BBB_1,selectlocation.monitored_pc,selectlocation.my_location); // 実際に処理に使用する際の遅延
    th1.join();
    th2.join();

    return 0;
}