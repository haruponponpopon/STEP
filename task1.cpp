#include<map>
#include<string>
#include<set>
#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>




int main(){
    /*変数定義*/
    std::map<std::string, std::string> pages;
    std::map<std::string, std::set<std::string>> links;
    std::map<std::string, int>used; //<ページ番号, 未使用0使用済1>
    std::string start_word; //起点となる言葉
    std::string goal_word; //探したい言葉


    /*標準出力からの受け取り*/
    std::cin >> start_word >> goal_word;

    /*ファイルの読み込み*/
    {
        std::ifstream file("data/pages_small.txt");
        // std::ifstream file("data/pages.txt");
        std::string data;
        while (std::getline(file, data)) {
            auto index = data.find('\t');
            auto id = data.substr(0, index);
            auto title = data.substr(index + 1, data.size() - id.size() - 1);
            pages[id] = title;
            used[id] = 0;  //usedの初期化
        }
    }
    std::cout << pages["33"] << std::endl; //ID33のページ名表示
    {
        std::ifstream file("data/links_small.txt");
        // std::ifstream file("data/links.txt");
        std::string data;
        while (std::getline(file, data)) {
            auto index = data.find('\t');
            auto from = data.substr(0, index);
            auto to = data.substr(index + 1, data.size() - from.size() - 1);
            links[from].insert(to);
        }
    }
    // for (auto x: links["33"]) std::cout << x << " "; //ID33がリンクするIDを全て表示
    // for (auto x: links[33]) std::cout << pages[x] << " "; //ID33がリンクするIDのページ名を表示

}
