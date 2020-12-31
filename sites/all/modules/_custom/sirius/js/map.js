(function ($) {
	Drupal.behaviors.sirius_map = {
	  attach: function (context) { if (context === document) {

	  	// Event NID
	  	var event_nid = Drupal.settings.sirius_map.event_nid;

	  	// Set a bunch of icons
	  	icons = new Array();
	  	icons['current_location'] = L.icon({iconUrl: '/sites/all/modules/_custom/sirius/images/map/circle-24.png'});
	  	icons['Blue'] = L.icon({iconUrl: '/sites/all/modules/_custom/sirius/images/map/marker-icon-blue.png'});
	  	icons['Black'] = L.icon({iconUrl: '/sites/all/modules/_custom/sirius/images/map/marker-icon-black.png'});
	  	icons['Gold'] = L.icon({iconUrl: '/sites/all/modules/_custom/sirius/images/map/marker-icon-gold.png'});
	  	icons['Green'] = L.icon({iconUrl: '/sites/all/modules/_custom/sirius/images/map/marker-icon-green.png'});
	  	icons['Grey'] = L.icon({iconUrl: '/sites/all/modules/_custom/sirius/images/map/marker-icon-grey.png'});
	  	icons['Orange'] = L.icon({iconUrl: '/sites/all/modules/_custom/sirius/images/map/marker-icon-orange.png'});
	  	icons['Red'] = L.icon({iconUrl: '/sites/all/modules/_custom/sirius/images/map/marker-icon-red.png'});
	  	icons['Violet'] = L.icon({iconUrl: '/sites/all/modules/_custom/sirius/images/map/marker-icon-violet.png'});
	  	icons['Yellow'] = L.icon({iconUrl: '/sites/all/modules/_custom/sirius/images/map/marker-icon-yellow.png'});

	  	// React when a feature is added.
	  	var markers = new Array();
			$(document).on('leaflet.feature', function (e, lfeature, feature) {
				if (feature.sirius && feature.sirius.marker.id) {
					markers[feature.sirius.marker.id] = lfeature;
					if (feature.tooltip) { 
						lfeature.bindTooltip(feature.tooltip, feature.tooltip_options);
					}
				}
			});

			// React whena  map is added
			$(document).on('leaflet.map', function (e, map_settings, map, map_id) {
				// Don't know why this doesn't get passed in
				map_id = map._container.id;

				// Add the print control
				/*
				L.easyPrint({
					title: 'Print',
					position: 'topleft',
					sizeModes: ['A4Portrait', 'A4Landscape']
				}).addTo(map);
				*/

				// Handle modal links within marker popups. Normally we wouldn't bother, Drupal does this for free. 
				// But the HTML for the popup is rendered dynamically, so Drupal doesn't get a chance to handle it. So we
				// do some ugliness.
				$('#' + map_id).on('click', '.modal_link', function(event) {
					event.preventDefault();
					if (map.isFullscreen()) { map.toggleFullscreen(); }
					map.fire('onExitFullScreen');
					modal_url = $(this).attr('href');
					link = $("<a></a>").attr('href', modal_url).addClass('ctools-use-modal-processed').click(Drupal.CTools.Modal.clickAjaxLink);
					Drupal.ajax[modal_url] = new Drupal.ajax(modal_url, link.get(0), { url: modal_url, event: 'click', progress: { type: 'throbber' } });
					link.click();
				});

				// Handle current location. @todo this code is ugly.
		  	if (Drupal.settings.sirius_map.current_location && Drupal.settings.sirius_map.current_location.show) {
					var current_location_marker;
					map.locate({setView: false, watch: true})
        		.on('locationfound', function(e){
        			// Mark current location
        			if (current_location_marker) { map.removeLayer(current_location_marker); }
            	current_location_marker = L.marker([e.latitude, e.longitude], {icon: icons['current_location']});
            	map.addLayer(current_location_marker);

            	// Checkin
            	sirius_map_checkin(e);
        		})
       			.on('locationerror', function(e){
       				$('#sirius_map_poll_message').html(e.message);
        		});
		  	}
		  });

			// Send our location back to the server
			var last_checkin_ts;
		  function sirius_map_checkin(e) {
		  	// Don't check in unless it's in the settings
		  	if (!Drupal.settings.sirius_map.current_location) { return; }
      	if (!Drupal.settings.sirius_map.current_location.checkin) { return; }
      	if (!Drupal.settings.sirius_map.current_location.checkin.active) { return; }

      	// Don't check in too often
      	timeout = Drupal.settings.sirius_map.current_location.checkin.timeout;
      	if (!timeout) { timeout = 1000 * 60; }
    		if (last_checkin_ts + timeout > Date.now()) { return; } 
    		last_checkin_ts = Date.now();

    		// Checkin
				$.ajax({
					'url': '/sirius/ajax/map/current_location/checkin',
					'type': 'POST',
		  		'dataType': 'json',
		  		'data': {'location': {'lat': e.latitude, 'lon': e.longitude} },
		  		'error': function(jqXHR, textStatus, errorThrown) {
		  			console.log('Current location checkin failed: ' + textStatus + ': ' + errorThrown);
		  		},
					'success': function(data) {
		  			console.log('Current location complete: ' + data['msg']);
					}
				});
			}

			// A stupid utility function to make a random change in a marker.
	  	function jiggle(marker) {
	  		which = Math.random();
	  		if (which < 0.2) { icon = icons['Blue']; }
	  		else if (which < 0.4) { icon = icons['Black']; }
	  		else if (which < 0.6) { icon = icons['Gold']; }
	  		else if (which < 0.8) { icon = icons['Green']; }
	  		else  { icon = icons['grey']; }
	  		marker.setIcon(icon);
	  		marker._popup.setContent('I was jiggled by ' + which);
	  	}

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
					    	marker = markers[record.participant_nid];
					    	if (!marker) { continue; }
					    	marker.setIcon(icons[record.icon_name]);
					    	marker._popup.setContent(record.popup);
    						marker.setLatLng(new L.LatLng(record.lat, record.lon));
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
		    	marker = markers[response.participant_nid];
		    	if (!marker) { return; }
		    	marker.setIcon(icons[response.icon_name]);
		    	marker._popup.setContent(response.popup);
		    }
		  }

	  } }
	};
})(jQuery);
