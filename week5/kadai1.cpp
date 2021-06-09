#include <fstream>
// #include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>  //デバッグprint用

//ファイルからx,y座標を読み込んでvectorに入れて返す
std::vector<std::vector<double>> read_file(const std::string& file_number){
    std::vector<std::vector<double>> coordinate(2);
    std::ifstream file("input_"+file_number+".csv");
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

/*交差しているか否かを判定する f(x,y) = (x1-x2)(y-y1)+(x1-x)(y1-y2)
g(x,y) = (x1-x2)(y-y1)+(x1-x)(y1-y2) を考え、f(x,y)とg(x,y)の正負が異なるとき交差する。
*/
bool cross_city(const std::vector<std::vector<double>>& coordinate, const int city_A1, const int city_A2, const int city_B1, const int city_B2){
    double judge1 = ((coordinate[0][city_A1]-coordinate[0][city_A2])*(coordinate[1][city_B1]-coordinate[1][city_A1])+
    (coordinate[0][city_A1]-coordinate[0][city_B1])*(coordinate[1][city_A1]-coordinate[1][city_A2]))*
    ((coordinate[0][city_A1]-coordinate[0][city_A2])*(coordinate[1][city_B2]-coordinate[1][city_A1])+
    (coordinate[0][city_A1]-coordinate[0][city_B2])*(coordinate[1][city_A1]-coordinate[1][city_A2]));
    double judge2 = ((coordinate[0][city_B1]-coordinate[0][city_B2])*(coordinate[1][city_A1]-coordinate[1][city_B1])+
    (coordinate[0][city_B1]-coordinate[0][city_A1])*(coordinate[1][city_B1]-coordinate[1][city_B2]))*
    ((coordinate[0][city_B1]-coordinate[0][city_B2])*(coordinate[1][city_A2]-coordinate[1][city_B1])+
    (coordinate[0][city_B1]-coordinate[0][city_A2])*(coordinate[1][city_B1]-coordinate[1][city_B2]));
    if (judge1<0&&judge2<0) return true;
    else return false;
}

void uncross(std::vector<int> &city_order, int city_A, int city_B){
    if (city_A>city_B){
        std::swap(city_A, city_B);
    }
    while (city_A<city_B){
        std::swap(city_order.at(city_A), city_order.at(city_B));
        city_A++;
        city_B--;
    }
}

//交差している道をswapして交差をなくす
void two_opt(std::vector<int>& city_order, const std::vector<std::vector<double>>& coordinate){
    int city_count = (int)city_order.size();
    //最後尾と先頭
    for (int i=0; i<city_count-1; i++){
        if (cross_city(coordinate, city_order.at(0), city_order.at(city_count-1), city_order.at(i), city_order.at(i+1))){
            uncross(city_order, i+1, city_count-1);
        }
    }
    for (int i=0; i<city_count-1; i++) {
        int previous_j = -1;
        for (int j=0; j<city_count-1; j++){
            if (cross_city(coordinate, city_order.at(i), city_order.at(i+1), city_order.at(j), city_order.at(j+1))){
                uncross(city_order, i+1, j);
                if (j!=previous_j) {
                    previous_j=j;
                    j=0;
                }
            }
        }
    }
}

void write_file(const std::vector<int>& ans_city_order, const std::string& file_number){
    std::string filename("output_"+file_number+".csv");
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
    if (argc!=2) {
        std::cerr << "put any number of 0-6" << std::endl;
        std::exit(1);
    }
    std::string a = argv[1];
    /*ファイルの読み込み*/
    std::vector<std::vector<double>> coordinate = read_file(argv[1]);
    /*貪欲法による経路*/
    std::vector<int> ans_city_order = choose_nearest_city(coordinate);

    /*交差した道をswapして交差をなくす*/
    two_opt(ans_city_order, coordinate);
    /*ファイルに書き込み*/
    write_file(ans_city_order, argv[1]);
}
