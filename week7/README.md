# 課題1  
[予想](https://docs.google.com/document/d/1aYkRgIqQD0vaGqTiVQC_nJeYk3RjrZ9RIKulBvp5ABg/edit?usp=sharing)  
行列の大きさ512で計測  
|ループ順序|1回目(sec)|2回目(sec)|3回目(sec)|平均(sec)|
|--:|--:|--:|--:|--:|
|i-k-j|2.33|2.30|2.24|2.29|
|k-i-j|2.18|2.33|2.45|2.32|
|i-j-k|2.92|2.96|2.94|2.94|
|j-i-k|2.92|2.92|2.94|2.93|
|k-j-i|3.18|3.22|3.14|3.18|
|j-k-i|3.40|3.36|3.42|3.39|  

実行時間が早い順に<i,k,j>と<k,i,j>、<i,j,k>と<j,i,k>、<j,k,i>と<k,j,i>であるという予想は当たっていた。  
<i,k,j> < <k,i,j> << < <i,j,k> < <j,i,k> << < <k,j,i> < <j,k,i>　という予想は有意な差があるとは言えなさそうだった。  
# 課題2  
インタプリタ言語は構文木構造でaを配列としたとき、a[0]とa[1]は全くの別物として扱われ、隣り合ったメモリに確保される保証がない。よって行列の時も隣の数字がキャッシュに保存されている保証はなく、どのようにアクセスしても実行時間に変わりはなくなる。  
仮にインタプリタ言語もa[0]とa[1]が隣り合ったメモリに確保されキャッシュされるとする。  
インタプリタ言語はfor文の中を構文木で分解して一つ一つ見ていくので、コンパイラ言語と違って計算が最適化されていない。よって実行にものすごく時間がかかる。  
授業スライドによるとC++では1.8秒のところPythonでは18分であった。故に、メモリアクセスしたときのキャッシュによって削減される実行時間が微々たる量になる
。例えばC++にとっての1秒は結構大きい(55.6%)が、Pythonでは微々たる量(0.09%)である。よってインタプリタ言語ではi,k,jを入れ替えても実行時間はほとんど変わらない。
# 課題3  
## greedy法、2-opt法(TSP.cpp)    
### 方針
前回の滅茶苦茶になってしまったコードを改良した。  
2-opt関数の中身を変更し、以前は交差しているかを調べていたが、交差を反転させた時に距離が短くなるかどうか(swap_makes_shorter関数)を用いて、trueのときに交差を入れ替えるとした。また、交差が全て取り除かれるまでwhile文で交差の反転を繰り返した。  
その後、更なる改良を加えるため、greedy法を一番近い都市を選んでいた(choose_nearest_city関数)のを、2つの都市を訪れたときに最も距離が近くなる2都市のうち1番近い都市を入れる(choose_nearest_city2関数)、2つの都市を訪れたときに最も距離が近くなる2都市をセットで入れる(choose_nearest_city3関数)を実装してみた。順にgreedy1,2,3とする。    
### 実行方法  
ほとんどが1分以内に終わります。  
csvファイルが7のときは30分かかります。  
greedy1
```
$g++ insert.cpp -O2
$./a.out ファイル番号 1
(例)
$./a.out 0 1
```
greedy2
```
$g++ insert.cpp -O2
$./a.out ファイル番号 2
(例)
$./a.out 3 2
```
greedy3
```
$g++ insert.cpp -O2
$./a.out ファイル番号 3
(例)
$./a.out 6 3
```
### 実行結果  
|csvファイル|greedy1と2-opt(m)|greedy2と2-opt(m)|greedy3と2-opt(m)|
|--:|--:|--:|--:|
|0|3418.1|3418.1|3418.1|
|1|3832.29|3778.72 |3778.72|
|2|4994.89|4931.44|5108.65|
|3|8705.44|8480.51|8490.69|
|4|11311.56|12021.42|10934.41|
|5|21726.21|22425.38|22141.31|
|6|42523.97|44298.44|43822.63 |
|7|84033.15|87843.81|87021.7| 

## insert法(insert.cpp)   
greedy法を改善?しても全然結果が良くなりません!  
greedy法をこれ以上いじると実行時間が大変なことになりそうなので、全く方針を変えて見ました。  
### 方針
まず二つの都市を決めて、ループさせる。その後に他の都市を順番に一番経路が近くなるような場所に順次挿入していく。  
<img width="682" alt="スクリーンショット 2021-06-29 9 10 01" src="https://user-images.githubusercontent.com/72332745/123718311-c9bba780-d8b9-11eb-8851-53f7ef73dbd0.png">  
配列に都市を挿入すると後ろの数字を全部ずらさないといけないので、計算時間を考慮して訪問都市の順序の管理には線形リストを用いることとした。
```
struct node{
    struct node *next;
    int city;
    double distance_to_next;
};
```
まず1つ目の都市0と2つ目の都市のループを考えて、その後に他の都市を全ての場所に挿入して距離を計算し、一番短くなったところを訪問順とした。なお、都市数が1以下の時にはこのコードは動かない。  
### 実行方法  
5秒以内に終わります。  
線形リストに挿入する都市を0,1,2,3の順にする時  
```
$g++ insert.cpp -O2  
$./a.out ファイル番号 1
例えば
$./a.out 1 1
```
線形リストに挿入する都市を都市0から近い順にする時  
```
$g++ insert.cpp -O2
$./a.out ファイル番号 2
```
線形リストに挿入する都市を都市0から遠い順にする時  
```
$g++ insert.cpp -O2
$./a.out ファイル番号 (1,2以外の文字)
```
今回工夫した点として、どれを選んだかがターミナル上に表示されるようになっているので、間違えた時に気づけます。  
### 実行結果  
0から順番に入れるのが一番良いという結果になりました。近い順、遠い順にするとなぜか交差してしまいます。  
都市数が少ない時はgreedy法より良い結果が出せるのですが、都市数が増えるとそうもいきませんでした…
|csvファイル|0から順番(m)|0から近い順(m)|0から遠い順(m)|
|--:|--:|--:|--:|
|0|3291.62|3291.62|3418.1|
|1|3778.72|3778.72 |4171.88|
|2|4670.27|4832.65|5368.58|
|3|8621.01|10121.34 |10121.17|
|4|11555.35|13068.43|13635.49|
|5|22451.41|26086.62|26223.15|
|6|44556.49|51823.76|51992.43|
|7|88790.8|104178.58|104684.27| 
