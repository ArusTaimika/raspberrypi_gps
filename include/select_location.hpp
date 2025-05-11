#ifndef SELECT_LOCATION_HPP
#define SELECT_LOCATION_HPP

#include <iostream>
#include <cctype> // tolower を使うために必要
#include <vector>
#include <cstring>    // 文字列操作
#include <unistd.h>   // gethostname用（Linuxの場合）

namespace select_location{

class SelectLocation{
    /*
    Locationを選択するクラス
    */


    public:
        std::vector<std::string> send_selected_ips;
        std::vector<int> send_selected_port;
    

        std::pair<std::string,int> monitored_pc;

        char target_location  = 's';
        char my_location = 's';

        SelectLocation(int argc, char* argv[]); 

        void set_locationip();

        ~SelectLocation();

    private:
        char get_my_location();
        
        char get_target_location(char location);
};

}

#endif // SELECT_LOCATION_HPP  // 3. ここでガードを閉じます