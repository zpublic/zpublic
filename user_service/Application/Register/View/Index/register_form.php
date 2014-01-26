<!DOCTYPE html>
<html>
<head>
	<title>注册！</title>
	<link href="PUBLIC_URL/styles/bootstrap.min.css" rel="stylesheet"/>
	<link href="PUBLIC_URL/styles/register.min.css" rel="stylesheet"/>
	<script type="text/javascript" src="PUBLIC_URL/scripts/jquery.min.js"></script>
	<script type="text/javascript" src="PUBLIC_URL/scripts/bootstrap.min.js"></script>

	<script type="text/javascript" src="PUBLIC_URL/code-complete/require.js" data-main="PUBLIC_URL/{:APP_DEBUG?'js/boot.js':'scripts/boot.min.js'}"></script>
	<!--script type="text/javascript" src="PUBLIC_URL/scripts/require.min.js" data-main="PUBLIC_URL/{:APP_DEBUG?'js/boot.js':'scripts/boot.min.js'}"></script-->
</head>

<body style="">

<div class="container">

	<form class="form-signin" method="post" role="form" type="ajax" action="{:U('register')}" data-action="{:U('register_ajax')}">
		<h2 class="form-signin-heading">注册帐号</h2>

		<fieldset>
			<input type="email" name="email" class="form-control" placeholder="电子邮件" required="required" autofocus="">
			<input type="password" name="passwd" class="form-control" placeholder="密码" required="required" autocomplete="off">
			<input type="password" name="repasswd" class="form-control" placeholder="重复密码" required="required" autocomplete="off">
		</fieldset>
		<label class="checkbox">
		<input type="checkbox" name="agree" value="1" required="required">
		我已经阅读并同意
		<a href="###">《使用协议》（知道你不会看，自动帮你勾上）</a>
		</label>
		<div>
			<button class="btn btn-lg btn-primary col-xs-7" type="submit">确定注册</button>
			<a href="{:U('Login/index/index')}" class="btn btn-lg btn-default col-xs-4 col-xs-offset-1">登录已有帐号</a>
		</div>
		<div class="clearfix" style="margin-bottom:30px;"></div>
		<div id="Alert" class="alert alert-info" style="display: none;"></div>
	</form>
</div>
</body>
</html>
