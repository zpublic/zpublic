
proto文件位于：  
zPublic\src\irene\common\proto_files\  

执行zPublic\src\irene\common\auto_gen.bat后，生成的代码文件位于：  
zPublic\src\irene\common\protocol\  

-----------------------------------------------------------------
opcodes约定
===========
0-399 保留  
400-3999 基础模块  
4000-39999 内部插件  
40000-59999 外部插件  
60000-4294967295 保留  
一般情况下  
模块或者插件中一项功能分配100个opcodes  
一个插件分配1000个opcodes  

-----------------------------------------------------------------
基础支持模块相关proto
=====================
上线相关proto  
<table>
	<tr>
		<td>传输方向</td>
		<td>包名</td>
		<td>参数</td>
		<td>作用</td>
	</tr>
	<tr>
		<td>C2S</td>
		<td>auth</td>
		<td>name</td>
		<td>要求验证</td>
	</tr>
	<tr>
		<td>S2C</td>
		<td>auth</td>
		<td>password</td>
		<td>传输验证信息</td>
	</tr>
	<tr>
		<td>C2S</td>
		<td>login</td>
		<td>return</td>
		<td>验证通过或者失败</td>
	</tr>
	<tr>
		<td>S2C</td>
		<td>setname</td>
		<td>name</td>
		<td>修改rouji名</td>
	</tr>
	<tr>
		<td>S2C</td>
		<td>setpass</td>
		<td>password</td>
		<td>修改密码</td>
	</tr>
</table>

插件模块相关proto
=================