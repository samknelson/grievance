(function ($) {
	Drupal.behaviors.sirius_showhide = {
	  attach: function (context) { if (context === document) {

	  	// @todo: it's dumb to have "toggle" and "toggle_byclass", with different scripts.
	  	$('.sirius_showhide_toggle').click(function(event) {
      	event.preventDefault();
      	target_id = $(this).data('sirius_showhide_target');
      	if ($('#' + target_id).is(':visible')) {
      		$('#' + target_id).hide();
      		$('#' + target_id).addClass('sirius_showhide_target_hidden');
      		$(this).html('show');
      	} else {
      		$('#' + target_id).show();
      		$('#' + target_id).removeClass('sirius_showhide_target_hidden');
      		$(this).html('hide');
      	}
      	return false;
      });

	  	$('.sirius_showhide_toggle_byclass').click(function(event) {
      	event.preventDefault();
      	target_sel = '.' + $(this).data('sirius_showhide_target');
	  		console.log("I was clicked: " + target_sel);

      	if ($(this).hasClass('sirius_showhide_open')) {
      		$(target_sel).hide();
      		$(target_sel).addClass('sirius_showhide_target_hidden');
      		$(this).removeClass('sirius_showhide_open');
      		$(this).html('show');
      	} else {
      		$(target_sel).show();
      		$(target_sel).removeClass('sirius_showhide_target_hidden');
      		$(this).addClass('sirius_showhide_open');
      		$(this).html('hide');
      	}
      	return false;
      });


	  } }
	};
})(jQuery);
