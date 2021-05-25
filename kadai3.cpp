#include <string>
#include <vector>
#include <iostream>
#include <cassert>

struct _tokens{
    std::string type;
    double number;
};

//tokenの指定された場所(insert_plus_index)に+を入れて後ろを一つずつずらす
void insert_plus_initialize(std::vector<_tokens> &tokens, int &token_size, const int insert_plus_index){
    int token_index = token_size;
    while(token_index>insert_plus_index){
        tokens[token_index].type = tokens[token_index-1].type;
        tokens[token_index].number = tokens[token_index-1].number;
        token_index--;
    }
    tokens[insert_plus_index].type = "plus";
    tokens[insert_plus_index].number = 0;
    token_size++;
}

//与えられた文字cが数字を表す場合trueを返す
bool is_digit(char c){
    if (c>='0'&&c<='9') return true;
    else return false;
}

//構造体tokens[token_index]にline[line_index]以降に入っている数字を一つ入れて、読み終わった次の各indexまでindexを更新する
void read_number(std::vector<_tokens> &tokens, int &line_index, int &token_size, std::string line){
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
    tokens[token_size].type = "number";
    tokens[token_size].number = number;
    token_size++;
}

//+が入力された時に構造体tokensにその旨を入れる
void read_plus(std::vector<_tokens> &tokens, int &line_index, int &token_size){
    tokens[token_size].type = "plus";
    token_size++;
    line_index++;
}

//-が入力された時に構造体tokensにその旨を入れる
void read_minus(std::vector<_tokens> &tokens, int &line_index, int &token_size){
    tokens[token_size].type = "minus";
    token_size++;
    line_index++;
}

//*が入力された時に構造体tokensにその旨を入れる
void read_multiply(std::vector<_tokens> &tokens, int &line_index, int &token_size){
    tokens[token_size].type = "multiply";
    token_size++;
    line_index++;
}

//'/'が入力された時に構造体tokensにその旨を入れる
void read_divide(std::vector<_tokens> &tokens, int &line_index, int &token_size){
    tokens[token_size].type = "divide";
    token_size++;
    line_index++;
}

//'('が入力された時に構造体tokensにその旨を入れる
void read_opened_parenthesis(std::vector<int> &start_point, int &line_index, const int end_parameter){
    start_point.push_back(end_parameter);
    line_index++;
}

//')'が入力された時に構造体tokensにその旨を入れる
void read_closing_parenthesis(int &end_parameter, std::vector<std::vector<int>> &calculate_range, std::vector<int> &start_point, int &line_index){
    int start = start_point.back();
    start_point.pop_back();
    calculate_range[0].push_back(start);
    calculate_range[1].push_back(end_parameter);
    end_parameter = start+1;
    line_index++;
}

//文字列lineから数字、記号に分割しtokensに入れる
void tokenize(std::vector<std::vector<int>> &calculate_range, std::vector< _tokens> &tokens, std::string line, int &token_size){
    int line_index = 0; //lineのどこを処理しているかを示す
    int end_parameter=0;
    std::vector<int> start_point; //開始位置をいれるスタックとする
    while(line_index<(int)line.size()){
        if (is_digit(line[line_index])){
            read_number(tokens, line_index, token_size, line);
            end_parameter++;
        }else if (line[line_index]=='+'){
            read_plus(tokens, line_index, token_size);
            end_parameter++;
        }else if (line[line_index]=='-'){
            read_minus(tokens, line_index, token_size);
            end_parameter++;
        }else if (line[line_index]=='*'){
            read_multiply(tokens, line_index, token_size);
            end_parameter++;
        }else if (line[line_index]=='/'){
            read_divide(tokens, line_index, token_size);
            end_parameter++;
        }else if (line[line_index]=='('){
            read_opened_parenthesis(start_point, line_index, end_parameter);
        }else if (line[line_index]==')'){
            read_closing_parenthesis(end_parameter, calculate_range, start_point, line_index);
        }else{
            std::cout << "Invalid character found: " << line[line_index] << std::endl;
            assert(0);
        }
    }
    calculate_range[0].push_back(0);
    calculate_range[1].push_back(end_parameter);
}
//与えられたindexの一つ前のtokensに与えられたnumberをいれて、indexとindex+1番目を削除してその後ろを詰める処理をする
void two_shorten_tokens(std::vector<_tokens> &tokens, const int index, const double number, int &token_size){
    tokens[index-1].number = number; //3要素が1要素になる
    token_size -= 2; //2つ要素が減るのでtokensのサイズも2減る
    tokens[index].type ="";  //初期化しておく
    tokens[index+1].type="";
    int new_index = index; //詰めるインデックス
    while(new_index<token_size){
        tokens[new_index].type = tokens[new_index+2].type;
        tokens[new_index].number = tokens[new_index+2].number;
        new_index++;
    }
}

//tokensから*と/を処理しtokensを書き換える
void evaluate_multiply_divide(std::vector<_tokens> &tokens, const int start, int &end, int &token_size){
    int index = start+1; //tokensの今現在参照している場所
    while(index<end){
        if (tokens[index].type=="multiply"){
            double number = tokens[index-1].number*tokens[index+1].number;
            two_shorten_tokens(tokens, index, number, token_size);
            end -= 2; //token_sizeだけ2減らしてしまったが、endもそれに伴って減少するのでここで減らす
            index--;
        }else if (tokens[index].type=="divide"){
            if (tokens[index+1].number==0){ //ゼロ徐算エラー
                std::cout << "divide by zero" << std::endl;
                assert(0);
            }
            double number = tokens[index-1].number/tokens[index+1].number;
            two_shorten_tokens(tokens, index, number, token_size);
            end -= 2;
            index--;
        }
        index++;
    }
}


//tokensから*,-,+,/の処理をして答えを一つの数字に変換して返す
double evaluate_without_parenthesis(std::vector<_tokens> &tokens, const int start, int &end, int &token_size){
    evaluate_multiply_divide(tokens, start, end, token_size);
    double answer = 0;
    int index = start+1; //start番目は+だから飛ばして大丈夫
    while (index < end){     
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

//tokensから()も含めた全ての処理をする
double evaluate(std::vector<_tokens> &tokens, std::vector<std::vector<int>> &calculate_range, int &token_size){
    double answer=0;
    for (int i=0; i<(int)calculate_range[0].size(); i++){
        int start = calculate_range[0][i];
        int end = calculate_range[1][i];
        insert_plus_initialize(tokens, token_size, start);  //指定された場所(start)に+を入れてtokenの中身をずらす
        end++;  //initializeでtoken_index(tokensのサイズを示す)のみ値が更新されているので、endも更新のため一つ増やす
        answer = evaluate_without_parenthesis(tokens, start, end, token_size); //カッコの中身を計算
        tokens[start].number = answer;
        tokens[start].type = "number";
        int slide_range = end-start-1; //前に詰める数
        for (int i=start+1; i<token_size-slide_range; i++){
            tokens[i].number = tokens[i+slide_range].number;
            tokens[i].type = tokens[i+slide_range].type;
        }
        token_size -= slide_range;//tokensのサイズは詰めた分だけ減少
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
        std::vector<_tokens> tokens(line.size()*2); //カッコが多い時大量のplusが挿入されるが最大でstring長/2+1個挿入されると考えられる。例(((3)))
        std::vector<std::vector<int>> calculate_range(2);  //()に対応、計算すべき範囲を順番に入れる配列
        int token_size = 0; //tokensのサイズを示す。tokensの中身を増やせば増加し減らせば減少する。
        tokenize(calculate_range, tokens, line, token_size);  //tokensに文字のタイプを入れる。数字はint型に変換する
        answer.at(i) = evaluate(tokens, calculate_range, token_size);  //tokensからanswerを導く
    }
    for (int i=0; i<query_num; i++){
        std::cout << answer.at(i) << std::endl;
    }
    return 0;
}

