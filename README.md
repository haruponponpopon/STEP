# week2  
## 前回からの改善点  
#### 動けばいいから見やすいコードへ  
競プロでコードを書くことが多く、コードを早く書くことばかり考えて見やすさが疎かになっていた。  
用途に合わせて書き方を変えていけるようにしたい。  
#include <bits/stdc++.h> -> 用途に合わせてライブラリをインポートする  
rep や　using namespace std -> for文、std::cout など省略せず書く  
一文字の関数名 -> 何をしているか一目でわかる関数名  
使わなかった変数名はそのまま -> 使わなかった変数名は消す
  
main関数内をできるだけ簡潔に記述することを意識した。  


## 課題1  
matrix.cppをご覧ください。  
#### main関数  
標準入力から行列のサイズを受け取りNとする。

    initialize(matrix1,N);
    initialize(matrix2,N);

    double begin = get_time();

    mul(matrix1, matrix2, ans, N);

    double end = get_time();

    print_equation(matrix1, matrix2, ans, N);
    
matrix1*matrix2=ans という行列積を考える。initialize関数で初期化し、mul関数で行列積を計算しansに代入し、print_equation関数でprint。
行列積の計算時間を計測したいので、mul関数の前後に時間を計測するbeginとendをもってくる。  
#### initialize関数  
行列のポインタを受け取り、各要素に1から3までの乱数を代入する。  
#### mul関数  
二つの行列と答えを入れる配列を受け取り、行列積を答えを入れる配列に代入する。  
#### print_equation関数  
行列の計算式をprintする関数。  
答えをprintするときに要素の桁数によって表示した時に見栄えが悪くなってしまったので桁数に応じてスペースをいれることで対応した。
space配列には行列の各行の最大の桁数が入る。これを答えをprintする関数内で行わなかったのは、計算時間削減のためである。  
行ごとにfor文でprintしていく。  
matrix1をprintし、大体真ん中(N/2と等しい)の行であった時に*マークをprintし、matrix2をprintし、大体真ん中(N/2と等しい)の行であった時にイコールをprintし答えをprintする。 
#### print_matrix関数  
一行分の左のカッコをprintし、要素をprintし右のカッコをprintする。printするカッコの種類はprint_left_frame、print_right_frameより指示をもらう。  
#### print_right_frame関数 print_left_frame関数  
一番上の行や一番下の行であった時、`/`や`\`で全体で大きなカッコになるようにする。それ以外の時は`|`をprintする。　　
#### print_matrix_ans関数  
行列積を桁数に応じてスペースをいれながらprintする関数。

　　　　　for (int k=0; k < space.at(j)-calc_digit(matrix[i][j]); k++){
            std::cout << " ";
        }
        
このコードでスペースを補填している。  
#### max_digit関数  
print_equation関数内でspace配列を作る時に用いる関数。各行の最大値を調べ、その桁数を配列にいれて返す。  
#### calc_digit関数  
与えられた数字の桁数を返す関数。0は今回は出現しないので考えられていない。  
