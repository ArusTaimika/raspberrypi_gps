
#include "select_location.hpp"

namespace select_location {
    SelectLocation::SelectLocation(int argc, char* argv[]) {
        monitored_pc = std::pair<std::string,int> {argv[2],std::stoi(argv[3])};
        target_location = get_target_location(argv[1][0]);
        my_location = get_my_location();
    }

    char SelectLocation::get_my_location(){
        char hostname[256]; // hostnameの最大サイズ
        if(gethostname(hostname, sizeof(hostname)) != 0){
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
    
    char SelectLocation::get_target_location(char location){
        if(location != 'a' && location != 'b' && location != 'c' && location != 'd'){
            std::cerr << "入力が不正です" << std::endl;
            exit(EXIT_FAILURE);
        }
        return location;
    }

    void SelectLocation::set_locationip(){
        std::string copy_ip;
        switch (my_location){
            case 'a':
                switch (target_location){
                    case 'a':
                        std::cerr << "選択できない" << std::endl; 
                        break;
                    case 'b':
                        copy_ip = "100.77.38.11";
                        break;
                    case 'c':
                        copy_ip = "100.77.38.12";
                        break;
                    case 'd':
                        copy_ip = "100.77.38.13";
                        break;
                }
                break;
            case 'b':
                switch (target_location){
                    case 'a':
                        copy_ip = "100.77.38.21";
                        break;
                    case 'b':
                        std::cerr << "選択できない" << std::endl; 
                        break;
                    case 'c':
                        copy_ip = "100.77.38.22";
                        break;
                    case 'd':
                        copy_ip = "100.77.38.23";
                        break;
                }
                break;
            case 'c':
                switch (target_location){
                    case 'a':
                        copy_ip = "100.77.38.31";
                        break;
                    case 'b':
                        copy_ip = "100.77.38.32";
                        break;
                    case 'c':
                        std::cerr << "選択できない" << std::endl; 
                        break;
                    case 'd':
                        std::cerr << "選択できない" << std::endl; 
                        break;
                }
                break;
            case 'd':
                switch (target_location){
                    case 'a':
                        copy_ip = "100.77.38.31";
                        break;
                    case 'b':
                        copy_ip = "100.77.38.32";
                        break;
                    case 'c':
                        std::cerr << "選択できない" << std::endl; 
                        break;
                    case 'd':
                        std::cerr << "選択できない" << std::endl; 
                        break;
                }
                break;
        }
        send_selected_ips = {copy_ip, /*to Master*/ monitored_pc.first  /*to TestPC*/};
        send_selected_port = {40000,monitored_pc.second};

    }
    // デストラクタ
    SelectLocation::~SelectLocation() {
    }
}