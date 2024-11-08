#include "udp_connect.hpp"

namespace udp_lib {

// コンストラクタ
UdpConnect::UdpConnect(std::string address, int port, size_t element_count) {
    /*
    UDP通信する相手のIPアドレスとポート番号が引数
    */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address.c_str());
    addr.sin_port = htons(port);
    
    //bufferの値を定義
    buffer_size = element_count * sizeof(double);
    buffer = new char [buffer_size];
}

// UDP送信関数（double型データを送信）
void UdpConnect::udp_send(const std::vector<double>& values) {
    //valuesの値をbafferにコピー
    std::memcpy(buffer, values.data(), buffer_size);
    sendto(sock, buffer, buffer_size, 0, (struct sockaddr *)&addr, sizeof(addr));
}

// UDPバインド関数
void UdpConnect::udp_bind() {
    if (bind(sock, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

// UDP受信関数（double型データを受信）
std::vector<double> UdpConnect::udp_recv() {
    struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);
    //データ受信
    size_t received_bytes = recvfrom(sock, buffer, buffer_size, 0, (struct sockaddr*)&sender_addr, &addr_len);
    // 受信データのサイズが設定済みのバッファサイズと一致するか確認
    if (received_bytes != buffer_size) {
        std::cerr << "Error: Received data size mismatch!" << std::endl;
        return {};
    }
    // 受信データをstd::vector<double>に変換
    std::vector<double> received_values(buffer_size / sizeof(double));
    std::memcpy(received_values.data(), buffer, buffer_size);
    return received_values;
}


// デストラクタ
UdpConnect::~UdpConnect() {
    delete[] buffer;  // 動的に確保したメモリを解放
    close(sock);
}


}
