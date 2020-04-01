(function ($) {
	Drupal.behaviors.esignature = {
	  attach: function (context) {

	  	if ($('#sirius_esig_data').length) { 
				$('#sirius_esig_data').parent().hide();
				$('#sirius_esig_submit').prop('disabled', true);

				var sirius_esig = $('#sirius_esig').signature({
					syncField: sirius_esig_data,
					syncFormat: 'PNG',
	  			change: function(event, ui) { 
	  				$('#sirius_esig_submit').prop('disabled', false);
	  			}
				});

				$('#sirius_esig_clear').click(function(event) {
					sirius_esig.signature('clear');
					$("#sirius_esig_data").val('');
					$("#sirius_esig").removeClass('sirius_esignature_pane_accepted');
					$('#sirius_esig_submit').prop('disabled', true);
					return false;
				});

				$('#sirius_esig_submit').click(function() {
					if (sirius_esig.signature('isEmpty')) {
						alert('Please sign in the space provided.');
						return false;
					}
					$("#sirius_esig").addClass('sirius_esignature_pane_accepted');
				});
			}
	  }
	};
})(jQuery);
