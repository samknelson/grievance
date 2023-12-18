(function ($) {
	Drupal.behaviors.sirius_ux = {
		attach: function (context) {
		}
	};

	Drupal.behaviors.sirius_ux.flash = function(msg, priority) {
		function toggle_latest() {
			if ($('#sirius_ux_flash_wrapper').find('.sirius_ux_flash_latest').is(':visible')) { hide_latest(); } else { show_latest(); }
		}

		function show_latest() {
			$('#sirius_ux_flash_wrapper').find('.sirius_ux_flash_latest').show(500);
			$('#sirius_ux_flash_wrapper').find('.sirius_ux_flash_latest_toggle').html('<i class="far fa-minus-square"></i>');
		}

		function hide_latest() {
			$('#sirius_ux_flash_wrapper').find('.sirius_ux_flash_latest').hide(500);
			$('#sirius_ux_flash_wrapper').find('.sirius_ux_flash_latest_toggle').html('<i class="far fa-plus-square"></i>');

			hide_log();
		}

		function toggle_log() {
			if ($('#sirius_ux_flash_wrapper').find('.sirius_ux_flash_log').is(':visible')) { hide_log(); } else { show_log(); }
		}

		function show_log() {
			$('#sirius_ux_flash_wrapper').find('.sirius_ux_flash_log').show(500);
			$('#sirius_ux_flash_wrapper').find('.sirius_ux_flash_log_toggle').html('<i class="far fa-minus-square"></i>');
		}

		function hide_log() {
			$('#sirius_ux_flash_wrapper').find('.sirius_ux_flash_log').hide(500);
			$('#sirius_ux_flash_wrapper').find('.sirius_ux_flash_log_toggle').html('<i class="far fa-plus-square"></i>');
		}

		div_wrapper = $('#sirius_ux_flash_wrapper');
		if (!div_wrapper.length) {
			var div_wrapper = $('<div />').appendTo('body');
			div_wrapper.attr('id', 'sirius_ux_flash_wrapper');
			div_wrapper.addClass('sirius_noprint');

			html = '<div class="sirius_ux_flash_log" style="display: none;"></div>';

			html += '<div class="sirius_ux_flash_latest">';
			html += '<span class="sirius_ux_flash_msg"></span>';
			html += '</div>';

			html += '<div class="sirius_ux_flash_ctrl sirius_ux_flash_log_toggle"><i class="far fa-plus-square"></i></div>';
			html += '<div class="sirius_ux_flash_ctrl sirius_ux_flash_latest_toggle"><i class="far fa-minus-square"></i></div>';

			div_wrapper.html(html);

			div_wrapper.find('.sirius_ux_flash_latest_toggle').click(function() { toggle_latest(); });
			div_wrapper.find('.sirius_ux_flash_log_toggle').click(function() { toggle_log(); });
		}

		show_latest();

		// Append to the log
		div_log = div_wrapper.find('.sirius_ux_flash_log');
		div_log.append('<div class="sirius_ux_flash_item">' + msg + '</div>');
		if (div_log.find('.sirius_ux_flash_item').size() > 100) {
			div_log.find('.sirius_ux_flash_item').first().remove();
		}

		// Don't replace "success" or "error" messages with "info" for at least 30 seconds
		if (priority == 'info') {
			last_update = Drupal.behaviors.sirius_ux.last_update;
			if (!last_update) { last_update = 0; }
			interval = Date.now() - last_update;
			if (interval < 30*1000) { return; }
		}

		Drupal.behaviors.sirius_ux.last_update = Date.now();

		div_latest = $('#sirius_ux_flash_wrapper').find('.sirius_ux_flash_latest');
		div_latest.find('.sirius_ux_flash_msg').html(msg);
		div_latest.removeClass('sirius_ux_flash_warning');
		div_latest.removeClass('sirius_ux_flash_error');
		div_latest.removeClass('sirius_ux_flash_info');
		div_latest.removeClass('sirius_ux_flash_success');
		div_latest.addClass('sirius_ux_flash_' + priority);
	}
})(jQuery);
