#!/bin/bash
if [[ ! -d target ]]; then
  mkdir target
fi
javac src/main/java/ascii/Main.java -d target
