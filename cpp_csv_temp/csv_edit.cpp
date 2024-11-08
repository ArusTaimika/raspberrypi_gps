#include "csv_edit.hpp"
#include <iostream>
#include <fstream>


namespace csv_lib{
// コンストラクタ
Csvedit::Csvedit(const std::string &filename) : filename(filename), file(filename, std::ios::app) {
    if (!file.is_open()) {
        std::cerr << "ファイルを開けませんでした: " << filename << std::endl;
    }
}


// ヘッダを設定する関数
void Csvedit::csv_write_headers(const std::vector<std::string> &headers) {

    if (!file.is_open()) return;

    // ヘッダを書き込む
    for (size_t i = 0; i < headers.size(); ++i) {
        file << headers[i];
        if (i < headers.size() - 1) {
            file << ","; // カンマで区切る
        }
    }
    file << "\n"; // ヘッダ行末に改行を追加
    file.flush();  // バッファの内容をファイルに書き込む
}

// データをCSVファイルに書き込むメソッド
void Csvedit::csv_write_data(const std::pair<std::vector<double>, std::chrono::nanoseconds> &data) {

    if (!file.is_open()) {
        std::cerr << "ファイルを開けませんでした: " << filename << std::endl;
        return;
    }
    // vector<double>のデータを書き込み
    for (size_t i = 0; i < data.first.size(); ++i) {
        file << data.first[i];
        if (i < data.first.size() - 1) {
            file << ","; // カンマで区切る
        }
    }
    // nanosecondsのデータを書き込み
    file << "," << data.second.count(); // ナノ秒はカンマ区切りで追加
    file << "\n"; // 行末に改行を追加
    file.flush();  // バッファの内容をファイルに書き込む

}

Csvedit::~Csvedit(){
    if (file.is_open()){
        file.close();
    }
}

}