*This project has been created as part of the 42 curriculum by kayoshid.*

## Description

`get_next_line(int fd)` は、ファイルディスクリプタから1行ずつ読み取る関数です。
改行があれば改行を含めて返し、末尾に改行がなければ最後の文字まで返します。
EOFで返す文字がない場合、またはエラーが起きた場合は `NULL` を返します。
通常のファイルと標準入力に対応する必須部分を実装しています。

複数のファイルディスクリプタを交互に読むボーナス部分は実装していません。

## Instructions

利用するプログラムから `get_next_line.h` をインクルードし、次のようにコンパイルします。
`main.c` は利用側で用意してください。

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o gnl
```

`-D BUFFER_SIZE=42` を省略した場合も、既定値は42です。
ソースだけをコンパイルする場合は次のコマンドを使えます。

```sh
cc -Wall -Wextra -Werror -c get_next_line.c get_next_line_utils.c
```

`get_next_line(fd)` を `NULL` が返るまで呼び出し、返された行は使用後に
呼び出し側で `free()` してください。標準入力には `fd = 0` を指定します。
同じ入力はEOFまで読み切ってから、次の入力へ切り替えてください。
読み取り途中には、次回用のデータが内部に保持されます。
`BUFFER_SIZE` が0以下の場合は `NULL` を返します。

## Algorithm

1. `get_next_line()` は、`fd` と `BUFFER_SIZE` を確認します。
   `static` 変数の `buffer`、`buffer_length`、`buffer_capacity` に、
   読み取り済みでまだ返していないデータと、その長さ・確保済み容量を保持します。
2. `get_next_line()` が呼ぶ `gnl_read()` は、保存データから改行を探します。
   すでに改行があれば、追加の読み取りをせずに `1` を返します。
3. 改行がなければ、`gnl_read()` は `gnl_reserve()` を呼びます。
   `gnl_reserve()` は `buffer_length + BUFFER_SIZE + 1` バイトの容量があるか確認し、
   不足していれば、必要量と現在の容量の2倍のうち大きい方を `malloc()` で確保します。
   `gnl_copy()` で保存データを新しい領域へコピーし、古い領域を `free()` します。
   容量計算では整数のオーバーフローも確認します。
4. `gnl_read()` は `read()` で保存領域の末尾へ最大 `BUFFER_SIZE` バイトを読み足し、
   `buffer_length` を更新して、未確認の位置から改行検索を続けます。
   改行を見つけるかEOFに達したら `1`、読み取りや領域確保に失敗したら `0` を返します。
5. `get_next_line()` は、読み取りが成功し、返すデータがあれば `gnl_take()` を呼びます。
   `gnl_take()` は改行まで（改行がなければ残り全部）の長さを調べ、
   返却用の領域を `malloc()` で確保します。`gnl_copy()` で1行をコピーし、
   終端文字 `\0` を付けます。改行がある場合は改行も含めます。
6. `gnl_take()` は、返す分を `buffer_length` から引き、
   `gnl_copy()` で余りを保存領域の先頭へ移して、返却用の行を `get_next_line()` に渡します。
7. `get_next_line()` は、余りが空なら `gnl_clear()` で保存領域を解放してから行を返します。
   エラー時やEOFで返すデータがない場合も、`gnl_clear()` で3つの変数を初期状態に戻し、
   `NULL` を返します。返された行は、呼び出し側が使用後に `free()` します。

保存領域は長さで管理し、返却する行にだけ終端文字を付けます。
外部関数は `read`、`malloc`、`free` のみです。
補助処理は、[get_next_line_utils.c](get_next_line_utils.c#L15) の5つの関数に分けています。

| 関数 | 役割 |
|---|---|
| `gnl_clear` | 保存領域を解放し、ポインタと管理情報を初期化する |
| `gnl_copy` | 指定したバイト数を先頭からコピーする。前方への重なりのある移動にも使う |
| `gnl_reserve` | 容量が不足したときだけ、領域を拡張して既存データを移す |
| `gnl_read` | 検索位置を進めながら、改行またはEOFまで読み足す |
| `gnl_take` | 最初の1行を取り出し、余りを保存領域の先頭へ移動する |


保存領域は、容量が足りないときだけ原則2倍に拡張します。これにより、メモリ確保とデータのコピーの回数を減らします。また、読み足した後の改行検索は未確認の位置から続けます。長い行でも同じ部分を何度も処理しないよう、この方式を採用しています。
し側が返却された行を保存し続ける場合、その分は別途必要です。

## State

`get_next_line()` は、次の3つの `static` 変数で読み取り状態を保持します。

```c
static char		*buffer;
static size_t	buffer_length;
static size_t	buffer_capacity;
```

| 変数 | 役割 |
|---|---|
| `buffer` | 読み取って、まだ返していないデータの保存領域へのポインタ |
| `buffer_length` | 読み取り済みで、まだ返していないデータのバイト数 |
| `buffer_capacity` | `buffer` に確保してある領域のバイト数 |

`buffer_length` は使用量、`buffer_capacity` は確保済み容量。
空き容量が十分なら再確保せず、`buffer + buffer_length` の位置へ直接読み足します。
保存領域は終端文字ではなく `buffer_length` で管理します。

検索位置 `search_pos` は `gnl_read()` のローカル変数で、
呼び出し時には0から検索を始め、読み足した後は確認済みの位置から続けます。
例えば `abc` を確認した後に `de\n` を読み足すと、位置3の `d` から検索を再開します。
`gnl_take()` は改行の位置を改めて調べ、返す行の長さを決めます。

保存データ `abc\nde` から `abc\n` を返すと、残りの `de` を先頭へ移し、
`buffer_length` を6から2へ更新します。領域は縮小しないので `buffer_capacity` は変わりません。

`static` 変数は、呼び出しをまたいで値を保持します。
初回は `buffer` が `NULL`、`buffer_length` と `buffer_capacity` が0に自動初期化されます。
残りが空になった場合やエラー時には、`gnl_clear()` が同じ初期状態へ戻します。

補助関数には `&buffer`、`&buffer_length`、`&buffer_capacity` を渡して呼び出し元の変数を更新します。
例えば、`gnl_read()` 内の `*buffer_length += bytes_read` は保存データの長さを更新します。
`char **buffer` の `*buffer` は保存領域へのポインタ、`(*buffer)[search_pos]` はその位置の文字です。

## Resources

- 配布された `get_next_line.pdf`（Version 14.3）：課題仕様。
- 指定された英語・日本語のNorm（Version 4.1）のPDFおよびTeX：コーディング規約。

## AIの使用
課題・Normの確認、READMEの作成補助
