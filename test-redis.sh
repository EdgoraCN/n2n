export LDFLAGS=-L/home/ahoo/workplace/edgora/vcpkg/installed/x64-linux/lib
export LD_LIBRARY_PATH=/home/ahoo/workplace/edgora/vcpkg/installed/x64-linux/lib:$LD_LIBRARY_PATH
export C_INCLUDE_PATH=/home/ahoo/workplace/edgora/vcpkg/installed/x64-linux/include/hiredis:/home/ahoo/workplace/edgora/vcpkg/installed/x64-linux/include/openssl:/home/ahoo/workplace/edgora/vcpkg/installed/x64-linux/include:$C_INCLUDE_PATH

cc -g -O2 -g3 -O3 -Wall   redis_test.c -L/home/ahoo/workplace/edgora/vcpkg/installed/x64-linux/lib  -lhiredis -o redis_test
./redis_test 127.0.0.1 6379 12345 2