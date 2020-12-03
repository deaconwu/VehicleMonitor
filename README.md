1. Compress
	提供数据压缩/解压 API, 生成"Compress.dll"

2. DataCompress
	实现数据压缩/解压，需调用"Compress.dll"

3. FileCompress
	压缩单个文件的demo
	
4. FileZip
	实现多个文件打包成zip，调用zlib库，
	需下载http://zlib.net/， 进入zlib-1.2.11\contrib\vstudio\vc14，编译生成zlibwapi.lib和zlibwapi.dll

5. HttpPost
	实现Http上传json串和文件到http server，调用libcurl库
	需下载https://curl.se/libcurl/， 进入curl-7.72.0\projects\Windows\VC14，编译生成curl-7.72.0\build\Win64\VC14\LIB Release\libcurl.lib
	
6. smtp
	实现Smtp发邮件，包括主题、内容、附件，中英文，多个收件人
	发送方需登录邮箱获取授权码
	
7. MsgHandle
	实现实时接收(异步Socket)，实时解析(线程)，定时触发统计(线程)，实时预警(线程)，存储(循环队列)