#include <string> 
#include <vector>
#include <iostream> 
#define hash_size 100
#define max_overlap 10
#define cache_size 3

int hash(std::string URL);
void access_page(std::string URL, std::string web);
void add_hash(std::string URL, std::string web);
void enqueue(std::string URL);
std::string dequeue();
void print_queue();
void print_hash_table();
void remove_hash();

typedef struct _hash_table{
    std::string URL;
    std::string web;
    int overlap;
} Hash;
std::vector<std::vector<_hash_table>> hash_table(hash_size);  //ハッシュテーブルの配列
std::string queue[cache_size+max_overlap];  //キューの配列
int tail = 0, head = 0; // キューの要素区間を表す変数
int cache_count=0;  //キャッシュの数
int overlap_count=0;  //キューの中で重複している数


//ハッシュ値を返す。URLは英語の小文字のみからなると仮定
int hash(std::string URL){
    int hash_value=0;
    for (int i=0; i<(int)URL.size()-4; i++){
        hash_value+=URL[i]-'a';
        hash_value%=hash_size;
    }
    return hash_value;
}
//新たにハッシュテーブルに追加する
void add_hash(std::string URL, std::string web){
    Hash new_hash;
    new_hash.URL = URL;
    new_hash.web = web;
    new_hash.overlap = 1;
    hash_table[hash(URL)].push_back(new_hash);
}
//キューからキャッシュを一つ削除する
void remove_hash(){
    bool finish = false;
    while (!finish){
        std::string remove_URL = dequeue();
        int remove_hash_value = hash(remove_URL);
        for (int i=0; i<(int)hash_table[remove_hash_value].size(); i++){
            if(hash_table[remove_hash_value][i].URL==remove_URL){
                hash_table[remove_hash_value][i].overlap--;
                if (hash_table[remove_hash_value][i].overlap==0){
                    std::cout << remove_URL << " was removed from chache." << std::endl;
                    finish=true;
                }else{
                    overlap_count--;
                }
            }
        }
    }
}
//ページにアクセスした時の処理
void access_page(std::string URL, std::string web){
    int hash_value = hash(URL);
    bool cache=false;
    for (int i=0; i<(int)hash_table[hash_value].size(); i++){
        if (hash_table[hash_value][i].URL==URL){
            cache=true;
            hash_table[hash_value][i].overlap++; //overlapを一増やす
            enqueue(URL); //キューに追加
            if (hash_table[hash_value][i].overlap>1) overlap_count++;  //キャッシュに入っている時
            else {   //キャッシュに入っていない時
                remove_hash();
                std::cout << URL << " was added to cache." << std::endl;
            }
            break;
        }
    }
    if (!cache){ //今までに調べたことがなかったとき
        add_hash(URL, web);  //キャッシュが見つからなかったらハッシュテーブルに追加する
        enqueue(URL); //キューに追加
        if (cache_count==cache_size){
            remove_hash();
        }else{
            cache_count++;
        }
        std::cout << URL << " was added to cache." << std::endl;
    }
}
//デバッグ用、ハッシュテーブルの中身を表示する
void print_hash_table(){
    std::cout << "current hash: " << std::endl;
    for (int i=0; i<hash_size; i++){
        for (int j=0; j<(int)hash_table[i].size();i++){
            std::cout << ":" << hash_table[i][j].URL << " " << hash_table[i][j].overlap << std::endl;
        }
    }
}
int main(){
    access_page("abc.com", "abc");
    std::cout << cache_count << " " << overlap_count << std::endl;
    access_page("acb.com", "ringo");
    std::cout << cache_count << " " << overlap_count << std::endl;
    access_page("pen.com", "pen");
    std::cout << cache_count << " " << overlap_count << std::endl;
    access_page("pen.com", "pen");
    std::cout << cache_count << " " << overlap_count << std::endl;
    access_page("cab.com", "book");
    std::cout << cache_count << " " << overlap_count << std::endl;
    access_page("acb.com", "ringo");
    std::cout << cache_count << " " << overlap_count << std::endl;
    //print_queue();
    access_page("fish.com", "fish");
    std::cout << cache_count << " " << overlap_count << std::endl;
    access_page("abc.com", "abc");
    std::cout << cache_count << " " << overlap_count << std::endl;
    //print_hash_table();
}


void enqueue(std::string URL) {
    queue[tail++] = URL;
    if (tail == cache_size+max_overlap) tail = 0; // リングバッファの終端に来たら 0 に
}
std::string dequeue() {
    std::string URL = queue[head];
    ++head;
    if (head == cache_size+max_overlap) head = 0;
    return URL;
}

//デバッグ用 キューの中身を表示する
void print_queue() {
    std::cout << "current queue: ";
    if (head<tail){
        for (int i=head; i<tail; i++){
            std::cout << queue[i] << " ";
        }
    }else{
        for (int i=head; i<max_overlap+cache_size; i++){
            std::cout << queue[i] << " ";
        }
        for (int i=0; i<tail; i++){
            std::cout << queue[i] << " ";
        }
    }
    std::cout << std::endl;
}
