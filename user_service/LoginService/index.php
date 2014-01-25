#!/bin/php
<?php
chdir(__DIR__);
require 'messages.php';

require (isset($argv[1])? $argv[1] : 'config') . '.php';

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if(!$socket){
	error('无法建立socket对象');
	exit;
}

doing('监听： ' . HOST . ':' . PORT);
if(socket_bind($socket, HOST, PORT) && socket_listen($socket)){
	success();
} else{
	error(socket_strerror(socket_last_error()));
	exit;
}

mainLoop($socket);

function mainLoop($socket){
	doing('等待连接');
	while($connection = socket_accept($socket)){
		if(FORK){
			while($pid = pcntl_wait($s, WNOHANG) > 0){
				echo "[{$pid}]回收资源\n";
			}
			$pid = pcntl_fork();
			if($pid == -1){
				error('fork失败！');
			} elseif($pid == 0){
				success('新连接');
				handle($connection);
			}
		} else{
			handle($connection);
		}
	}
}

function handle($socket){
	$buffer = '';
	$pid    = FORK? posix_getpid() : 'main';
	while($buffer .= socket_read($socket, 20480, PHP_BINARY_READ)){
		echo "[{$pid}]" . $buffer;
		if(strlen($buffer) <= 4){
			continue;
		}
		$code = unpack('ll', substr($buffer, 0, 4));
	}
}
