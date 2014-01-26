define(['jquery', 'bs-alert'], function (){
	"use strict";
	$.fn.ajaxSubmit = function (){
		if(this.get(0).nodeName != 'FORM'){
			return false;
		}
		var notify = this.data('notify');
		if(undefined === notify){
			notify = this.find('.alert.alert-info');
			if(!notify.length){
				notify = false;
			}
			this.data('notify', notify);
		}

		var that = this;

		var act = this.data('action');
		if(!act){
			act = this.attr('action');
			this.data('action', act);
		}
		function mute(){
			return false;
		}

		if(notify){
			notify.text('正在请求').alert('show').alert('info');
		}
		var data = this.serialize();

		var controls = this.on('click', 'a,input,button', mute)
				.find('input,button').filter(':not(.disabled):not([disabled])')
				.attr('disabled', 'disabled');
		var ret = $.ajax({
			url     : act,
			dataType: 'json',
			method  : this.attr('method'),
			data    : data,
			context : this
		});
		ret.then(function (json){
			controls.removeAttr('disabled');
			this.off('click', mute);
			var e = new $.Event('submitAjax');
			that.trigger(e, json);
			if(notify){
				if(e.isDefaultPrevented() || e.isPropagationStopped()){
					notify.alert('hide');
				} else if(json.code === 0){
					notify.text(json.message || '成功！')
							.alert('success').alert('hide');
				} else{
					notify.text(json.message || '服务器错误，不明觉厉。')
							.alert('warning').alert('hide');
				}
			}
		}, function (obj, stat, msg){
			controls.removeAttr('disabled');
			this.off('click', mute);
			if(notify){
				notify.text('HTTP ' + obj.status + ' ' + msg + '<br/>' + '请检查网络！')
						.alert('danger').alert('hide');
			}
		});
		return ret;
	};

	return function (){
		var no_ajax_submit = false;
		var frmCnt = $('form[type=ajax]').removeAttr('type')
				.submit(function (e){
					if(no_ajax_submit){
						return true;
					}
					$(this).ajaxSubmit();
					return false;
				}).length;
		if(frmCnt){
			window.disableAutoAjax = function (arg){
				no_ajax_submit = arg !== false;
			}
		}
	};
});
