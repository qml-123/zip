首先新建build/目录和bin/目录
进入build/目录，执行 cmake .. 命令，之后执行make，可执行文件就输出到bin/目录中了

///
mkdir build
mkdir bin
cd build
cmake ..
make
cd ..
bin/server 
bin/client
///

支持三种命令ls、cat、send
例如: ls，打印服务端目录结构(不能加参数)
cat filename, 查看文件内容
send -lz filename(目前可用参数 -lz,-al,-ar)
