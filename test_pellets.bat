@echo off
@echo =
@echo =
copy ".\trunk\sln\Release\test_pellets.exe" ".\" > out1.txt
@echo =
@echo =
".\test_pellets.exe"
@echo =
@echo =
@exit /b 0