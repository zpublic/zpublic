module.exports = function (window_def){
	var source = '';
	var n, info;
	for(n in window_def){
		info = window_def[n];
		source += info.class + '* GameWindowControl::m_dlg_' + info.name + ' = NULL;\n';
	}
	source += '\n';

	source += 'void* GameWindowControl::create_dialog(const int &type)\n{\n' +
	          '\tswitch(type){\n';

	for(n in window_def){
		info = window_def[n];
		source += '\tcase ' + info.id + ':\n' +
		          '\t\treturn (void*)get_' + info.name + '_dialog();\n';
	}
	source += '\t}\n\treturn NULL;\n}\n\n';

	source += 'UINT_PTR GameWindowControl::start(const int &type)\n{\n' +
	          '\tswitch(type){\n';

	for(n in window_def){
		info = window_def[n];
		source += '\tcase ' + info.id + ':\n' +
		          '\t\treturn get_' + info.name + '_dialog()->DoModal();\n';
	}
	source += '\t}\n\treturn GWIN_ID_EXIT;\n}\n\n';

	for(n in window_def){
		info = window_def[n];
		console.log('\t[CLASS]::get_%s_dialog()', info.name);
		source += info.class + '* GameWindowControl::get_' + info.name + '_dialog(){\n' +
		          '\tif(m_dlg_' + info.name + ' == NULL){\n' +
		          '\t\tm_dlg_' + info.name + ' = new ' + info.class + '();\n' +
		          '\t}\n' +
		          '\treturn m_dlg_' + info.name + ';\n' +
		          '}\n\n';
		console.log('\t[CLASS]::destroy_%s_dialog()', info.name);
		source += 'void GameWindowControl::destroy_' + info.name + '_dialog(){\n' +
		          '\tif(m_dlg_' + info.name + ' != NULL){\n' +
		          '\t\tdelete m_dlg_' + info.name + ';\n' +
		          '\t\tm_dlg_' + info.name + ' = NULL;\n' +
		          '\t}\n' +
		          '}\n\n';
	}

	return source;
};

