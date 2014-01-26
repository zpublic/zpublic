<?php
return array(
	'DEFAULT_MODULE'       => 'Index', // 默认模块

	/* 数据库设置 */
	'DB_TYPE'              => 'pdo', // 数据库类型
	'DB_DSN'               => 'mysql:unix_socket=/var/lib/mysql/mysql.sock;dbname=coeus',
	'DB_NAME'              => 'coeus',
	'DB_USER'              => 'root',
	'DB_PWD'               => 'meiyoumima36911',
	'DB_PREFIX'            => '', // 数据库表前缀
	'DB_FIELDTYPE_CHECK'   => false, // 是否进行字段类型检查
	'DB_FIELDS_CACHE'      => true, // 启用字段缓存
	'DB_CHARSET'           => 'utf8', // 数据库编码默认采用utf8
	'DB_DEPLOY_TYPE'       => 0, // 数据库部署方式:0 集中式(单一服务器),1 分布式(主从服务器)
	'DB_RW_SEPARATE'       => false, // 数据库读写是否分离 主从式有效
	'DB_MASTER_NUM'        => 1, // 读写分离后 主服务器数量
	'DB_SLAVE_NO'          => '', // 指定从服务器序号
	'DB_SQL_BUILD_CACHE'   => true, // 数据库查询的SQL创建缓存
	'DB_SQL_BUILD_QUEUE'   => 'apc', // SQL缓存队列的缓存方式 支持 file xcache和apc
	'DB_SQL_BUILD_LENGTH'  => 20, // SQL缓存的队列长度
	'DB_SQL_LOG'           => false, // SQL执行日志记录
	'DB_BIND_PARAM'        => false, // 数据库写入数据自动参数绑定

	/* 数据缓存设置 */
	'DATA_CACHE_TIME'      => 0, // 数据缓存有效期 0表示永久缓存
	'DATA_CACHE_TYPE'      => 'Apc', // 数据缓存类型,支持:File|Db|Apc|Memcache|Shmop|Sqlite|Xcache|Apachenote|Eaccelerator

	/* SESSION设置 */
	'SESSION_AUTO_START'   => false, // 是否自动开启Session

	/* 模板引擎设置 */
	'TMPL_TEMPLATE_SUFFIX' => '.php', // 默认模板文件后缀

	/* URL设置 */
	'URL_PARAMS_BIND_TYPE' => 0, // URL变量绑定的类型 0 按变量名绑定 1 按变量顺序绑定

	'CHECK_APP_DIR'        => true, // 是否检查应用目录是否创建

	/* URL设置 */
	'URL_MODEL'            => 2, // URL访问模式,可选参数0、1、2、3,代表以下四种模式：
	// 0 (普通模式); 1 (PATHINFO 模式); 2 (REWRITE  模式); 3 (兼容模式)  默认为PATHINFO 模式

	'TMPL_PARSE_STRING'    => array(
		'PUBLIC_URL' => 'http://public.gamediary.org',
		'</title>'   => "</title>
	<script>
		window.PUBLIC_URL='http://public.gamediary.org';
		window.APP_DEBUG=" . (APP_DEBUG? 'true' : 'false') . ";
	</script>"
	)
);
