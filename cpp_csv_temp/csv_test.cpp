#include "csv_edit.hpp"
#include <vector>
#include <chrono>
#include <thread>

int main() {
    // インスタンス化
    csv_lib::Csvedit csvWriter("output.csv");

    // ヘッダを設定
    csvWriter.csv_write_headers({"Data1", "Data2", "Data3", "Timestamp"});

    // サンプルデータを生成し，ループで書き込み
    for (int i = 0; i < 5; ++i) {
        // データを作成
        std::vector<double> dataVector = {1.1 * i, 2.2 * i, 3.3 * i};
        
        // 現在のタイムスタンプを取得
        auto timestamp = std::chrono::high_resolution_clock::now().time_since_epoch();
        std::chrono::nanoseconds nanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(timestamp);

        // データをペア型にして書き込み
        csvWriter.csv_write_data(std::make_pair(dataVector, nanosec));

        // 少し待機
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
