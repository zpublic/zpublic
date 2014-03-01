var fs = require('fs');
var path = require('path');

var coeus_client_path = path.resolve(__dirname, '../../client');
console.log('source path: %s', coeus_client_path);

var dlgFile = /game_(.+)_dlg\.h/i;
var list = [];
fs.readdirSync(coeus_client_path).forEach(function (file){
	var mat = dlgFile.exec(file);
	if(mat){
		list.push(mat[1]);
	}
});

var def = {}, dlgClass = /class\s+([a-z]+)/i;
list.forEach(function (name, index){
	var info = {index: index, name: name};
	info.file = 'game_' + name + '_dlg.h';
	info.path = path.resolve(coeus_client_path, info.file);
	info.id = 'GWIN_ID_' + name.toUpperCase();
	var content = fs.readFileSync(info.path);
	var mats = dlgClass.exec(content);
	if(mats){
		info.class = mats[1];
	} else{
		throw new SyntaxError('Cannot parse dialog file name.');
	}
	console.log("GameWindow[%d]? %s (%s)\n\tID=%s\n\tclass=%s\n", info.index, name, info.file, info.id, info.class);
	def[name] = info;
});

console.log('process header file...');
var header_path = path.resolve(coeus_client_path, 'window_manager.h'),
		header_content = fs.readFileSync(header_path).toString();

console.log('\tenum...');
header_content = require(__dirname + '/subproc_lib/wm_header_enum_gen.js')(header_content, def);
console.log('\tclass property & method...');
header_content = require(__dirname + '/subproc_lib/wm_header_class_parse.js')(header_content, def);
fs.writeFileSync(header_path, TabFilter(header_content));

console.log('process source file...');
var source_content = '#include "stdafx.h"\n' +
                     '#include "window_manager.h"\n';
for(var n in def){
	var info = def[n];
	source_content += '#include "' + info.file + '"\n';
}
source_content += '\n';

console.log('\tfactory method...');
source_content += require(__dirname + '/subproc_lib/wm_source_factory.js')(def);

var source_path = path.resolve(coeus_client_path, 'window_manager.cpp');
fs.writeFileSync(source_path, TabFilter(source_content));

function TabFilter(str){
	return str.replace(/\t/g, '    ');
}
