#include <fstream>
// #include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>  //デバッグprint用

//ファイルからx,y座標を読み込んでvectorに入れて返す
std::vector<std::vector<long double>> read_file(std::string file_name){
    std::vector<std::vector<long double>> coordinate(2);
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
        auto x = std::stold(data.substr(0,index));
        auto y = std::stold(data.substr(index+1));
        coordinate[0].push_back(x);
        coordinate[1].push_back(y);
    }
    return coordinate;
}
int main(){
    std::vector<std::vector<long double>> coordinate;
    coordinate = read_file("input_0.csv");
    for (int i=0; i<(int)coordinate.size(); i++){
        for (int j=0; j<(int)coordinate[i].size(); j++){
            std::cout << coordinate[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
