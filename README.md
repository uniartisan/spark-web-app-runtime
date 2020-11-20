[                                                        ](spk://store/development/spark-webapp-runtime-waker) ![](RackMultipart20201120-4-ntgpe9_html_1acb66441c6937a3.gif)

                                                   ![](RackMultipart20201120-4-ntgpe9_html_c5dd964be3f7b4b3.gif)

运行 &quot;spark-webapp-runtime -h&quot; 查看命令行帮助信息


用法：./SparkWebAppRuntime [选项]
描述: 星火网页应用运行时

选项：
 -h, --help 显示这个此帮助。
 -v, --version 显示版本信息。
 -p, --parser 启用参数解析方式. 默认顺序解析方式.
 -t, --title \&lt;title\&gt; 设置程序的运行标题. 默认是 星火网页应用运行时.
 -u, --url \&lt;url\&gt; 设置要打开的目标URL. 默认是空.
 -w, --width \&lt;width\&gt; 设置程序运行的宽度. 默认是 1024.
 -H, --height \&lt;height\&gt; 设置程序运行的高度. 默认是 768
 -i, --ico \&lt;ico\&gt; 设置程序运行的图标.
 -d, --desc \&lt;desc\&gt; 设置程序的描述信息.
 -c, --cfg \&lt;cfg\&gt; 设置程序运行的配置文件.
 -r, --root \&lt;root\&gt; 设置内置WebServer的根路径.
 -P, --port \&lt;port\&gt; 设置内置WebServer的监听端口号.

这里给大家附送两个例子，帮助大家理解使用

这是一款使用了-r选项（自带webserver）的一款应用的desktop文件

[Desktop Entry]
 Categories=Game;
 Encoding=UTF-8
 Exec=spark-webapp-runtime -p -u http://localhost:38324 -t &quot;Unfair Undyne&quot; -d &quot;不公平的安黛因，传说之下（Undertale）鱼姐战&quot; -w 720 -H 620 -i /opt/durapps/fairdyne/icon.png -r /opt/durapps/fairdyne/pages -P 38324
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

作为星火系列应用，我们的设计哲学是M.I.S.(Make it Simple)没错我就是从KISS抄的

配置spark-webapp-runtime环境是非常简单的

只需要

点击

                            [   ](spk://store/development/spark-webapp-runtime-waker) ![](RackMultipart20201120-4-ntgpe9_html_64e2d0d76a9e2e60.gif)[
](spk://store/development/spark-webapp-runtime-waker)

即可配置完成

现在我们来测试一下

spark-webapp-runtime

如果你能看到

![](RackMultipart20201120-4-ntgpe9_html_aaa9c7403a5f4461.gif)

那么恭喜你，配置完成了

# 1.简易操作

作为第一种调用方式，简易调用是非常简单粗暴的

你可以试试输入这一段

spark-webapp-runtime 垃圾百度 https://www.baidu.com

看到了么？就是这么简单

![](RackMultipart20201120-4-ntgpe9_html_e6fc0f985ad49a90.gif)

如果觉得默认的1024x768分辨率不合你心意

你可以简单的在后面加上你喜欢的分辨率

比如这样

spark-webapp-runtime 垃圾百度 https://www.baidu.com 768 1024

![](RackMultipart20201120-4-ntgpe9_html_ceac29ecac00fe92.gif)

And你甚至可以使用本地的网页文件

先找到文件

    ![](RackMultipart20201120-4-ntgpe9_html_52dd6dcf6c40f06a.gif)

在浏览器里打开

![](RackMultipart20201120-4-ntgpe9_html_dd596e3d8bbe0c72.gif)

复制粘贴

spark-webapp-runtime 这是一个本地网页 file:///home/sy120zx/Desktop/Index%20of%20\_store\_games\_fairdyne\_.html 1024 768

![](RackMultipart20201120-4-ntgpe9_html_21550fd9e5d5f3d.gif)

你还可以整一些生草的活

spark-webapp-runtime 垃圾百度 https://www.baidu.com 768 84

![](RackMultipart20201120-4-ntgpe9_html_f41a3cae433a275e.gif)

所以大家学会了么？spark-webapp-runtime的简单调用方式：

spark-webapp-runtime [标题] [地址]

spark-webapp-runtime [标题] [地址] [宽] [高]

简易调用优点：简单，符合直觉，无需学习就可以使用，语法兼容spark-web-core（已弃用）

缺点：自定义程度低，不支持本地服务器功能

# 2.标准调用