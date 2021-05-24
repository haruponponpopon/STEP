# 課題1  *と/に対応した電卓の実装
## struct _tokens  
文字の種類をいれる構造体   
文字が数字だったらnumberに数字を入れて、文字の種類をtypeにいれる。typeには数字ならnumber、+ならplus、-ならminus、*ならmultiply、/ならdivideが入る。　　

## main関数  
一番最初にクエリの数を読み込む     
その後for文でクエリの数だけ答えを計算し、それをanswerに格納する　  　
for文では一つのクエリをstring型のlineに読み込み、lineから構造体に入れていくための構造体tokensを宣言する。    
insert_plus_initialize関数で構造体tokensの0番目に+をいれる。(のちの処理をしやすくするため)    
tokenize関数でlineを構造体tokensに変換する。    
evaluate関数で構造体tokensから答えとなる数字が返ってくるので、それをanswer配列に格納する。    
## insert_plus_initialize関数  
構造体tokensの0番目に"plus"を入れる。次に何かを入れることになったらtokensの1番目に値を入れるべきなのでtoken_indexを1増やす。  
## tokenize関数  
lineを先頭から読み込んでいき、数字だったらread_number関数を呼び出し、+だったらread_plus関数を呼び出し、-だったらread_minus関数を呼び出し、
*だったらread_multiply関数を呼び出し、/だったらread_divide関数を呼び出し、tokensに適切な処理を加える。それ以外だったらerrorを返す。  
## read_number関数  
while文で数字以外のものが読み込まれるまでlineの指定されたindexから文字を読み込む。それを数字に変換しnumberにいれる。数字が例えば2文字読み込まれたら二桁の数になるように処理を施す。文字が一つ読み込まれるたびにline_indexを一増やし、次に読み込むlineの場所を示すようにする。数字が全て読み込まれた後、もし数字以外のものが小数点だったら次のwhile文に移行し、数字が読み込まれるたびに桁が下がるように処理を施しながらnumberにいれる。指定された構造体tokensの場所に、typeには'number'、数字には処理されたnumberをいれる。  
## read_plus関数、read_minus関数、read_multiply関数、read_divide関数  
指定された記号(plus,minus,multiply,divide)を構造体tokensの指定された場所に入れる。  
## evaluate関数  
乗除算を先に計算して構造体tokensの中身を書き換えてくれる関数evaluate_multiply_divideを呼び出す。戻り値であるanswerに0を代入する。構造体tokensを1番目から読み込んでいき(先頭には+が入っている)、数字であったら、一つ前の符号が+か-かを確認し、answerに構造体の中のnumberを加減算する。構造体にminusかplus以外のものが入っていたら、どこかがバグっているということなのでエラーを返す。構造体のtypeはnumberかminusかplusのいずれかなので、それ以外のものが入っていたら、バグなのでエラーを返す。  
## evaluate_multiply_divide関数  
例えば2+4*4を2+16にする役割を持つ。  
構造体tokensを先頭から読み込んでいき、typeがmultiplyがdivideだったらnumberにその記号の前後の数字を乗除算した値を入れて、構造体tokensにnumberをいれて、不要になった二箇所のtokensの要素を消して前に詰めてくれる関数two_shorten_tokensを呼び出す。  
## two_shorten_tokens関数  
構造体tokensの指定されたindex(*や/の場所)のひとつ前に乗除算結果を示すnumberをいれる。tokenのサイズが二つ減少するのでtoken_sizeを二つ減らす。値を残しておくのは良く無いので、構造体tokensのindex番目とindex+1番目のtypeから文字列を消し去る。その後numberを入れた後に残っている構造体の中身を二つ詰める処理をする。  

## テストケース  
[テストケース](https://docs.google.com/document/d/1Mj_tx71ruQhekDrBvhUV_xXzLySPP28A4jCZP2lU_iQ/edit?usp=sharing)
 
## つまづいてしまったところ  
tokenize関数にlineを渡す時、constをつけて値変を防ごうとしたらconst std::string型からchar型には変換できなかった。  
文字を数字に変換('1'を1に)する関数を作るべきかと思ったが良い関数名が思い浮かばず、関数を使わずに記述した。  
read_plus関数内で構造体のnumberにアクセスした時にエラーを検知できるような何かを代入すべきかと思ったが、何を代入すれば良いかわからなかった。    
read_plus関数内で変数line_indexは直接の処理には使われず値を渡すべきか迷ったが、line_indexの値はread_XX関数内で処理するで統一したほうがわかりやすいと思い値を渡すことにした。
