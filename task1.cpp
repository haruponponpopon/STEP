#include<map>
#include<string>
#include<set>
#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <queue>

//pagesファイルを読み込む
void read_pages(std::map<std::string, std::string> &pages){
    // std::ifstream file("data/pages_small.txt");
    std::ifstream file("data/pages.txt");
    std::string data;
    while (std::getline(file, data)) {
        auto index = data.find('\t');
        auto id = data.substr(0, index);
        auto title = data.substr(index + 1, data.size() - id.size() - 1);
        pages[id] = title;
    }
}

//linksファイルを読み込む
void read_links(std::map<std::string, std::set<std::string>> &links){
    // std::ifstream file("data/links_small.txt");
    std::ifstream file("data/links.txt");
    std::string data;
    while (std::getline(file, data)) {
        auto index = data.find('\t');
        auto from = data.substr(0, index);
        auto to = data.substr(index + 1, data.size() - from.size() - 1);
        links[from].insert(to);
    }
}

//ページをIDに変換する
void search_ID(const std::string &start_page, const std::string &goal_page, 
std::string &start_ID, std::string &goal_ID, const std::map<std::string, std::string> &pages){
    int already_searched = 0;
    for (const auto& page : pages) {
        if (page.second == start_page) {
            already_searched++;
            start_ID =  page.first;
            if (already_searched==2) break;
        }
        if (page.second == goal_page) {
            already_searched++;
            goal_ID = page.first;
            if (already_searched==2) break;
        }
    }
    if (already_searched!=2){  //startかgoalの少なくとも片方が見つからなかった
        std::cout << "Invalid word" << already_searched << std::endl;
        assert(0);
    }
    return;
}

//探すIDが見つかるまで幅優先探索を行う。見つかったらtrueを見つからなかったらfalseを返す
bool bfs_goal_ID(const std::string &start_ID, const std::string &goal_ID, std::map<std::string, std::string> &previous_ID,
std::map<std::string, std::set<std::string>> &links){
    bool found = false;
    std::queue<std::string> que;
    que.push(start_ID); //初期設定

    /*dfs開始*/
    while(!que.empty()){  
        std::string current_ID = que.front();
        que.pop();
        if (current_ID==goal_ID) {
            found = true;
            break;
        }
        for (auto next_ID: links[current_ID]){
            if (previous_ID[next_ID].size()) continue;
            previous_ID[next_ID] = current_ID;
            que.push(next_ID);
        }

    }
    return found;
}

//goalからstartまで辿っていき、途中で通るpageのIDをroute配列に入れて返す。
void reverse_route(const std::string &start_ID, const std::string &goal_ID, 
std::map<std::string, std::string> &previous_ID, std::vector<std::string> &route){
    std::string current_ID= goal_ID;
    route.push_back(current_ID);
    while(1){
        current_ID = previous_ID[current_ID];
        route.push_back(current_ID);
        if (current_ID==start_ID) break;
    }
    return;
}

//route配列をもとに標準出力
void print_route(std::vector<std::string> &route, std::map<std::string, std::string> &pages){
    for (int i=(int)route.size()-1; i>=0; i--){
        std::cout << pages[route.at(i)];
        if (i) std::cout << "->";
    }
}

int main(){
    /*変数定義*/
    std::map<std::string, std::string> pages;
    std::map<std::string, std::set<std::string>> links;
    std::map<std::string, std::string> previous_ID; //そのページの一つ前のページのID
    std::string start_page; //起点となる言葉
    std::string goal_page; //探したい言葉
    std::string start_ID;
    std::string goal_ID;
    std::vector<std::string> route;  //startからgoalまでの道のりに出てくるPageのIDをしまう


    /*標準出力からの受け取り*/
    std::cin >> start_page >> goal_page;

    /*ファイルの読み込み*/
    read_pages(pages);
    read_links(links);

    /*標準出力からのID検索*/
    search_ID(start_page, goal_page, start_ID, goal_ID, pages);

    /*幅優先探索*/
    bool found = bfs_goal_ID(start_ID, goal_ID, previous_ID, links);
    /*見つからなかったときの処理*/
    if (!found){
        std::cout << "No Connection" << std::endl;
        return 0;
    }

    /*ルートを逆戻り*/
    reverse_route(start_ID, goal_ID, previous_ID, route);

    /*ルートのprint*/
    print_route(route, pages);
    
    return 0;

}
