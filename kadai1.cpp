#include <string>
#include <vector>
#include <iostream>
#include <cassert>

struct _tokens{
    std::string type;
    double number;
};

//tokenの一番初めに+を入れる
void insert_plus_initialize(std::vector<_tokens> &tokens, int &token_index){
    tokens[0].type = "plus";
    token_index++;
}

//与えられた文字cが数字を表す場合trueを返す
bool is_digit(char c){
    if (c>='0'&&c<='9') return true;
    else return false;
}

//構造体tokens[token_index]にline[line_index]以降に入っている数字を一つ入れて、読み終わった次の各indexまでindexを更新する
void read_number(std::vector<_tokens> &tokens, int &line_index, int &token_index, std::string line){
    double number = 0;
    while (line_index < (int)line.size()&&is_digit(line[line_index])){
        number = number*10+line[line_index]-'0';
        line_index++;
    }
    if (line_index < (int)line.size()&&line[line_index]=='.'){
        line_index++;
        double decimal = 0.1;
        while (line_index < (int)line.size()&&is_digit(line[line_index])){
            number += (line[line_index]-'0')*decimal;
            decimal *= 0.1;
            line_index++;
        }
    }
    tokens[token_index].type = "number";
    tokens[token_index].number = number;
    token_index++;
}

//+が入力された時に構造体tokensにその旨を入れる
void read_plus(std::vector<_tokens> &tokens, int &line_index, int &token_index){
    tokens[token_index].type = "plus";
    token_index++;
    line_index++;
}

//-が入力された時に構造体tokensにその旨を入れる
void read_minus(std::vector<_tokens> &tokens, int &line_index, int &token_index){
    tokens[token_index].type = "minus";
    token_index++;
    line_index++;
}

//*が入力された時に構造体tokensにその旨を入れる
void read_multiply(std::vector<_tokens> &tokens, int &line_index, int &token_index){
    tokens[token_index].type = "multiply";
    token_index++;
    line_index++;
}

//'/'が入力された時に構造体にtokensにその旨を入れる
void read_divide(std::vector<_tokens> &tokens, int &line_index, int &token_index){
    tokens[token_index].type = "divide";
    token_index++;
    line_index++;
}

//文字列lineから数字、記号に分割しtokensに入れる
void tokenize(std::vector< _tokens> &tokens, std::string line, int &line_index, int &token_index){
    while(line_index<(int)line.size()){
        if (is_digit(line[line_index])){
            read_number(tokens, line_index, token_index, line);
        }else if (line[line_index]=='+'){
            read_plus(tokens, line_index, token_index);
        }else if (line[line_index]=='-'){
            read_minus(tokens, line_index, token_index);
        }else if (line[line_index]=='*'){
            read_multiply(tokens, line_index, token_index);
        }else if (line[line_index]=='/'){
            read_divide(tokens, line_index, token_index);
        }else{
            std::cout << "Invalid character found: " << line[line_index] << std::endl;
            assert(0);
        }
    }
}
//与えられたindexの一つ前のtokensに与えられたnumberをいれて、indexとindex+1番目を削除してその後ろを詰める処理をする
void two_shorten_tokens(std::vector<_tokens> &tokens, const int index, const double number, int &token_size){
    tokens[index-1].number = number; //3要素が1要素になる
    token_size -= 2;
    tokens[index].type ="";
    tokens[index+1].type="";
    int new_index = index; //詰めるインデックス
    while(new_index<token_size){
        tokens[new_index].type = tokens[new_index+2].type;
        tokens[new_index].number = tokens[new_index+2].number;
        new_index++;
    }
}

//tokensから*と/を処理しtokensを書き換える
void evaluate_multiply_divide(std::vector<_tokens> &tokens, int &token_size){
    int index = 1;
    while(index<token_size){
        if (tokens[index].type=="multiply"){
            double number = tokens[index-1].number*tokens[index+1].number;
            two_shorten_tokens(tokens, index, number, token_size);
            index--;
        }else if (tokens[index].type=="divide"){
            if (tokens[index+1].number==0){ //ゼロ徐算エラー
                std::cout << "divide by zero" << std::endl;
                assert(0);
            }
            double number = tokens[index-1].number/tokens[index+1].number;
            two_shorten_tokens(tokens, index, number, token_size);
            index--;
        }
        index++;
    }
}


//tokensから+答えを一つの数字に変換して返す
double evaluate(std::vector<_tokens> &tokens, int &token_size){
    evaluate_multiply_divide(tokens, token_size);
    double answer = 0;
    int index = 1;
    while (index < token_size){
        if(tokens[index].type=="number"){
            if (tokens[index-1].type=="plus"){
                answer += tokens[index].number;
            }else if (tokens[index-1].type == "minus"){
                answer -= tokens[index].number;
            }else{
                std::cout << "Invalid syntax" << std::endl;
                assert(0);
            }
        }else if (tokens[index].type=="plus"||tokens[index].type=="minus"){
            index++;
            continue;
        }else{
            std::cout << "Invalid syntax" << std::endl;
            assert(0);
        }
        index++;
    }
    return answer;
}




int main(){
    int query_num;
    std::cin >> query_num; //クエリの数
    std::vector<double> answer(query_num); //各クエリの答えを格納する
    for (int i=0; i<query_num; i++){
        std::string line;  //標準入力
        std::cin >> line;
        std::vector<_tokens> tokens(line.size()+1);
        int line_index = 0; //lineのどこを処理しているかを示す
        int token_index = 0; //tokenのどこに値を入れるべきかを示す
        insert_plus_initialize(tokens, token_index);  //先頭に+を入れる
        tokenize(tokens, line, line_index, token_index);  //tokensに文字のタイプを入れる。数字はint型に変換する
        answer.at(i) = evaluate(tokens, token_index);  //tokensからanswerを導く
    }
    for (int i=0; i<query_num; i++){
        std::cout << answer.at(i) << std::endl;
    }
}
