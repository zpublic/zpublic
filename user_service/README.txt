用户配置要求。。

*所有* 请求都rewrite到 index.php/$query_string

/Application/Common/Conf/config.php
需要一个PUBLIC_URL，访问这个URL可以打开 /Public 目录

使用 LoginService ：
用php5.5或以上版本运行：
	php index.php [配置]
配置文件里有各种东西
