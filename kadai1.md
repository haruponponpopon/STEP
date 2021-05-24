# 課題1  
## main関数  
一番最初にクエリの数を読み込む  
文字の種類を構造体に入れていく  
数字はnumber  
+はplus  
-はinus  

## テストケース  
[テストケース](https://docs.google.com/document/d/1Mj_tx71ruQhekDrBvhUV_xXzLySPP28A4jCZP2lU_iQ/edit?usp=sharing)
 
## つまづいてしまったところ  
tokenize関数にlineを渡す時、constをつけて値変を防ごうとしたらconst std::string型からchar型には変換できなかった。  
文字を数字に変換('1'を1に)する関数を作るべきかと思ったが良い関数名が思い浮かばず、関数を使わずに記述した。  
read_plus関数内で構造体のnumberにアクセスした時にエラーを検知できるような何かを代入すべきかと思ったが、何を代入すれば良いかわからなかった。    
read_plus関数内で変数line_indexは直接の処理には使われず値を渡すべきか迷ったが、line_indexの値はread_XX関数内で処理するで統一したほうがわかりやすいと思い値を渡すことにした。
