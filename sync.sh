rsync --include "*.cpp" --include "*.txt" --include ".clang-format" --include "*/" --exclude="*" \
-a --progress -e 'ssh -p22222' baitianyu@10.170.54.216:/home/baitianyu/Linux-Web-Study/ /Users/baitianyu/CLionProjects/CPPStudy/Linux-Web-Study/
# --include "*/"目录及子目录下的所有前面这些扩展名的文件要同步，后面--exclude="*"除了前面的全都同步

