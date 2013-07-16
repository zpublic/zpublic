@echo off
set protocol_path=./protocol
set proto_files_path=./protocol/proto_files

if not exist %proto_files_path%/*.proto (echo No proto files in directory '%proto_files_path%' ) && pause && exit
protoc.exe -I=%proto_files_path% --cpp_out=%protocol_path% %proto_files_path%/*.proto
echo Generated finished, output dir '%protocol_path%'