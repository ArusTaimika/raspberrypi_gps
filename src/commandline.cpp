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
    if (argc < 7){
        std::cerr << "[Error] Not enough arguments" << std::endl;
        std::cerr << "[Info] 1:local or tailscale \n"
                  << "       2:Target CopyRobot name 1 (e.g., cra1, crb1, crc1, cra2, crb2, crc2, cra3, crb3) \n"
                  << "       3:Target CopyRobot name 2 (e.g., cra1, crb1, crc1, cra2, crb2, crc2, cra3, crb3) \n"
                  << "       4:Target location 1(e.g., a, b, c, d) \n"
                  << "       5:Target location 2 (e.g., a, b, c, d) \n"
                  << "       6:Monitored PC Port (e.g., 50222, 51222, 52222) \n"
        << std::endl;
        exit(1);
    }
    /*
    * Select mode: local or tailscale
    * Target CopyRobot name 1: cra1, crb1, crc1, cra2, crb2, crc2, cra3, crb3
    * Target CopyRobot name 2: cra1, crb1, crc1, cra2, crb2, crc2, cra3, crb3
    */
    auto select_mode = argv[1];
    auto target_copyrobot_name_1 = argv[2];
    auto target_copyrobot_name_2 = argv[3];
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
    auto copy_ip_1 = ip_suffixes.find(target_copyrobot_name_1);
    auto copy_ip_2 = ip_suffixes.find(target_copyrobot_name_2);
    if (copy_ip_1 == ip_suffixes.end()) {
        std::cerr << "[Error] Invalid CopyRobot name" << std::endl;
        std::exit(1);
    }
    if (copy_ip_2 == ip_suffixes.end()) {
        std::cerr << "[Error] Invalid CopyRobot name" << std::endl;
        std::exit(1);
    }
    if (copy_ip_1 == copy_ip_2){
        std::cerr << "[Error] Same CopyRobot name" << std::endl;
        std::exit(1);
    }
    std::string target_copyrobot_ip_1 = head_ip + copy_ip_1->second;
    std::string target_copyrobot_ip_2 = head_ip + copy_ip_2->second;

    /*
    * Target location
    */
    static const std::unordered_map<std::string, std::string> ip_suffixes2 = {
            {"a", "201"}, {"b", "202"}, {"c", "203"}, {"d", "204"}
    };
    auto target_loc_1 = ip_suffixes2.find(std::string(1, argv[4][0]));
    auto target_loc_2 = ip_suffixes2.find(std::string(1, argv[5][0]));
    if (target_loc_1 == ip_suffixes2.end()) {
        std::cerr << "[Error] Invalid Target location" << std::endl;
        std::exit(1);
    }
    if (target_loc_2 == ip_suffixes2.end()) {
        std::cerr << "[Error] Invalid Target location" << std::endl;
        std::exit(1);
    }
    if (target_loc_1 == target_loc_2){
        std::cerr << "[Error] Same Target location" << std::endl;
        std::exit(1);
    }
    std::string target_raspi_ip_1 = head_ip + target_loc_1->second;
    std::string target_raspi_ip_2 = head_ip + target_loc_2->second;
    std::cout << "[Info] Multilateral Communication Setup" << std::endl;
    std::cout << "[Info] Target CopyRobot IP 1: " << target_copyrobot_ip_1 << std::endl;
    std::cout << "[Info] Target CopyRobot IP 2: " << target_copyrobot_ip_2 << std::endl;
    std::cout << "[Info] Target raspi IP 1: " << target_raspi_ip_1 << std::endl;
    std::cout << "[Info] Target raspi IP 2: " << target_raspi_ip_2 << std::endl;

    // CSV filenames
    std::string pc_name = "PC"+my_name+".csv";
    std::string raspi_name = "raspi"+my_name+".csv";
    // My location number
    std::string my_location_number = argv[6];
    if (my_location_number != "50222" && my_location_number != "51222" && my_location_number != "52222"){
        std::cerr << "[Error] Invalid Monitored PC Port" << std::endl;
        std::exit(1);
    }
    std::cout << "[Info] Monitored PC Location: " << my_location_number << std::endl;
    return {target_copyrobot_ip_1, target_copyrobot_ip_2,
         target_raspi_ip_1, target_raspi_ip_2, pc_name, raspi_name, my_location_number};
}
