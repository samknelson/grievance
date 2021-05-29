(function ($) {
	Drupal.behaviors.sirius_autorefresh = {
	  attach: function (context) {

	  	// Event NID
	  	var settings = Drupal.settings.sirius_autoreresh;

	  	function sirius_autorefresh_msg(msg) {
	  		$('#sirius_autorefresh_msg').html(msg + ' / ');
	  	}

	  	if (settings['url'] && settings['html_id']) {
	  		interval = settings['interval'];
	  		if (!interval) { interval = 5*60; }
				setInterval(sirius_autorefresh_refresh, interval * 1000);
				// sirius_autorefresh_refresh();
				sirius_autorefresh_msg('Automatic refresh every <strong>' + settings['interval'] + '</strong> seconds');
	  	}

			function sirius_autorefresh_refresh() {
				url = settings['url'];
				html_id = settings['html_id'];
				interval = settings['interval'];
				console.log("Refreshing " + html_id + " with content from " + url);
				$('#sirius_autorefresh_msg').html('... Refreshing ...');
				$.ajax({
					'url': url,
					'type': 'GET',
					'success': function(data) {
		      	$('#' + html_id).html(data);
		    	},
		    	'error': function(jqXHR, textStatus, errorThrown) {
		    		console.log(jqXHR);
		    		console.log(textStatus);
		    		console.log(errorThrown);
		    		sirius_autorefresh_msg('Error: ' + errorThrown);
		    	}
		    });

				var d = new Date();
				sirius_autorefresh_msg('Automatic refresh every <strong>' + settings['interval'] + '</strong> seconds, last ' + d.toLocaleTimeString());
		  }

			$('#sirius_autorefresh_link_refreshnow').unbind('click').click(function(event) {
				event.preventDefault();
				sirius_autorefresh_refresh();
			});

	  }
	};
})(jQuery);
