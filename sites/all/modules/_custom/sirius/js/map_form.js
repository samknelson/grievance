(function ($) {
	Drupal.behaviors.sirius_map_form = {
	  attach: function (context) { 

	  	function sirius_map_form_success(position) {
	  		$('#sirius_map_current_location_lat').val(position.coords.latitude);
	  		$('#sirius_map_current_location_lon').val(position.coords.longitude);
				$('#sirius_map_current_location_ts').val(position.timestamp);
	  	}

	  	function sirius_map_form_failure(error) {
	  		$('#sirius_map_current_location_err').val(error.message);
	  	}

	  	navigator.geolocation.getCurrentPosition(sirius_map_form_success, sirius_map_form_failure);
	  }
	};
})(jQuery);
