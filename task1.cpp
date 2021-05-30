#include<map>
#include<string>
#include<set>
#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <queue>

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
    if (already_searched!=2){
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


int main(){
    /*変数定義*/
    std::map<std::string, std::string> pages;
    std::map<std::string, std::set<std::string>> links;
    std::map<std::string, int>used; //<ページ番号, 未使用0使用済1>
    std::map<std::string, std::string> previous_ID; //そのページの一つ前のページのID
    std::string start_page; //起点となる言葉
    std::string goal_page; //探したい言葉
    std::string start_ID;
    std::string goal_ID;
    vector<std::string> route;  //startからgoalまでの道のりに出てくるPageのIDをしまう


    /*標準出力からの受け取り*/
    std::cin >> start_page >> goal_page;

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
    // std::cout << pages["33"] << std::endl; //ID33のページ名表示
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

    
    search_ID(start_page, goal_page, start_ID, goal_ID, pages);
    // std::cout << start_ID << " " << goal_ID << std::endl;
    bool found = bfs_goal_ID(start_ID, goal_page, previous_ID, links);

    if (!found){
        std::cout << "No Connection" << std::endl;
        return 0;
    }

    
    std::string current_ID= goal_ID;
    std::cout << pages[current_ID] << " ";
    while(1){
        current_ID = previous_ID[current_ID];
        std::cout << pages[current_ID] << " ";
        if (current_ID==start_ID) break;
    }
    return 0;

}
