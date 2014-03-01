var start_sig = /class\s+GameWindowControl/;
var gen_start = '\t// <GENERATE id="property">\n';
var gen_end = '\t// </GENERATE>\n';
var gen_comment = '\t// Auto Generated section, do not edit !!';
var gen_sig = /\s*\/\/ <GENERATE id="property">[\s\S]+?\/\/ <\/GENERATE>/;

module.exports = function (header, window_def){
	header = header.replace(gen_sig, '');
	var mats = start_sig.exec(header);
	var start = mats.index;
	var copy = header;
	copy = copy.replace(/("|')(.*?)\1/g, function (mats){
		return (new Array(mats.length+1).join('"'));
	});
	copy = copy.replace(/\/\/(.*?)$/gm, function (mats){
		return (new Array(mats.length+1).join('/'));
	});
	copy = copy.replace(/\/\*(.*?)\*\//gm, function (mats){
		return (new Array(mats.length+1).join('*'));
	});
	var level = 0, pos;
	//var test, esc = String.fromCharCode(27);
	do{
		var ileft = copy.indexOf('{', start);
		var iright = copy.indexOf('}', start);

		/*test = copy;
		if(ileft > 0){
			test = test.substr(0, ileft) + esc + '[38;5;9m{' + esc + '[0m' + test.substr(ileft + 1);
		}
		if(iright > 0){
			var testr = test.indexOf('}', start);
			test = test.substr(0, testr) + esc + '[38;5;10m}' + esc + '[0m' + test.substr(testr + 1);
		}
		console.log(test);*/

		if(iright < 0){
			console.error('Cannot process header file. no close brace found.');
			process.exit(1);
		}
		if(ileft < 0){
			ileft = iright;
		}

		if(ileft < iright){
			level++;
			start = ileft + 1;
		} else{
			level--;
			start = iright + 1;
		}
		// console.log('LEVEL = %d\n==========================================', level)
		if(level == 0){
			pos = start;
			break;
		}
	} while(ileft > 0 && iright > 0);
	if(!pos){
		console.error('Cannot process header file. cannot find class define.');
		process.exit(1);
	}

	var generate = '\tpublic:\n';
	for(var i in window_def){
		var info = window_def[i];
		generate += '\t\tinline static class ' + info.class + '* get_' + info.name + '_dialog();\n';
		generate += '\t\tstatic void destroy_' + info.name + '_dialog();\n';
	}
	generate += '\n\tprivate:\n';
	for(i in window_def){
		info = window_def[i];
		generate += '\t\tstatic class ' + info.class + '* m_dlg_' + info.name + ';\n';
	}

	return header.substr(0, pos - 1) + '\n' + gen_start + gen_comment + '\n'
			       + generate + gen_end + header.substr(pos - 1);
};
