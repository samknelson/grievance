(function ($) {
	Drupal.behaviors.sirius_pcct_provider_search_form = {
	  attach: function (context) { 

	  	// We need to enable or disable the "use my current location" option depending on whether the user has given us permission.

	  	$('#sirius_map_current_location_lat').change(function(){ sirius_pcct_provider_search_form_handle_geo(); });

	  	function sirius_pcct_provider_search_form_handle_geo() {
	  		if ($('#sirius_map_current_location_lat').val() || $('#sirius_map_current_location_lon').val()) {
	  			// Show the option
	  			console.log("Show :)");
	  			$('#filters_location_type option[value="latlon"]').prop('disabled', false);
	  		} else {
	  			// Hide the option
	  			$('#filters_location_type option[value="latlon"]').prop('disabled', true);
	  			$('#filters_location_type').val('address');
	  		}
	  	}

	  }
	};
})(jQuery);
