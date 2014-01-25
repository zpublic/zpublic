<?php
$e = chr(27);
function doing($line){
	global $e;
	echo "{$e}[2m{$line}{$e}[0m  --  ";
}

function success($msg = '成功'){
	global $e;
	echo "{$e}[38;5;10m{$msg}{$e}[0m\n";
}

function error($msg = '失败'){
	global $e;
	echo "{$e}[38;5;9m{$msg}{$e}[0m\n";
}

function warn($msg = '警告'){
	global $e;
	echo "{$e}[38;5;3m{$msg}{$e}[0m\n";
}

register_shutdown_function(function (){
	echo "\n";
});
