(function ($) {
	Drupal.behaviors.sirius_map_form = {
	  attach: function (context) { 

	  	function sirius_map_form_success(position) {
	  		$('#sirius_map_current_location_lat').val(position.coords.latitude).trigger('change');
	  		$('#sirius_map_current_location_lon').val(position.coords.longitude).trigger('change');
				$('#sirius_map_current_location_ts').val(position.timestamp).trigger('change');
	  	}

	  	function sirius_map_form_failure(error) {
	  		$('#sirius_map_current_location_err').val(error.message).trigger('change');
	  	}

	  	navigator.geolocation.getCurrentPosition(sirius_map_form_success, sirius_map_form_failure);
	  }
	};
})(jQuery);
