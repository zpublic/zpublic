<?php
namespace Register\Model;

use Think\Model;

class RegisterModel extends Model{
	protected $tableName = 'users';
	protected $name = 'users';

	protected $_validate = array(
		array('agree', 'require', '必须同意使用协议', self::MUST_VALIDATE, self::MODEL_INSERT),
		array('email', 'email', '该邮箱已经被使用', self::MUST_VALIDATE, 'unique', self::MODEL_INSERT),
		array('repasswd', 'passwd', '确认密码不正确', self::MUST_VALIDATE, 'confirm', self::MODEL_INSERT),
	);
	protected $_auto = array(
		array('passwd', 'encrypt', self::MODEL_BOTH, 'callback'),
		array('reg_date', 'time', self::MODEL_INSERT, 'function'),
		array('email_verify', '0', self::MODEL_INSERT),
		array('last_login', '0', self::MODEL_INSERT),
	);

	public function checkExist($email){
		return $this->where(array('email' => $email))->count();
	}

	protected function encrypt($str){
		return mencrypt($str, 'where to get the key?');
	}
}
