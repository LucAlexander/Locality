cd Entity-Component-System/
make build
cd ../DataContainers/
make build
cd ../SDL2-Utility/
make build
cd ../compile/
make
mv compile cmp
mv cmp ..
cd ..
