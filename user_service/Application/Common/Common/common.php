<?php
/**
 *
 *
 * @param        $string
 * @param        $key
 * @param string $algo
 * @param string $mode
 *
 * @return string
 */
function mencrypt($string, $key, $algo = MCRYPT_DES, $mode = MCRYPT_MODE_CFB){
	$key = md5($key);
	$td  = mcrypt_module_open($algo, '', $mode, '');
	$key = substr($key, 0, mcrypt_enc_get_key_size($td));
	$iv = mcrypt_create_iv(mcrypt_enc_get_iv_size($td), MCRYPT_DEV_URANDOM);
	if(mcrypt_generic_init($td, $key, $iv) != -1){
		$c_t = mcrypt_generic($td, $string);
		$c_t = $iv . $c_t;
	} else{
		$c_t = '';
	}
	mcrypt_generic_deinit($td);
	mcrypt_module_close($td);
	return $c_t;
}

/**
 *
 *
 * @param        $string
 * @param        $key
 * @param string $algo
 * @param string $mode
 *
 * @return string
 */
function mdecrypt($string, $key, $algo = MCRYPT_DES, $mode = MCRYPT_MODE_CFB){
	$key     = md5($key);
	$td      = mcrypt_module_open($algo, '', $mode, '');
	$key     = substr($key, 0, mcrypt_enc_get_key_size($td));
	$iv_size = mcrypt_enc_get_iv_size($td);
	$iv = substr($string, 0, $iv_size);
	$string = substr($string, $iv_size);
	if(mcrypt_generic_init($td, $key, $iv) != -1){
		$c_t = mdecrypt_generic($td, $string);
	} else{
		$c_t = '';
	}
	mcrypt_generic_deinit($td);
	mcrypt_module_close($td);
	return $c_t;
}
