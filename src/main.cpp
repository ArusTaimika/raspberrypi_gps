
#include <cstdlib>
#include <iostream>
#include <atomic>
#include <csignal>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <vector>
#include "udp_connect.hpp"
#include "commandline.hpp"
#include "cpu_manager.hpp"
#include "csv_edit.hpp"


std::atomic<bool> running{true};

std::string pc_name = "PCB.csv";
std::string raspi_name = "raspiB.csv";

int receive( std::vector<std::string> send_selected_ips) {
    try {
        /*
         UDP通信初期化
        */
        // IPアドレスとポート番号を指定して、UdpConnectインスタンスを作成
        udp_lib::UdpConnect udpConnection_receive("0.0.0.0", 60000, 18);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる
        udp_lib::UdpConnect udpConnection_send_copy(send_selected_ips[0], 40000, 12);
        udp_lib::UdpConnect udpConnection_send_raspi(send_selected_ips[1], 62000, 26);
        // バインド（サーバーとして動作するために必要）
        udpConnection_receive.udp_bind();
        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;
        // 送信データの定義
        std::vector<double> send_data(12, 0.0);
        std::vector<double> send_raspi(26, 0.0); // ダミーデータ
        // CSVファイルの初期化
        std::string csv_filename = "output_file/"+pc_name;//std::string(1, my_location)+
        csv_lib::Csvedit csvWriter(csv_filename);
        csvWriter.csv_write_headers({"NaN","RT","PMRx", "PMRy", "AMRx","VgMRx", "VgMRy", "WgMR", 
                                     "PCR1x", "PCR1y", "ACR1", "VgCR1x", "VgCR1y", "WgCR1",
                                     "PCR2x", "PCR2y", "ACR2", "VgCR2x", "VgCR2y", "WgCR2"});
        // csvデータの型定義  
        std::pair<std::vector<int64_t>, std::vector<double>> csv_data;
        std::cout << "[Info] Start receiving UDP data..." << std::endl;
        // データ受信を無限ループで行う
        while (running) {
            // UDP受信
            std::pair<std::vector<double>, int64_t> receivedData = udpConnection_receive.udp_recv();// pairはfirst, secondで抽出可能
            
            // 現在時刻取得
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());
            send_data.assign(receivedData.first.begin(), receivedData.first.begin()+12);
            udpConnection_send_copy.udp_send(send_data, nano_receive_clock.count());
            //udpConnection_send_raspi.udp_send(send_raspi, nano_receive_clock.count());
            // 出力
            //std::cout << "roop_count : " << nano_receive_clock.count() << std::endl;
            //csv出力
            csv_data = {{0,nano_receive_clock.count()}, receivedData.first};
            // データをペア型にして書き込み
            csvWriter.csv_write_data(csv_data);
        }

    } catch (const std::exception &e) {
        std::cerr << "[Error] " << e.what() << std::endl;
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
        udp_lib::UdpConnect udpConnection_receive("0.0.0.0", 62000, 26);  // "0.0.0.0"はすべてのIPアドレスからの接続を受け入れる
        // バインド（サーバーとして動作するために必要）
        udpConnection_receive.udp_bind();
        // システムクロック定義
        std::chrono::nanoseconds nano_receive_clock;
        
        // CSVファイルの初期化
        std::string csv_filename = "output_file/"+raspi_name;
        csv_lib::Csvedit csvWriter(csv_filename);
        csvWriter.csv_write_headers({"TT","RT","TD","NaN"});
        // csvデータの型定義  
        std::pair<std::vector<int64_t>,std::vector<double>>  csv_data;
         // 送信データの定義
        std::vector<double> send_data{0,0};
        double delay_time = 0.0; // 遅延時間の初期化
        // データ受信を無限ループで行う
        while (running) {
            // UDP受信
            std::pair<std::vector<double>, int64_t> receivedData = udpConnection_receive.udp_recv();// pairはfirst, secondで抽出可能
            
            // 現在時刻取得v
            std::chrono::high_resolution_clock::time_point receive_clock = std::chrono::high_resolution_clock::now();
            nano_receive_clock = std::chrono::duration_cast<std::chrono::nanoseconds>(receive_clock.time_since_epoch());
            delay_time = (nano_receive_clock.count() - receivedData.second) / 1000000.0; // ミリ秒単位に変換
            // 出力
            //std::cout << "delay_time : " <<send_data[5]  << std::endl;
            //csv出力
            send_data = {delay_time, 0};
            csv_data = {{receivedData.second, nano_receive_clock.count()},send_data};
            // データをペア型にして書き込み
            csvWriter.csv_write_data(csv_data);
        }

    } catch (const std::exception &e) {
        std::cerr << "[Error] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void move_csv_PC(){
    std::string local_file = "output_file/"+pc_name;
    std::string remote_path = R"(/mnt/shared_csv/)"+pc_name; // 共有フォルダのパスを指定
    std::cout << "[Info] Moving file from " << local_file << " to " << remote_path << std::endl;
    try {
        std::filesystem::copy_file(local_file, remote_path, std::filesystem::copy_options::overwrite_existing);

        // 移動にしたい場合は元ファイルを削除
        //std::filesystem::remove(local_file);

        std::cout << "[Info] File move completed" << std::endl;
    } catch (std::filesystem::filesystem_error& e) {
        std::cerr << "[Error] " << e.what() << std::endl;
    }

}

void move_csv_raspi(){
    std::string local_file = "output_file/"+raspi_name;
    std::string remote_path = R"(/mnt/shared_csv/)"+raspi_name; // 共有フォルダのパスを指定
    std::cout << "[Info] Moving file from " << local_file << " to " << remote_path << std::endl;
    try {
        std::filesystem::copy_file(local_file, remote_path, std::filesystem::copy_options::overwrite_existing);

        // 移動にしたい場合は元ファイルを削除
        //std::filesystem::remove(local_file);

        std::cout << "[Info] File move completed" << std::endl;
    } catch (std::filesystem::filesystem_error& e) {
        std::cerr << "[Error] " << e.what() << std::endl;
    }
}

void handle_sigint(int) {
    std::cerr << "\nCtrl+C ..." << std::endl;
    running = false;  // スレッドを止めるため
}

int main(int argc, char* argv[]){
    // argc[1]に選択したいlocationを渡す
    // argc[2]に監視用PCのIPアドレスを渡す
    // argc[3]に監視用PCのポート番号を渡す
    // 対象locationを渡す
    try{
        std::signal(SIGINT, handle_sigint);
        // CPUのガバナーを設定
        set_cpu_governor("performance");
        std::vector<std::string> selectlocation = parse_command_line(argc, argv);

        std::thread th1(receive, selectlocation);
        std::thread th2(receive_test_from_raspi); //
        th1.join();
        th2.join();
    } catch (const std::exception &e) {
        std::cerr << "[Error] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "[Info] Ctrl+C end..." << std::endl;
    // ファイル移動
    move_csv_PC();
    //move_csv_CR();
    move_csv_raspi();
    set_cpu_governor("ondemand");
    return 0;
}