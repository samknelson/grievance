(function ($) {
	Drupal.behaviors.sirius_showhide = {
	  attach: function (context) { if (context === document) {


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
      });
	  } }
	};
})(jQuery);
