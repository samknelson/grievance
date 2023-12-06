(function ($) {
	Drupal.behaviors.sirius_ux = {
		attach: function (context) {
		}
	};

	Drupal.behaviors.sirius_ux.flash = function(msg, priority) {
		div = $('#sirius_ux_flash_wrapper');
		if (!div.length) {
			var div = $('<div />').appendTo('body');
			div.attr('id', 'sirius_ux_flash_wrapper');

			div.click(function() {
				div.hide(1000);
			});
		}

		div.show();
		div.html(msg);
		div.removeClass();
		div.addClass('sirius_ux_flash_' + priority);
	}
})(jQuery);
