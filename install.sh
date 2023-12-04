#!/bin/bash

if ! [ -x "$(command -v cmake)" ]; then
  echo -e '\033[0;31mError:\033[0m cmake is not installed on your system.' >&2
  exit 1
fi

set -e

cmake -B build

cmake --build build

cp build/Server build/R-Type .

rm -rf build

echo -e '\n\033[0;32mR-Type successfully installed\033[0m'
echo -e "\nExecute \033[1;37m./Server [port]\033[0m to run the server, don't forget to specify a port"
echo -e 'Execute \033[1;37m./R-Type\033[0m to run the game\n'
