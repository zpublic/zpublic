var start_sig = /(class\s+GameWindowControl)/;
var gen_start = '// <GENERATE id="enum">\n';
var gen_end = '// </GENERATE>\n';
var gen_comment = '// Auto Generated section, do not edit !!';
var gen_sig = /\s*\/\/ <GENERATE id="enum">[\s\S]+?\/\/ <\/GENERATE>\s*/;

module.exports = function (header, window_def){
	var enum_source = '\n\n' + gen_start + gen_comment + '\n';
	enum_source += '\t#define GWIN_ID_SUCCESS_EXIT 60000\n';
	enum_source += '\t#define GWIN_ID_EXIT 60001\n';
	for(var n in window_def){
		var info = window_def[n];
		enum_source += '\t#define ' + info.id + ' ' + (info.index + 61000) + '\n';
	}
	enum_source += gen_end + '\n\n';

	header = header.replace(gen_sig, '');
	header = header.replace(start_sig, enum_source + '$1');
	return header;
};
