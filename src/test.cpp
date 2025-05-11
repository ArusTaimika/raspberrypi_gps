#include <iostream>
#include <cstring>    // 文字列操作
#include <unistd.h>   // gethostname用（Linuxの場合）

char get_location_from_hostname() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        std::cerr << "ホスト名の取得に失敗しました．" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string hostname_str(hostname);
    // たとえば"raspi-a"の末尾の"a"を取得
    if (hostname_str.find("raspi-") == 0 && hostname_str.size() > 6) {
        return hostname_str[6];  // "raspi-"の次の文字
    } else {
        std::cerr << "ホスト名の形式が不正です．" << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main() {
    char location = get_location_from_hostname();
    std::cout << "Location is automatically set to: " << location << std::endl;
    return 0;
}

