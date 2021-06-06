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
##  配列のサイズを取得  
