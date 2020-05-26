export LDFLAGS=-L/home/ahoo/workplace/edgora/vcpkg/installed/x64-linux/lib
export LD_LIBRARY_PATH=/home/ahoo/workplace/edgora/vcpkg/installed/x64-linux/lib:$LD_LIBRARY_PATH
export C_INCLUDE_PATH=/home/ahoo/workplace/edgora/vcpkg/installed/x64-linux/include/hiredis:/home/ahoo/workplace/edgora/vcpkg/installed/x64-linux/include/openssl:/home/ahoo/workplace/edgora/vcpkg/installed/x64-linux/include:$C_INCLUDE_PATH
make clean
./autogen.sh
./configure 
make