*This project has been created as part of the 42 curriculum by kayoshid.*

## Description

`get_next_line(int fd)` は、ファイルディスクリプタから1行ずつ読み取る関数です。
改行があれば改行を含めて返し、末尾に改行がなければ最後の文字まで返します。
EOFで返す文字がない場合、またはエラーが起きた場合は `NULL` を返します。
通常のファイルと標準入力に対応する必須部分を実装しています。
複数のファイルディスクリプタを交互に読むボーナス部分は対象外です。

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

1. `static char *stash` に、前回読み取ってまだ返していない文字列を保持します。
2. `stash` に改行がなければ、`BUFFER_SIZE` バイトずつ `read()` し、
   確保した領域に連結します。改行を見つけるかEOFに達した時点で読み取りを止めます。
3. 最初の改行まで（EOFの場合は残り全部）を別の領域にコピーして返します。
   未返却の部分は `stash` の先頭へ移動し、次の呼び出しに備えます。
4. 残りが空になった場合や、読み取り・メモリ確保に失敗した場合は、
   保持領域を解放してポインタを `NULL` に戻します。

単純な文字列とループだけで実装し、連結リストや容量管理を省くことで、
処理とメモリの所有関係を追いやすくしています。
外部関数は `read`、`malloc`、`free` のみです。
補助関数はすべて `get_next_line_utils.c` にまとめています。

この方式は追加の読み取りごとに既存の文字列をコピーするため、
長さLの行とバッファサイズBに対して、連結の処理量は最悪で
O(L²/B + L) です。また、行を返す際には残りの文字列を移動します。
使用メモリはO(L + B)です。極端に長い行での速度より実装の簡潔さを優先しています。

## Resources

- 配布された `get_next_line.pdf`（Version 14.3）：課題仕様。
- 指定された英語・日本語のNorm（Version 4.1）のPDFおよびTeX：コーディング規約。
- `man 2 read`：読み取り、EOF、エラー、短い読み取りの説明。
- `man 3 malloc`、`man 3 free`：動的メモリの確保と解放。

AI（Codex）は、課題・Normの確認、Cファイルとヘッダーの実装、
このREADMEの作成、および動作・エラー処理の検証に使用しました。
