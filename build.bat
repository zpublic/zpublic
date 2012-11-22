@echo off 

@echo ================	Build Client	=================
"%VS11PATH%\devenv.com" .\trunk\sln\zpublic.sln /rebuild "Release"
if ERRORLEVEL 1 exit /b 1

@echo ================	Copy File 	=================
copy .\trunk\sln\Release\test_basic.exe .\bin

@echo ================	Unit Test	=================
.\bin\test_basic.exe

@echo ================	End~~	=================
@exit /b 0
