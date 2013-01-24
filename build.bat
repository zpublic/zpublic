@echo off 
@echo =
@echo ================	Build Client	=================
@echo =
@echo =
"%VS11PATH%\devenv.com" .\sln\zpublic.sln /rebuild "Release"
if ERRORLEVEL 1 exit /b 1
@echo =
@echo =
@echo ================	Copy File 	=================
@echo =
@echo =
copy .\trunk\sln\Release\test_basic.exe .\
copy .\trunk\sln\Release\test_pellets.exe .\
@echo =
@echo =
@echo ================	Unit Test	=================
@echo =
@echo =
".\test_basic.exe"
".\test_pellets.exe"
@echo =
@echo =
@echo ================	End~~	=================
@echo =
@exit /b 0
