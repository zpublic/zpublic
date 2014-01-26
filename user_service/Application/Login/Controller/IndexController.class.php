<?php
namespace Login\Controller;

use Think\Controller;

class IndexController extends Controller{
	public function index(){
		$this->show('泡面说先不做登录。');
	}
}
