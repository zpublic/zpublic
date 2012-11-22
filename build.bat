@echo off 
@echo |
@echo ================	Build Client	=================
@echo |
@echo |
"%VS11PATH%\devenv.com" .\trunk\sln\zpublic.sln /rebuild "Release"
if ERRORLEVEL 1 exit /b 1
@echo |
@echo |
@echo ================	Copy File 	=================
@echo |
@echo |
copy .\trunk\sln\Release\test_basic.exe .\bin
@echo |
@echo |
@echo ================	Unit Test	=================
@echo |
@echo |
.\bin\test_basic.exe
@echo |
@echo |
@echo ================	End~~	=================
@echo |
@exit /b 0
