# 前回・前々回学んだこと  
## 配列を削除する機能  
例えば3番目以降を削除するなら  
```
#include <iostream>
#include <vector>
#include <algorithm>

int main(){
	std::vector<int> array = {1,2,3,4,5};
	array.erase(std::remove_if(array.begin(), array.end(), [](int i) {return i>=3; }), array.end());
	for (int i=0; i<(int)array.size(); i++){
		std::cout << array.at(i) << " ";
	}
	std::cout << std::endl;
}
```
##  最適化オプション  
通常  
```$g++ task1.cpp -o task1```
最適化オプション、前回のファイルの実行時間が約1/5になりました。  
```$g++ task1.cpp -o task1 -O2```
調べたところ、実行時間は短くなるが、コンパイル時間が長くなるとのことでした。よって、実行時間が非常に長いもののときのみ最適化オプションをつける
ことが適しています。  
## エラーの出力  
ファイルの読み込み時には必ずファイルが正常に読み込まれているか(ex.データを受け取った配列が空になっていないか)ということをチェックする。  
## 参照渡し  
```std::map<std::string, std::string>& pages```
&の後にスペースを入れることが多い。  
## void関数のreturnは必要な時だけ  
void関数でreturnは不必要ならコードが冗長になるので書かない。
## mapにconstをつけて関数の引数としたいとき  
findとiter->secondを組み合わせるとconstをつけてもエラーにならない。予期せぬ書き換えを防ぐためにも、積極的に使っていこう。
```
#include <iostream>
#include <map>
#include <string>

void search_key (const std::map<std::string, int>& fruit_map, std::string key){
	auto iter = fruit_map.find(key);
	if (iter == fruit_map.end()) {
		std::cout << "not exist" << std::endl;
  		return;  // 見つからなかった
	}
	std::cout << iter->second<< std::endl;
}

int main(){
	std::map<std::string, int> fruit_map;
	fruit_map["apple"] = 1;
	fruit_map["banana"] = 2;
	search_key(fruit_map, "banana");
	search_key(fruit_map, "orange");
}
```
# task1.cpp  
## コードを記述しながら考えたこと  
### long double VS double  
どちらも8byte型なのでメモリに関しては考えなくてよさそうでした。  
#### doubleの良いところ  
long doubleより文字数が少ないので、コードが冗長になりにくい。  
#### long doubleの良いところ  
doubleよりも正確  
例えば`input_0.csv`ファイルの最後の行を小数点以下20桁でprintすると  
`150.17533883877582,   25.512728869805677`元のデータ  
`150.17533883877581502,25.512728869805677334`double  
`150.17533883877582,   25.512728869805676999`long double  
とlong doubleの方が元のデータに近い値となる。  
#### 今回選んだのは  
double型を選びました。元のデータの桁数を考慮するとdouble型でも十分表せると考えたためです。  
### fileの読み込みのエラーの記述  
read_file関数ではファイル関連のエラーで余計なデバッグ時間を取らせないために、できるだけエラー出力を記述しようと思いました。try-catch文で数字以外の
データが読み込まれたらエラーを出力しようと思いましたが、","がないだけだったり数字の中に文字"a"などが紛れているだけの時にはエラーを拾ってくれないようでした。    
また、毎行きちんと数字が読み込まれているか確認することに関して、今回はデータ数が少ないので問題なさそうですが、ファイルのデータが増えてくると実行時間に影響が出てしまい、毎行読み込むのは無駄なのかもしれないとも思いました。
## コードの実行方法  
以下のようなディレクトリ構成にします。N=5の時の実行方法です。
```
├── input_0.csv
├── output_0.csv
└── task1.cpp
```
コンパイルします  
```
$g++ task1.cpp -o task1 -O2
```
実行します。input_1.csvのときは0のところを1に変えてください。  
```
$./task1 0
```
どのcsvファイルでも1分かからず実行が終了します。  
## 方針  
貪欲法で都市をめぐる順番を決めた後、交差している場所を見つけて経路をつなぎ変えました(2-opt法)。もう一工夫欲しいと思い、貪欲法の中において、スタートとなる都市を全てのパターン(N回)で試し、もっとも総距離が短くなる都市をスタートに選ぶことにしました。
| データファイル | 貪欲法のみ |2-optと貪欲法 |2-optでswap時j=0 |貪欲法に一工夫+2-opt |
| ------------- | ------------- |------------- |------------- |------------- |
| input_0.csv  | 3418.1 m  |3418.1 m  |3418.1 m  |3418.1 m  |
| input_1.csv  | 3832.29 m  |3832.29 m  |3832.29 m  |3832.29 m  |
| input_2.csv  | 5449.44 m |4974.5 m |4974.5 m |4937.18 m |
| input_3.csv  | 10519.16 m  |10051.35 m |10051.35 m |8868.93 m |
| input_4.csv  | 12684.06 m  |12140.74 m |12140.74 m |11633.86 m |
| input_5.csv  | 25331.84 m  |92322.33 m |23109.46 m |22765.78 m |
| input_6.csv  | 49892.05 m  |70158.98 m |45596 m |870894.9 m |
