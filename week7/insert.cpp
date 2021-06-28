#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <float.h>
#include <cmath>
using namespace std;

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
    std::cout << "File read succeeded..." << endl;
    return coordinates;
}

double city_distance(const Point& p1, const Point& p2){
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

/*都市0から一番近い都市を返す*/
int nearest_city_from_zero (const std::vector<Point>& coordinates){
    double min_distance = DBL_MAX;
    int min_city;
    for (int i=1; i<(int)coordinates.size(); i++){
        double distance = city_distance(coordinates.at(0), coordinates.at(i));
        cout << i << " " << distance << endl;
        if (min_distance>distance){
            min_distance = distance;
            min_city = i;
        }
    }
    cout << min_city << endl;
    return min_city;
}

std::vector<int> insert_city (const std::vector<Point>& coordinates){
    double distance_one_to_two = city_distance(coordinates.at(0), coordinates.at(1));
    struct node *start = (struct node*)malloc(sizeof(struct node));
    struct node *next_start = (struct node*)malloc(sizeof(struct node));
    start->next = next_start;
    start->city = 0;
    start->distance_to_next = distance_one_to_two;
    next_start->next = start;
    next_start->city = 1;
    next_start->distance_to_next = distance_one_to_two;
    for (int i=2; i<(int)coordinates.size(); i++){
        double min_distance = DBL_MAX;
        struct node *min_node;
        struct node *current = start;
        bool done = false;
        while (!done || current!=start){
            done = true;
            double distance = city_distance(coordinates.at(current->city), coordinates.at(i))+city_distance((coordinates.at(i)), coordinates.at(current->next->city))-current->distance_to_next;
            if (distance<min_distance){
                min_distance = distance;
                min_node = current;
            }
            current = current->next;
        }
        struct node *insert_city = (struct node*)malloc(sizeof(struct node));
        insert_city->next = min_node->next;
        insert_city->city = i;
        insert_city->distance_to_next = city_distance(coordinates.at(insert_city->next->city), coordinates.at(i));
        min_node->next = insert_city;
        min_node->distance_to_next = city_distance(coordinates.at(min_node->city), coordinates.at(i));
    }
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
    std::cout << "file write succeeded..." << endl;
}

int main(int argc, char *argv[]){
    if (argc!=2) error("put any numer");
    std::string file_number = argv[1];
    /*ファイルの読み込み*/
    std::vector<Point> coordinates = read_file(file_number);
    std::vector<int> city_order = insert_city(coordinates);
    /*ファイルに書き込み*/
    write_file(city_order, file_number);
}
