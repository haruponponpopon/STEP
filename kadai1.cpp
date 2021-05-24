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

//tokensから答えを一つの数字に変換して返す
double evaluate(std::vector<_tokens> &tokens, const int &token_size){

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
        }//else{
        //     std::cout << "Invalid syntax" << std::endl;
        //     assert(0);
        // }
        index++;
    }
    return answer;
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
        }else{
            std::cout << "Invalid character found: " << line[line_index] << std::endl;
            assert(0);
        }
    }
}



int main(){
    std::string line;  //標準入力
    std::cin >> line;
    std::vector<_tokens> tokens(line.size()+1);
    int line_index = 0; //lineのどこを処理しているかを示す
    int token_index = 0; //tokenのどこに値を入れるべきかを示す
    insert_plus_initialize(tokens, token_index);
    tokenize(tokens, line, line_index, token_index);
    double answer = evaluate(tokens, token_index);
    std::cout << answer << std::endl;
}
