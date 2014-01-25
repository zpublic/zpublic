<?php
return array(
	'DB_TYPE'           => 'pdo', // 数据库类型
	'DB_DSN'            => 'mysql:unix_socket=/var/lib/mysql/mysql.sock;dbname=coeus',
	'DB_USER'           => 'root',
	'DB_PWD'            => 'Ezreal_LOL',
	'DB_PREFIX'         => '', // 数据库表前缀
	'SHOW_PAGE_TRACE'   => true,
	'TMPL_PARSE_STRING' => array(
		'PUBLIC_URL' => '/Public',
		'</title>'   => "</title>
	<script>
		window.PUBLIC_URL='/Public';
		window.APP_DEBUG=" . (APP_DEBUG? 'true' : 'false') . ";
	</script>"
	)
);
