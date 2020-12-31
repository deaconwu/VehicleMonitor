1. Compress
	提供数据压缩/解压 API, 生成"Compress.dll"

2. DataCompress
	实现数据压缩/解压的demo，需调用"Compress.dll"

3. FileCompress
	压缩单个文件的demo
	
4. FileZip
	实现多个文件打包成zip的demo，调用zlib库，
	需下载http://zlib.net/， 进入zlib-1.2.11\contrib\vstudio\vc14，编译生成zlibwapi.lib和zlibwapi.dll

5. HttpPost
	实现Http上传json串和文件到http server的demo，调用libcurl库
	需下载https://curl.se/libcurl/， 进入curl-7.72.0\projects\Windows\VC14，编译生成curl-7.72.0\build\Win64\VC14\LIB Release\libcurl.lib
	
6. smtp
	实现Smtp发邮件的demo，包括主题、内容、附件，中英文，多个收件人
	发送方需登录邮箱获取授权码

7. VehicleDuilibMfc
	实现MFC界面美化的demo
	需下载https://github.com/quanzhuo/DuiDesigner，编译生成UI设计工具，添加控件布局并生成控件属性表(.xml)
	需下载https://github.com/duilib/duilib, 编译生成DuiLib.lib
	
8. MsgHandle
	实现实时接收(异步Socket)，实时解析(线程)，定时触发统计(线程)，实时预警(线程)，存储(循环队列)
	
9. NewEnergyVehicle
    实现新能源风险车，分步执行程序数据收发、数据入库、常态化点名、数据上传。
	配置内容存放、周期触发执行程序、接入本地数据库记录日志
	接入http上传的代码"HttpPost"
	测试可执行程序的启动和退出
	需下载sqlite3，sqlite3命令：
	9.1. 命令行输入"sqlite3"

	9.2. 输入命令: "sqlite3 要打开的数据库名绝对路径"

	9.3. 查看数据库路径 ".databases"

	9.4. 查看所有表 ".tables"

	9.5. 查看表的完整信息 ".schema 表名" 
	
	可下载安装SQLiteStudio查看可视化表
	