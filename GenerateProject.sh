[ ! -d "./build" ] && mkdir build
cd ./build
cmake ..

read -s -n 1 -p "Press any key to continue . . ."
echo ""
