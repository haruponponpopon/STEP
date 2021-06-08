#include <fstream>
// #include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>  //デバッグprint用

//ファイルからx,y座標を読み込んでvectorに入れて返す
std::vector<std::vector<double>> read_file(const std::string& file_name){
    std::vector<std::vector<double>> coordinate(2);
    std::ifstream file(file_name);
    if (!file) {
        std::cerr << "File not found" << std::endl;
        std::exit(1);
    }
    std::string data;
    std::getline(file, data); //不要な一行目を読みこむ
    if (data!="x,y"){
        std::cout << "File is in the wrong format" << std::endl;
        std::exit(1);
    }
    while (std::getline(file, data)){
        auto index = data.find(',');
        try {
            auto x = std::stod(data.substr(0,index));
            auto y = std::stod(data.substr(index+1));
            coordinate[0].push_back(x);
            coordinate[1].push_back(y);
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "[" << data << "]: " << "invalid argument" << std::endl;
            std::exit(1);
        }
    }
    return coordinate;
}

double city_distance(const double ax, const double ay, const double bx, const double by){
    return sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
}

//貪欲法で一番近い都市を入れていく
std::vector<int> choose_nearest_city(const std::vector<std::vector<double>>& coordinate){
    int city_count = (int)coordinate[0].size();
    std::vector<int> used(city_count);
    std::vector<int> ans_city_order;
    int current_city = 0;
    ans_city_order.push_back(current_city);
    while ((int)ans_city_order.size()<city_count){
        used[current_city] = 1;
        int min_city;
        double min_city_distance = -1;
        for (int i=0; i<city_count; i++){
            if (used[i]==0){
                double distance = city_distance(coordinate[0][current_city], coordinate[1][current_city], coordinate[0][i], coordinate[1][i]);
                if (min_city_distance==-1||min_city_distance > distance){
                    min_city_distance = distance;
                    min_city = i;
                }
            }
        }
        ans_city_order.push_back(min_city);
        current_city = min_city;
    }
    return ans_city_order;
}

void write_file(const std::vector<int>& ans_city_order, const std::string& file_name){
    std::string filename(file_name);
    std::fstream file_out;

    file_out.open(filename, std::ios_base::out);
    if (!file_out.is_open()) {
        std::cerr << "failed to open " << filename << '\n';
        std::exit(1);
    } else {
        file_out << "index" << std::endl;
        for (int i=0; i<(int)ans_city_order.size(); i++){
            file_out << ans_city_order.at(i) << std::endl;
        }
        std::cout << "file write succeeded!" << std::endl;
    }
}


int main(int argc, char *argv[]){
    if (argc!=3) {
        std::cerr << "put two file name" << std::endl;
        std::exit(1);
    }
    /*ファイルの読み込み*/
    std::vector<std::vector<double>> coordinate = read_file(argv[1]);
    /*貪欲法による経路*/
    std::vector<int> greedy_city_order = choose_nearest_city(coordinate);
    /*ファイルに書き込み*/
    write_file(greedy_city_order, argv[2]);
}
