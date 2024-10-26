#include "csv_edit.hpp"
#include <vector>
#include <chrono>
#include <thread>

int main() {
    // インスタンス化
    csv_lib::Csvedit csvWriter("output.csv");

    // ヘッダを設定
    csvWriter.csv_write_headers({"server_time", "client_time", "delay_time"});

    // 送信データの型定義
    std::vector<std::chrono::nanoseconds>  send_data;
    // サンプルデータを生成し，ループで書き込み
    for (int i = 0; i < 5; ++i) {
        
        // 現在のタイムスタンプを取得
        auto timestamp = std::chrono::high_resolution_clock::now().time_since_epoch();
        std::chrono::nanoseconds nanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(timestamp);
        send_data = {timestamp, std::chrono::nanoseconds(200), std::chrono::nanoseconds(100)};
        // データをペア型にして書き込み
        csvWriter.csv_write_data(send_data);

        // 少し待機
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
