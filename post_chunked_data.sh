#!/bin/bash

file_path="unit_test"  # POSTするファイルのパス
chunk_size=1024  # チャンクサイズ（バイト単位）

# ファイルのサイズを取得
file_size=$(wc -c < "$file_path")

# チャンクの数を計算
num_chunks=$((file_size / chunk_size))

# チャンクごとにループ
for ((i=0; i < num_chunks; i++))
do
  # オフセットとチャンクサイズを計算
  offset=$((i * chunk_size))
  size=$chunk_size

  # 最後のチャンクの場合、サイズを調整
  if ((i == num_chunks - 1))
  then
    size=$((file_size - offset))
  fi

  # チャンクを切り出してPOSTリクエストを送信
  chunk=$(dd if="$file_path" bs=1 skip="$offset" count="$size" 2>/dev/null | base64)
  curl -X POST -H "Content-Type: application/octet-stream" --data-binary "$chunk" "http://localhost:8090"
done