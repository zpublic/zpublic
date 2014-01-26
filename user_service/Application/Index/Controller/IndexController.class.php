<?php
namespace Index\Controller;

use Think\Controller;

class IndexController extends Controller{
	public function index(){
		$this->show('默认页面。<a href="'.U('Register/index/index').'">注册</a><a href="'.U('Login/index/index').'">登录</a>');
	}
}
