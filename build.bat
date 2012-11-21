@echo off 

@echo ================	Build Client	=================
"%VS11PATH%\devenv.com" .\trunk\sln\zpublic.sln /rebuild "Release"
if ERRORLEVEL 1 exit /b 1
@echo ================	End~~	=================
@exit /b 0
