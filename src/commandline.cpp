# include "commandline.hpp"


std::string get_my_name(){
    /* ホスト名（raspi-以下）を抽出する関数*/
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0){
        std::cerr << "[Error] getting hostname" << std::endl;
        exit(1);
    }

    std::string hostname_str(hostname);
    return (hostname_str.substr(6)).substr(0,1);
}


std::vector<std::string> parse_command_line(int argc, char* argv[]){
    if (argc < 4){
        std::cerr << "[Error] Not enough arguments" << std::endl;
        std::cerr << "[Info] 1:local or tailscale \n"
                  << "       2:Target CopyRobot name (e.g., cra1, crb1, crc1, cra2, crb2, crc2, cra3, crb3) \n"
                  << "       3:Target location (e.g., a, b, c, d) \n"
        << std::endl;
        exit(1);
    }
    auto select_mode = argv[1];
    auto target_copyrobot_name = argv[2];
    std::string head_ip;
    // Set target CopyRobot IP address
    if (std::string(select_mode) == std::string("local")){
        head_ip = "192.168.11.";
    }
    else if (std::string(select_mode) == std::string("tailscale")){
        head_ip = "100.77.38.";
    }
    else{
        std::cerr << "[Error] Invalid mode" << std::endl;
        exit(1);
    }
    std::unordered_map<std::string, std::string> ip_suffixes = {
        {"cra1", "11"}, {"crb1", "21"}, {"crc1", "31"},
        {"cra2", "12"}, {"crb2", "22"}, {"crc2", "32"}
    };
    std::string my_name = get_my_name();
    std::cout << "[Info] This Raspberry Pi name: " << my_name << std::endl;
    if ("a" == my_name){
        ip_suffixes = {
            {"cra1", "11"}, 
            {"cra2", "12"}, {"crb2", "22"}, {"crc2", "32"},
            {"cra3", "13"}, {"crb3", "23"}
        };
    }
    else if ("b" == my_name){
        ip_suffixes = {
                            {"crb1", "21"}, {"crc1", "31"},
            {"cra2", "12"}, {"crb2", "22"},
            {"cra3", "13"}, {"crb3", "23"}
        };
    }
    else if ("c" == my_name){
            ip_suffixes = {
            {"cra1", "11"}, {"crb1", "21"}, {"crc1", "31"},
                                            {"crc2", "32"},
            {"cra3", "13"}, {"crb3", "23"}
        };
    }
    else if ("d" == my_name){
        ip_suffixes = {
            {"cra1", "11"}, {"crb1", "21"}, {"crc1", "31"},
            {"cra2", "12"}, {"crb2", "22"}, {"crc2", "32"}
        };
    }
    auto it = ip_suffixes.find(target_copyrobot_name);
    if (it == ip_suffixes.end()) {
        std::cerr << "[Error] Invalid CopyRobot name" << std::endl;
        std::exit(1);
    }
    std::string target_copyrobot_ip = head_ip + it->second;

    // Target location
    static const std::unordered_map<std::string, std::string> ip_suffixes2 = {
            {"a", "201"}, {"b", "202"}, {"c", "203"}, {"d", "204"}
    };
    auto it2 = ip_suffixes2.find(std::string(1, argv[3][0]));
    if (it2 == ip_suffixes2.end()) {
        std::cerr << "[Error] Invalid Target location" << std::endl;
        std::exit(1);
    }
    std::string target_raspi_ip = head_ip + it2->second;
    std::cout << "[Info] Target CopyRobot IP: " << target_copyrobot_ip << std::endl;
    std::cout << "[Info] Target raspi IP: " << target_raspi_ip << std::endl;
    return {target_copyrobot_ip, target_raspi_ip};
}
