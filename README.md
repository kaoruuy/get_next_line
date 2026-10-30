*This project has been created as part of the 42 curriculum by kayoshid.*

## Description

`get_next_line(int fd)` は、ファイルや標準入力から1行ずつ返す関数です。
改行があれば `\n` を含め、最終行に改行がなければ残りの文字列を返します。
EOFで返す文字がない場合や読み取りエラーの場合は `NULL` を返します。
複数のファイルディスクリプタを交互に読むボーナス部分は実装していません。

## Instructions

同梱の動作確認用 `main.c` とコンパイルします。

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o gnl
./gnl sample.txt
```

引数なしなら標準入力を読み取ります。`main.c` は各行に番号を付けて表示します。

```sh
printf 'first\nsecond\n' | ./gnl
```

`BUFFER_SIZE` は1回の `read()` で読み取る最大バイト数で、既定値は42です。
返す行の長さの上限ではありません。

利用側は `get_next_line.h` をインクルードし、`open()` の戻り値、または標準入力の `0` を渡します。
返された行は使用後に `free()` し、`NULL` が返るまで呼び出します。
同じ入力をEOFまで読み切ってから別の入力へ切り替えてください。
途中で呼び出しをやめると、次回用の文字列が内部に残る場合があります。

## Algorithm

1. `get_next_line()` は `fd` と `BUFFER_SIZE` を確認し、
   `static char *str` に読み取り済みでまだ返していない文字列を保持します。
2. `read_to_str()` は `is_new_line()` で改行を探します。
   改行がなければ `grow_buffer()` で空きを確保し、`read_more()` で末尾へ読み足します。
   新しく読んだ部分を検索し、改行またはEOFまで繰り返します。
3. `grow_buffer()` は容量不足時だけ、必要量と現在の容量の2倍のうち大きい方を確保します。
   保存済みの文字列を移して古い領域を解放し、容量計算のオーバーフローも確認します。
4. `divide_line()` は、`strcut_fr()` で最初の1行をコピーし、
   `strcut_lst()` で残りを次回用に保存します。元の文字列は解放します。
5. `get_next_line()` は切り出した行を返します。
   EOFで返す文字がなければ保存領域を解放し、`str` を `NULL` に戻します。

読み取りと行の切り出しを分け、メモリの用途を追いやすくしています。
容量を原則2倍に拡張し、改行検索を未確認の位置から続けることで、
長い行でも全文のコピーや検索を何度も繰り返さずに済みます。

| 変数 | 役割 |
|---|---|
| `str` | 読み取り済みで、まだ返していない文字列 |
| `text_length` | 保存している文字列のバイト数 |
| `buffer_capacity` | 確保してある領域のバイト数 |
| `search_pos` | 次に改行を検索する位置 |
| `bytes_read` | `read()` が返したバイト数。0はEOF、-1はエラー |

## Testing

```sh
cd gnlTester
make m
```

testerには `gcc`、`clang++`、`make`、Linuxでは `valgrind` が必要です。
`BUFFER_SIZE` を変え、空入力、改行の有無、長い行、標準入力などを確認します。

## Resources

- `get_next_line.pdf`（Version 14.3）：課題仕様。
- 英語・日本語のNorm（Version 4.1）：コーディング規約。
- `man 2 read`、`man 2 open`、`man 2 close`：ファイル操作。
- `man 3 malloc`、`man 3 free`：メモリ管理。
- `gnlTester/README.md`：testerの使い方。

## AIの使用

AI（Codex）は、課題・Normの確認、実装・修正案の作成、動作検証、READMEの作成補助に使用しました。
