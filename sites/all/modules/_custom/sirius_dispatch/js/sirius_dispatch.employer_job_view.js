(function($) { $(document).ready(function () {

	// Refresh immediately and every five seconds forever
	sirius_dispatch_refresh();
	setInterval(sirius_dispatch_refresh, 20000);


	function sirius_dispatch_refresh() {
		job_nid = Drupal.settings.sirius_dispatch.job_nid;
		console.log("Refreshing");
		$('#sirius_dispatch_ajax_status').html('... Refreshing ...');

		$.ajax({
        	'url': '/sirius/employer/dispatch/job/view/ajax/' + job_nid,
        	'type': 'GET',
			'success': function(data) {
            	$('#sirius_dispatch_ajax_body').html(data);
        	}
        });

		var d = new Date();
		$('#sirius_dispatch_ajax_status').html('Refreshed at ' + d.toLocaleTimeString());
    }

}); }(jQuery));