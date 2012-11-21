@echo off 

@echo ================	Build Client	=================
"%VS10PATH%\devenv.com" .\trunk\sln\zpublic.sln /rebuild "Release|Win32"
if ERRORLEVEL 1 exit /b 1
@echo ================	End~~	=================
@exit /b 0
