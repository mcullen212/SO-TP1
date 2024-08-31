#!/bin/zsh

CONTAINER_NAME="SO-TP1"

docker start "$CONTAINER_NAME"

docker exec -ti "$CONTAINER_NAME" bash -c "cd /root && bash"
