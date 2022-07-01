![](https://examine-spark.oss-cn-shanghai.aliyuncs.com/icons/2020/11/14/38fe8d30-268e-11eb-881e-fff03f7ff5ca.png)

运行 &quot;spark-webapp-runtime -h&quot; 查看命令行帮助信息


```shell
帮助:
第一种用法:

spark-webapp-runtime 应用标题 目标网址 应用窗口宽度 应用窗口高度

第二种用法:

运行 "spark-webapp-runtime -h" 查看命令行帮助信息


                用法：./spark-webapp-runtime [选项]
                描述: 星火网页应用运行环境

                选项：
                  -h, --help             显示这个此帮助。
                  -v, --version          显示版本信息。
                  -p, --parser           启用参数解析方式。默认为顺序解析方式。
                  -t, --title <title>    设置应用的标题。默认为
                                         星火网页应用运行环境。
                  -u, --url <url>        设置要打开的目标 URL。默认为空。
                  -w, --width <width>    设置应用的窗口宽度。默认为 1024。
                  -H, --height <height>  设置应用的窗口高度。默认为 768。
                  -T, --tray             启用托盘图标。默认不启用。
                  --full-screen          以全屏模式运行。默认关闭该功能。
                  --fix-size             固定窗口大小。默认关闭该功能。
                  --hide-buttons         隐藏控制按钮。默认关闭该功能。
                  -i, --ico <ico>        设置应用的图标。
                  -d, --desc <desc>      设置应用的描述信息。
                  -c, --cfg <cfg>        设置应用的配置文件。
                  -r, --root <root>      设置内置 WebServer 的根路径。
                  -P, --port <port>      设置内置 WebServer 的监听端口号。
                  -G, --GPU <0/1>        设置启用 GPU 渲染加速，默认开始。
            

需要注意的是, 命令行会覆盖配置文件信息。

```

这里给大家附送两个例子，帮助大家理解使用

这是一款使用了-r选项（自带webserver）的一款应用的desktop文件



    [Desktop Entry]
     Categories=Game;
     Encoding=UTF-8
     Exec=spark-webapp-runtime -p -u http://localhost:38324 -t "Unfair Undyne" -d "不公平的安黛因，传说之下（Undertale）鱼姐战" -w 720 -H 620 -i /opt/durapps/fairdyne/icon.png -r /opt/durapps/fairdyne/pages -P 38324
     Icon=/opt/durapps/fairdyne/icon.png
     MimeType=
     Name[zh\_CN]=Fairdyne
     Name=Fairdyne
     StartupWMClass=Fairdyne
     Terminal=False
     Type=Application
     X-Deepin-CreatedBy=com.deepin.dde.daemon.Launcher
     X-Deepin-AppID=fairdyne

附这款应用的下载链接

[https://dcstore.spark-app.store/store/games/fairdyne/fairdyne\_1.4\_plus\_sm1\_amd64.deb](https://dcstore.spark-app.store/store/games/fairdyne/fairdyne_1.4_plus_sm1_amd64.deb)

欢迎大家积极制作

以下是新手教程

**使用教程**

# 0.配置环境


配置 spark-webapp-runtime 环境是非常简单的

只需要

`sudo apt install spark-webapp-runtime-waker`

即可配置完成

现在我们来测试一下

`spark-webapp-runtime`

如果你能看到

![](https://examine-spark.oss-cn-shanghai.aliyuncs.com/images/2020/11/14/612135b0-268e-11eb-881e-fff03f7ff5ca.png)

那么恭喜你，配置完成了

# 1.简易操作

作为第一种调用方式，简易调用是非常简单粗暴的

你可以试试输入这一段

`spark-webapp-runtime 垃圾百度 https://www.baidu.com`

看到了么？就是这么简单

![](https://storage.deepin.org/thread/202011190957234196_thread_OSva1QtI80i.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=Bbs-minioadmin%2F20201121%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20201121T051756Z&X-Amz-Expires=20&X-Amz-SignedHeaders=host&X-Amz-Signature=1ea2f455b1e066599660c0e868d64eba262e3345356ddfa78a638a57d10e9120)

如果觉得默认的1024x768分辨率不合你心意

你可以简单的在后面加上你喜欢的分辨率

比如这样

`spark-webapp-runtime 垃圾百度 https://www.baidu.com 768 1024`
![](https://storage.deepin.org/thread/202011190958558825_thread_SGQvVsatdOi.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=Bbs-minioadmin%2F20201121%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20201121T051756Z&X-Amz-Expires=20&X-Amz-SignedHeaders=host&X-Amz-Signature=9ffeef4aec3311c9a19766b7ebb922fff454b792c8cb3c3cbb9164fd7e6bd046)


And你甚至可以使用本地的网页文件

先找到文件

 ![](https://s3.ax1x.com/2020/11/19/DugXGQ.png)

在浏览器里打开

![](https://s3.ax1x.com/2020/11/19/Du2pq0.png)

复制粘贴

`spark-webapp-runtime 这是一个本地网页 file:///home/sy120zx/Desktop/Index%20of%20\_store\_games\_fairdyne\_.html 1024 768`

![](https://s3.ax1x.com/2020/11/19/Du2QIO.png)

你还可以整一些生草的活

`spark-webapp-runtime 垃圾百度 https://www.baidu.com 768 84`

![](https://storage.deepin.org/thread/202011191000162762_thread_sbTYl51TF9Q.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=Bbs-minioadmin%2F20201121%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20201121T051756Z&X-Amz-Expires=20&X-Amz-SignedHeaders=host&X-Amz-Signature=ba183760a46a5c40ba92911621b7ce1a9e4f53f55243e61f4e22ef7f4e92d0b8)

所以大家学会了么？spark-webapp-runtime的简单调用方式：

`spark-webapp-runtime [标题] [地址]

`spark-webapp-runtime [标题] [地址] [宽] [高]``

简易调用优点：简单，符合直觉，无需学习就可以使用，语法兼容spark-web-core（已弃用）

缺点：自定义程度低，不支持本地服务器功能

# 2.标准调用