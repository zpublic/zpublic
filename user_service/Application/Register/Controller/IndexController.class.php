<?php
namespace Register\Controller;

use Think\Controller;

class IndexController extends Controller{
	public function index(){
		$this->display('Index/register_form');
	}

	public function check(){
	}

	public function register_ajax(){
		$mdl  = new \Register\Model\RegisterModel();
		$data = $mdl->create($_POST);
		$ret  = 0;
		if($data){
			$ret = $mdl->add($data);
		}
		if($ret){
			$this->ajaxReturn(array('code' => 0));
		} else{
			$this->ajaxReturn(array('code' => 1, 'message' => $mdl->getError() . $mdl->getDbError()));
		}
	}

	public function register(){
		$mdl  = new \Register\Model\RegisterModel();
		$data = $mdl->create($_POST);
		$ret  = 0;
		if($data){
			$ret = $mdl->add($data);
		}
		if($ret){
			$this->success('注册成功', '/');
		} else{
			$this->error('注册失败：' . $mdl->getError() . $mdl->getDbError(), '/');
		}
	}
}
