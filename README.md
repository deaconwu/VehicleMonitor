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