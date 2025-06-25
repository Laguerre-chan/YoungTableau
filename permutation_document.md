# Permutation

c++で置換を扱うためのクラスである．
置換のワード表記を，{1,...,n}を要素とするベクトルとして内部で扱っている．


## メンバ関数（数学感強くないもの）
### コンストラクタ
- `Permutation(vector<int>)`

受け取ったベクトルを置換のワード表記として扱う．受け取った配列が置換になってなかったらエラーになる．

- `Permutation(initializer_list<int>)`

Permutation perm{1,3,2};みたいな感じで置換を定義する．引数を置換のワード表記として扱う．受け取ったものが置換になってなかったらエラーになる．

- `Permutation(int size, int i, int j)`

互換を定義する．サイズがsizeで，iとjだけ入れ替える置換を定義する．
1<= i,j <= sizeであることをチェックし，違反したらエラーになる．


### ゲッター

- `get_entry() const`

置換のワード表記をベクトルとして返す．

返り値：`vector<int>`

- `get_entry(int i) const`

置換をσとすると，σ(i+1)を返す．要するに，内部では置換を`vector<int>`型として扱ってるので，それの第i要素を返してるだけである．

返り値：`int`
### その他便利な関数

- `show() const`

置換をワード表記で表示する．

- `size() const`

置換のサイズを返す．

返り値：`int`

- `show_findstat(int n) const`

`[2,3,1,4] => 2`みたいな感じで表示する関数．
「[置換] => n」という文字列が出力される．置換の統計量を検索するサイトfindstatに入力しやすいように出力するための関数である．

- `show_findmap(const Permutation & other) const`

`[2,3,4,1] => [4,1,2,3]`みたいな感じで出力する関数．
「[自分] => [other]」という文字列が出力される．置換の統計量を検索するサイトfindstatにあるmapfinderに入力しやすいように出力するための関数である．

## メンバ関数（数学感強いもの）
### 演算子
- `operator==(const Permutation&) const`

比較演算子．置換として完全に一致するかを比較する．

返り値：`bool`

- `operator*(const Permutation&) const`

置換の合成．対称群での積を計算する．

返り値：`Permutation`

- `operator<<(const Permutation&) const`

Bruhat順序での被覆関係．x<<yは，yがxをBruhat順序の意味で被覆することを意味する．

返り値：`bool`  

### 統計量

- `inv() const`
- `maj() const`
- `comaj() const`

それぞれ，文字通りの統計量を返す．

返り値：`int`
### 置換への全単射

- `reverse() const`
- `inverse() const`
- `complement() const`

名前の通りである．これらは総称してtrivial bijectionとも呼ばれる．

返り値：`Permutation`

### Subexcedentへの全単射



- `Lehmer() const`

Lehmer codeを出力する．

返り値：`vector<int>` 


# 関連する関数

- `enum_permutation(int size)`

大きさsizeの置換を列挙する関数．

返り値：`vector<Permutation>`
