(function ($) {
	Drupal.behaviors.sirius_event_map_list = {
	  attach: function (context) { if (context === document) {

	  	// Event NID
	  	var event_nid = Drupal.settings.sirius_map.event_nid;

	  	// Toggle
	  	$('.map_list_toggle').click(function(e) {
	  		e.preventDefault();
	  		pstatus_tid = $(this).attr('data-pstatus_tid');
	  		
	  		if ($(this).hasClass('inactive')) {
	  			$(this).removeClass('inactive');
		  		$('.pstatus_' + pstatus_tid).show();
	  		} else {
	  			$(this).addClass('inactive');
		  		$('.pstatus_' + pstatus_tid).hide();
	  		}
	  	});

	  	// Do an ajax refresh poll of the map
	  	function sirius_map_poll() {
	  		$('#sirius_map_poll_message').html('... refreshing the map ...');
				$.ajax({
					'url': '/sirius/ajax/event/map/poll/' + event_nid,
					'type': 'GET',
		  		'dataType': 'json',
		  		'error': function(jqXHR, textStatus, errorThrown) {
			  		$('#sirius_map_poll_message').html('Map refreshed failed: ' + textStatus + ': ' + errorThrown);
		  		},
					'success': function(data) {
						if (data['updates']) {
							for (i = 0; i < data['updates'].length; i++) {
								record = data['updates'][i];
							}
						}
						date = new Date();
						n = date.toDateString();
						time = date.toLocaleTimeString();
			  		$('#sirius_map_poll_message').html('Map refreshed at: ' + time);
					}
				});
	  	}

	  	// Manual poll
	  	$('#sirius_map_poll').click(function(event) {
	  		event.preventDefault();
	  		sirius_map_poll();
	  		// jiggle(markers[2313681]);
      });

      // Auto poll
      if (Drupal.settings.sirius_map.poll && Drupal.settings.sirius_map.poll.active) {
      	timeout = Drupal.settings.sirius_map.poll.timeout;
      	if (!timeout) { timeout = 60000; }
      	console.log("Polling started with timeout of " + timeout);
      	setInterval(sirius_map_poll, timeout);
      }

      if (Drupal.ajax) {
		    Drupal.ajax.prototype.commands.sirius_command_map_update = function(ajax, response, status) {
		    	$('#participant_' + response.participant_nid + ' .icon_wrap img').attr('src', response.icon_url);

		    	new_class = 'pstatus_' + response.pstatus_tid;
		    	classes = $('#participant_' + response.participant_nid).attr('class').split(/\s+/);
					$.each(classes, function(index, value) {
						if (value == new_class) { return; }
						if (! (value.startsWith('pstatus_'))) { 
							return;
						}
						$('#participant_' + response.participant_nid).removeClass(value);
	        });

		    	$('#participant_' + response.participant_nid).addClass('pstatus_' + response.pstatus_tid);
		    }
		  }

	  } }
	};
})(jQuery);
