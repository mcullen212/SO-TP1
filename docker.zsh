#!/bin/zsh

CONTAINER_NAME="SO-TP1"

docker exec -ti "$CONTAINER_NAME" bash -c "cd root && exec zsh"