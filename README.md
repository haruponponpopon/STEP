# 今回気をつけるべきこと
'*'や '&'の前後に空白を入れて見やすくする  
関数内で不変の時はconstをつけて値変を防ぐ  
順番はleftからright  
返り値を負にしない。基本1?  
乱数を毎回初期化せず、最初に一回して使い回す。  
配列が初期化されていないといけない時はコメントで明示していく  
macroで定数を宣言しない`constexpr size_t hash_size = 100;`とmain関数で宣言する。  
c++はtypedefは使われないことが多い  
dequeueやenqueueはエラーチェックを怠らない  
## 大規模なプログラムの一部を書いているつもりでコードを記述する  
今まで長いコードを書くことがあまりなかったのでconstもエラー処理も記述するだけ時間とスペースの無駄と思っていたが、今回エラー処理を記述していたおかげで、デバッグに要した時間が段違いに短くなった。バグを未然に防ぐことも重要だが、バグの早期発見を可能にするコードを書くことも重要に感じた。segmentation faultが起きなければ上々だと思っていたが、バグがあるのに動いてしまうコードが一番厄介であることに気がついた。
## 課題1,2
kadai1.cppとkadai1_2.mdをご覧ください  
## 課題3,4  
バックアップのためにあげています。一応バグは消えましたが、まだ途中です。
