#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <float.h>
#include <cmath>
using namespace std; //後で消すこと

struct Point{
    double x;
    double y;
};

/*エラーを出力する関数*/
void error(const std::string& error_message){
    std::cerr << error_message << std::endl;
    std::exit(1);
}

/*ファイルからx,y座標を読み込んでvectorに入れて返す*/
std::vector<Point> read_file(const std::string& file_number){
    std::vector<Point> coordinates;
    std::ifstream file("input_"+file_number+".csv");
    if (!file) error("read_file:File not found");
    std::string line;
    std::getline(file, line); //不要な一行を読み込む
    if (line!="x,y") error("read_file:File is in the wrong format");
    while (std::getline(file, line)){
        auto index = line.find(',');
        try {
            auto x = std::stod(line.substr(0,index));
            if (!(index+1)) error("read_file:File is in the wrong format"); //','が存在しない
            auto y = std::stod(line.substr(index+1));
            struct Point xy;
            xy.x = x;
            xy.y = y;
            coordinates.push_back(xy);
        }
        catch (const std::invalid_argument& e){
            error("read_file:["+line+"]:"+"invalid argument");
        }
        catch (const std::out_of_range& e){
            error("read_file:Invalid number in file"); //極端に大きな値、小さな値が入っている
        }
    }
    std::cout << "File read succeeded..." << endl;
    return coordinates;
}

double city_distance(const Point& p1, const Point& p2){
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

/*貪欲法で一番近い都市を入れていく*/
std::vector<int> choose_nearest_city(const std::vector<Point>& coordinates){
    size_t city_count = coordinates.size();
    std::vector<int> used(city_count);
    std::vector<int> city_order;
    int current_city = 0;
    city_order.push_back(current_city);
    used.at(current_city) = 1;
    while (city_order.size()<city_count){
        used.at(current_city) = 1;
        int min_city;
        double min_city_distance = DBL_MAX;
        for (int i=0; i<(int)city_count; i++){
            if (!used.at(i)){
                double distance = city_distance(coordinates.at(current_city), coordinates.at(i));
                if (min_city_distance>distance){
                    min_city_distance = distance;
                    min_city = i;
                }
            }
        }
        city_order.push_back(min_city);
        current_city = min_city;
    }
    std::cout << "greedy1 algorithm succeeded..." << std::endl;
    return city_order;
}

/*貪欲法で1番近い2都市の一番近い都市を入れていく*/
std::vector<int> choose_nearest_city2(const std::vector<Point>& coordinates){
    size_t city_count = coordinates.size();
    std::vector<int> used(city_count);
    std::vector<int> city_order;
    int current_city = 0;
    int next_city;
    city_order.push_back(current_city);
    used.at(current_city) = 1;
    while (city_order.size()<city_count-1){
        used.at(current_city) = 1;
        int min_city;
        double min_city_distance = DBL_MAX;
        for (int i=0; i<(int)city_count; i++){
            for (int j=0; j<(int)city_count; j++){
                if (!used.at(i)&&!used.at(j)&&i!=j){
                    double distance = city_distance(coordinates.at(current_city), coordinates.at(i))+city_distance(coordinates.at(i), coordinates.at(j));
                    if (min_city_distance>distance){
                        min_city_distance = distance;
                        min_city = i;
                        next_city = j;
                    }
                }
            }
        }
        city_order.push_back(min_city);
        current_city = min_city;
    }
    city_order.push_back(next_city);
    std::cout << "greedy2 algorithm succeeded..." << std::endl;
    return city_order;
}

/*貪欲法で1番近い2都市を両方入れていく*/
std::vector<int> choose_nearest_city3(const std::vector<Point>& coordinates){
    int city_count = (int)coordinates.size();
    std::vector<int> used(city_count);
    std::vector<int> city_order;
    int current_city = 0;
    city_order.push_back(current_city);
    used.at(current_city) = 1;
    while ((int)city_order.size()<city_count-1){
        int min_city1;
        int min_city2;
        double min_city_distance = DBL_MAX;
        for (int i=0; i<city_count; i++){
            for (int j=0; j<city_count; j++){
                if (!used.at(i)&&!used.at(j)&&i!=j){
                    double distance = city_distance(coordinates.at(current_city), coordinates.at(i))+city_distance(coordinates.at(i), coordinates.at(j));
                    if (min_city_distance>distance){
                        min_city_distance = distance;
                        min_city1 = i;
                        min_city2 = j;
                    }
                }
            }
        }
        city_order.push_back(min_city1);
        city_order.push_back(min_city2);
        used.at(min_city1) = 1;
        used.at(min_city2) = 1;
        current_city = min_city2;
    }
    if (!(city_count%2)){
        for (int i=0; i<city_count; i++){
            if (!used.at(i)) city_order.push_back(i);
        }
    }
    std::cout << "greedy3 algorithm succeeded..." << std::endl;
    return city_order;
}

/*入れ替えたら経路が短くなるときtrueを返す*/
bool swap_makes_shorter(const std::vector<Point>& coordinates, const int city_A1, const int city_A2, const int city_B1, const int city_B2){
    double before_swap = city_distance(coordinates.at(city_A1), coordinates.at(city_A2)) + city_distance(coordinates.at(city_B1), coordinates.at(city_B2));
    double after_swap = city_distance(coordinates.at(city_A1), coordinates.at(city_B1)) + city_distance(coordinates.at(city_A2), coordinates.at(city_B2));
    return after_swap<before_swap;
}

/*city_A<city_Bの時のみクロスを解除する。それ以外ならエラー*/
void uncross(std::vector<int> &city_order, int city_A, int city_B){
    if (city_B<=city_A) error ("uncross: invalid");
    while (city_A<city_B){
        std::swap(city_order.at(city_A), city_order.at(city_B));
        city_A++;
        city_B--;
    }
}

/*交差している道をswapして交差をなくす*/
void two_opt(std::vector<int>& city_order, const std::vector<Point>& coordinates){
    int city_count = (int)city_order.size();
    int count = 1;
    while (count){
        count=0;
        for (int i=0; i<city_count-3; i++){
            for (int j=i+2; j<city_count-1; j++){
                if (swap_makes_shorter(coordinates, city_order.at(i), city_order.at(i+1), city_order.at(j), city_order.at(j+1))){
                    uncross(city_order, i+1, j);
                    count++;
                }
            }
        }
        for (int i=1; i<city_count-1; i++){
            if (swap_makes_shorter(coordinates, city_order.at(i), city_order.at(i+1), city_order.at(city_count-1), city_order.at(0))){
                uncross(city_order, i+1, city_count-1);
                count++;
            }
        }
    }
}

/*ファイルの書き込み*/
void write_file(const std::vector<int>& city_order, const std::string& file_number){
    std::string filename("output_"+file_number+".csv");
    std::fstream file_out;

    file_out.open(filename, std::ios_base::out);
    if (!file_out.is_open()) error("write_file:failed to open");
    file_out << "index" << std::endl;
    for (size_t i=0; i<city_order.size(); i++) file_out << city_order.at(i) << std::endl;
    std::cout << "file write succeeded..." << endl;
}

int main(int argc, char *argv[]){
    if (argc!=3) error("argv: put any numer and instruction");
    std::string file_number = argv[1];
    /*ファイルの読み込み*/
    std::vector<Point> coordinates = read_file(file_number);
    /*貪欲法による経路*/
    std::vector<int> city_order(coordinates.size());
    if (*argv[2]=='1') city_order = choose_nearest_city(coordinates);
    else if (*argv[2]=='2') city_order = choose_nearest_city2(coordinates);
    else if (*argv[2]=='3') city_order = choose_nearest_city3(coordinates);
    else error("argv: instruction is invalid");
    /*交差した道をswapして交差をなくす*/
    two_opt(city_order, coordinates);
    /*ファイルに書き込み*/
    write_file(city_order, file_number);
}
