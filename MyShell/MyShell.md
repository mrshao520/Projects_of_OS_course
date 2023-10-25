# MyShell

## install

### compile

```shell
# 下载第三方库
$ apt install cmake
$ apt install libspdlog-dev

# 编译
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ make install
```

## options

* -v, --version : 显示版本
* -h, --help      : 显示帮助

## examples

```shell
$ MyShell -v   # 查看版本
```

![image-20231025184040077](../images/MyShell/image-20231025184040077.png)

```shell
$ MyShell -h    # 显示帮助
```

![image-20231025184417676](../images/MyShell/image-20231025184417676.png)

```shell
$ MyShell      # shell framework
```

![image-20231025184536656](../images/MyShell/image-20231025184536656.png)

```shell
$ cd /home     # 更改工作路径
```

![image-20231025184615582](../images/MyShell/image-20231025184615582.png)

```shell
$ help        # 显示帮助
```

![image-20231025184725864](../images/MyShell/image-20231025184725864.png)

```
$ # 命令
$ MyGrep --version
$ MyGrep -s make -d ./
```

![image-20231025185008132](../images/MyShell/image-20231025185008132.png)

![image-20231025185336786](../images/MyShell/image-20231025185336786.png)









