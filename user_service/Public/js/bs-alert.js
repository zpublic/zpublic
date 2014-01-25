define(['jquery'],function($){
	$.fn.alert.Constructor.prototype.success = function (){
		return this.removeClasses('alert-.*').addClass('alert-success');
	};
	$.fn.alert.Constructor.prototype.warning = function (){
		return this.removeClasses('alert-.*').addClass('alert-warning');
	};
	$.fn.alert.Constructor.prototype.danger = function (){
		return this.removeClasses('alert-.*').addClass('alert-danger');
	};
	$.fn.alert.Constructor.prototype.show = function (){
		return this.slideDown('fast');
	};
	$.fn.alert.Constructor.prototype.info = function (){
		return this.removeClasses('alert-.*').addClass('alert-info');
	};
	$.fn.alert.Constructor.prototype.hide = function (){
		var cls = this[0].className, t = 2000;
		if(this[0].hidetimer){
			clearTimeout(this[0].hidetimer);
		}
		if(/alert-success/.test(cls)){
			t = 1000;
		} else if(/alert-warning/.test(cls)){
			t = 4000;
		} else if(/alert-info/.test(cls)){
			t = 2000;
		} else if(/alert-danger/.test(cls)){
			return this.prepend('<a class="close" href="#" aria-hidden="true" onclick="$(this).parent().slideUp(\'fast\');return false;">&times;</a>');
		}
		var self = this;
		this[0].hidetimer = setTimeout(function (){
			self.slideUp('fast');
			self = null;
		}, t);
		return this;
	};
});
