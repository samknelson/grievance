(function ($) {
	Drupal.behaviors.esignature = {
	  attach: function (context) {

	  	//
	  	// Popup details
	  	//

	  	$('.sirius_esignature_popup_trigger').click(function(event) {
	  		event.preventDefault();
        var left = Math.floor(($(window).width() - Drupal.settings.sirius_esignature.popup_width) / 2);
        var top = Math.floor(($(window).height() - Drupal.settings.sirius_esignature.popup_height) / 2);

        var overlay = $(this).parent().find('.sirius_esignature_popup_overlay');
        var announcement = $(this).parent().find('.sirius_esignature_popup_wrap');
        var close = $(this).parent().find('.sirius_esignature_popup_close');

        overlay.show();
        announcement.show().css({
          'width': Drupal.settings.sirius_esignature.popup_width,
          'height': 'auto',
          'position': 'fixed',
          'top': '10%',
          'left': '50%',
          'transform': 'translate(-50%, 0)',
        });

        // close announcement by keyboard or mouse
        close.click(function() {
          overlay.fadeOut();
          announcement.fadeOut();
        });
        overlay.click(function() {
          overlay.fadeOut();
          announcement.fadeOut();
        });
        $(document).keyup(function(e) {
          if (e.keyCode == 27) {
            overlay.fadeOut();
            announcement.fadeOut();
          }
        });
      });

      //
      // Render the interactive signature canvas
      //

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
