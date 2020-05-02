(function($) { $(document).ready(function () {
	$('#page-banner').hide();
	$('#header').hide();
	$('.banner-menu-outer-wrapper').hide();
	$('.secondary-tasks-wrapper').hide();
	$('.primary-local-tasks-wrapper').hide();
	$('#page-footer').hide();
	console.log("Here.");
	/*
	participant_nid = Drupal.settings.sirius_event_phonebank.participant_nid;
	console.log("Phone bank for " + participant_nid);

	// sirius_event_phonebank_survey
	$('#sirius_event_phonebank_survey select').change(function(event) {
		sirius_event_phonebank_submit_ajax();
	});
	$('#sirius_event_phonebank_survey input').blur(function(event) {
		sirius_event_phonebank_submit_ajax();
	});
	$('#sirius_event_phonebank_survey textarea').blur(function(event) {
		sirius_event_phonebank_submit_ajax();
	});

	function sirius_event_phonebank_submit_ajax() {
		url = '/sirius/event/phonebank_submit_ajax';
		values = $( "input, textarea, select").serialize();
		console.log(values);
		$.ajax({
		  type: "POST",
		  url: url,
		  data: values,
		}).done(function( data ) {
			console.log("Ajax request returned: " + data);
	  });
	}
	*/
}); }(jQuery));

