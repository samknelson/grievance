(function($) {
	function grievance_clause_picker_change_edit_to_pick() {
		// Only do this on popup windows.
		if (!window.location.pathname.includes('popup')) { return; }

		$('.tool-picker').html('Pick');

		$('.tool-picker').click(function(e) {
			e.preventDefault();
			path = $(this).attr('href');
			path_parts = path.split('/');
			nid = path_parts[2];
			if (window.opener) {
				window.opener.sirius_clause_pick(nid);
			}
		});
	}

	$(document).ready(function () {

		// Run on page load
		grievance_clause_picker_change_edit_to_pick();
	});

	$(document).ajaxComplete(function(){
		// run on ajax callback
		grievance_clause_picker_change_edit_to_pick();
	});

}(jQuery));
