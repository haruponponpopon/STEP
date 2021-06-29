#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <float.h>
#include <cmath>
#include <numeric>
#include <algorithm>

struct Point{
    double x;
    double y;
};

struct node{
    struct node *next;
    int city;
    double distance_to_next;
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
    std::cout << "File read succeeded..." << std::endl;
    return coordinates;
}

double city_distance(const Point& p1, const Point& p2){
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

/*都市0からinstruction==2なら近い順にその他なら遠い順に都市番号を並べる*/
std::vector<int> nearest_city_order_from_zero (const std::vector<Point>& coordinates, char instruction){
    int city_count = (int)coordinates.size();
    std::vector<double> distance_from_zero(city_count);
    for (int i=1; i<city_count; i++) distance_from_zero.at(i) = city_distance(coordinates.at(0), coordinates.at(i));
    std::vector<int> city_check_order(city_count);
    std::iota(city_check_order.begin(), city_check_order.end(),0);
    if (instruction == '2'){
        std::cout << "city_check_order: nearest" << std::endl;
        sort(city_check_order.begin(), city_check_order.end(),[&](int x,int y) {
            return distance_from_zero[x] < distance_from_zero[y];
        });
    }else{
        std::cout << "city_check_order: farthest" << std::endl;
        sort(city_check_order.begin(), city_check_order.end(),[&](int x,int y) {
            return distance_from_zero[x] > distance_from_zero[y];
        });
    }
    return city_check_order;
}

/*訪れる都市をinsert法によって決める*/
std::vector<int> insert_city (const std::vector<Point>& coordinates, const std::vector<int>& city_check_order){
    if (coordinates.size()<2) error("insert_city: number of city is too small.");
    //初期設定(2つの都市をループさせる)
    double distance_one_to_two = city_distance(coordinates.at(city_check_order.at(0)), coordinates.at(city_check_order.at(1)));
    struct node *start = (struct node*)malloc(sizeof(struct node));
    struct node *next_start = (struct node*)malloc(sizeof(struct node));
    start->next = next_start;
    start->city = city_check_order.at(0);
    start->distance_to_next = distance_one_to_two;
    next_start->next = start;
    next_start->city = city_check_order.at(1);
    next_start->distance_to_next = distance_one_to_two;

    //未訪問の都市を順に線形リストに入れる
    for (int i=2; i<(int)coordinates.size(); i++){
        double min_distance = DBL_MAX;
        struct node *min_node;
        struct node *current = start;
        bool done = false;
        while (!done || current!=start){
            done = true;
            double distance = city_distance(coordinates.at(current->city), coordinates.at(city_check_order.at(i)))+city_distance((coordinates.at(city_check_order.at(i))), coordinates.at(current->next->city))-current->distance_to_next;
            if (distance<min_distance){
                min_distance = distance;
                min_node = current;
            }
            current = current->next;
        }
        struct node *insert_city = (struct node*)malloc(sizeof(struct node));
        insert_city->next = min_node->next;
        insert_city->city = city_check_order.at(i);
        insert_city->distance_to_next = city_distance(coordinates.at(insert_city->next->city), coordinates.at(city_check_order.at(i)));
        min_node->next = insert_city;
        min_node->distance_to_next = city_distance(coordinates.at(min_node->city), coordinates.at(city_check_order.at(i)));
    }
    //線形リストを配列に変換
    struct node *current = start;
    std::vector<int> city_order;
    bool done = false;
    while (!done||current!=start){
        done = true;
        city_order.push_back(current->city);
        current=current->next;
    }
    return city_order;
}

/*ファイルの書き込み*/
void write_file(const std::vector<int>& city_order, const std::string& file_number){
    std::string filename("output_"+file_number+".csv");
    std::fstream file_out;

    file_out.open(filename, std::ios_base::out);
    if (!file_out.is_open()) error("write_file:failed to open");
    file_out << "index" << std::endl;
    for (size_t i=0; i<city_order.size(); i++) file_out << city_order.at(i) << std::endl;
    std::cout << "file write succeeded..." << std::endl;
}

int main(int argc, char *argv[]){
    if (argc!=3) error("put any numer and instruction");
    std::string file_number = argv[1];
    /*ファイルの読み込み*/
    std::vector<Point> coordinates = read_file(file_number);
    /*線形リストに挿入する都市の順番を決める*/
    std::vector<int> city_check_order(coordinates.size());
    if (*argv[2] == '1') {
        std::iota(city_check_order.begin(), city_check_order.end(), 0);
        std::cout << "city_check_order: default" << std::endl;
    }
    else city_check_order = nearest_city_order_from_zero(coordinates, *argv[2]);
    /*0番目から順にinsertしていく。*/
    std::vector<int> city_order = insert_city(coordinates, city_check_order);
    // /*ファイルに書き込み*/
    write_file(city_order, file_number);
}
