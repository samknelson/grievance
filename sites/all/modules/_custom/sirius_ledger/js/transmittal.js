(function($) { $(document).ready(function () {
	// random_setting = Drupal.settings.sirius_ledger_tramsmittal.random_setting;

	$('.sirius_transmittal_autofill').click(function(e) {
		e.preventDefault();
		$('#' + $(this).attr('data-target')).val($(this).attr('data-amount'));
		return false;
	});

$('.sirius_transmittal_autofill_all').click(function(e) {
		e.preventDefault();
		$('.sirius_transmittal_autofill').each(function( index ) {
		  console.log("About to click #" +  index + ": " + $(this).attr('data-target'));
		  $(this).click();
		});		
		return false;
	});

}); }(jQuery));

