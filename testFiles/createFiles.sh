#!/bin/bash
for ((i=1; i<=55; i++)); do
    dd if=/dev/urandom of=file$i bs=$((200 + RANDOM % 100))M count=1
        ln -s file$i link${i}_1
    ln -s file$i link${i}_2
    ln -s file$i link${i}_3
done