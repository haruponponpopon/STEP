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
## 詰まったところ  
read_file関数ではファイル関連のエラーで余計なデバッグ時間を取らせないために、できるだけエラー出力を記述しようと思いました。try-catch文で数字以外の
データが読み込まれたらエラーを出力しようと思いましたが、","がないだけだったり数字の中に文字"a"などが紛れているだけの時にはエラーを拾ってくれないようでした。  
