支持的功能

该命令行工具基于GNU readline库实现，支持以下功能：

1. 命令提示
  
  ```bash
  cli% 
  exit  find  list  
  cli% 
  ```
  
2. 可选项提示（输入'-'后提示）
  
  ```bash
  cli% find -
  -depth  -help   -name   
  cli% find -
  ```
  
3. 文件提示（readline库自带）
  
  ```bash
  cli% list 
  CMakeLists.txt   a.out
  ```
  
4. 多行输入（以'\'结尾的换行）
  
  ```bash
  cli% find -name\
  >a.out 
  [INFO] input:find -name a.out
  ```
  
5. 单次输入多条命令，以';'作为分隔符。
  
  ```bash
  cli% find -name a.out ; list ;
  [INFO] input:find -name a.out 
  [INFO] input: list
  ```

6. 通过上下箭头查找历史命令


### 如何使用

参考main文件


### 编译

确保安装了readline库。

```bash
g++ main.cpp -lncurses -lreadline
```
  
