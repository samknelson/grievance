(function ($) {
	Drupal.behaviors.sirius_map = {
	  attach: function (context) { if (context === document) {

	  	// Event NID
	  	var event_nid = Drupal.settings.sirius_map.event_nid;

	  	// Our pin library
	  	pins = new Array();
	  	if (Drupal.settings.sirius_map.pins) {
				for (const pin_key in Drupal.settings.sirius_map.pins) { 
					pin = Drupal.settings.sirius_map.pins[pin_key];
					pins[pin_key] = L.icon(
						{
							iconUrl: pin.path,
							iconAnchor: [pin.iconAnchor.x,pin.iconAnchor.y],
							popupAnchor: [pin.popupAnchor.x,pin.popupAnchor.y],
							tooltipAnchor: [pin.tooltipAnchor.x,pin.tooltipAnchor.y],
						}
					);
				}
			}

	  	// React when a feature is added.
	  	var markers = {};
			$(document).on('leaflet.feature', function (e, lfeature, feature) {
				if (feature.sirius && feature.sirius.marker.id) {
					markers[feature.sirius.marker.id] = lfeature;
					// For some reason, the feature doesn't set all the pin options correctly. So screw it will do it again here.
					markers[feature.sirius.marker.id].setIcon(pins[feature.pin_key]);
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

				/*
				console.log(markers);
				console.log(map);
				var aLayerGroup = L.layerGroup(markers[2411531], markers[2412025], markers[2412536]);
				var overlays = {
					"My Markers": aLayerGroup,
				}
				aLayerGroup.addTo(map);
				layerControl = L.control.layers(null, overlays);
				layerControl.addTo(map);
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
            	current_location_marker = L.marker([e.latitude, e.longitude], {icon: pins[Drupal.settings.sirius_map.current_location.pin_key]});
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
					    	marker.setIcon(pins[record.pin_key]);
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
		    	marker.setIcon(pins[response.pin_key]);
		    	marker._popup.setContent(response.popup);
		    }
		  }

	  } }
	};
})(jQuery);
