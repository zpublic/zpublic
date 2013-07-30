
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
||*传输方向*||*包名*||*参数*||*作用*||
||C2S||auth||name||要求验证||
||S2C||auth||password||传输验证信息||
||C2S||login||return||验证通过或者失败||
||S2C||setname||name||修改rouji名||
||S2C||setpass||password||修改密码||

插件模块相关proto
=================