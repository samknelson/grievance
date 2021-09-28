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
	  		$('#sirius_map_poll_message').html('... refreshing the list ...');
	  		
				$.ajax({
					'url': '/sirius/ajax/event/map/poll/' + event_nid,
					'data': {
						'disable_group_by_address': true,
					},
					'type': 'GET',
		  		'dataType': 'json',
		  		'error': function(jqXHR, textStatus, errorThrown) {
			  		$('#sirius_map_poll_message').html('List refresh failed: ' + textStatus + ': ' + errorThrown);
		  		},
					'success': function(data) {
						if (data['updates']) {
							for (i = 0; i < data['updates'].length; i++) {
								record = data['updates'][i];
								sirius_map_list_update(record.marker_id, record.sirius.pstatus_tid, record.sirius.icon_url);
							}
						}
			  		$('#sirius_map_poll_message').html('List refreshed at: ' + new Date().toLocaleTimeString());
					}
				});
	  	}

	  	function sirius_map_list_update(marker_id, pstatus_tid, icon_url) {
	    	$('#marker_' + marker_id + ' .icon_wrap img').attr('src', icon_url);

	    	new_class = 'pstatus_' + pstatus_tid;
	    	if ($('#marker_' + marker_id).attr('class')) {
	    		classes = $('#marker_' + marker_id).attr('class').split(/\s+/);
					$.each(classes, function(index, value) {
						if (value == new_class) { return; }
						if (! (value.startsWith('pstatus_'))) { 
							return;
						}
						$('#marker_' + marker_id).removeClass(value);
	        });
				}

	    	$('#marker_' + marker_id).addClass('pstatus_' + pstatus_tid);
	  	}

	  	// Manual poll
	  	$('#sirius_map_poll').click(function(event) {
	  		event.preventDefault();
	  		sirius_map_poll();
      });

      // Auto poll
      if (Drupal.settings.sirius_map.poll && Drupal.settings.sirius_map.poll.active) {
      	timeout = Drupal.settings.sirius_map.poll.timeout;
      	if (!timeout) { timeout = 60000; }
      	console.log("Polling started with timeout of " + timeout);
      	setInterval(sirius_map_poll, timeout);
      }

      if (Drupal.ajax) {
	    	Drupal.ajax.prototype.commands.sirius_command_map_poll = function(ajax, response, status) {
	    		sirius_map_poll();
		    }
		  }


	  } }
	};
})(jQuery);
