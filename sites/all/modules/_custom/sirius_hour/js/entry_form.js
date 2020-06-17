(function ($) {
	Drupal.behaviors.sirius_hour_entry_form = {
	  attach: function (context) {

	  	console.log('Boo.');

	  	$('.sirius_table_detailrow_toggle').click(function(event) {
	  		event.preventDefault();
	  		target = $(this).parents('tr').next('tr.sirius_table_detailrow').find('div.sirius_table_detailrow_details');
	  		if (target.is(':visible')) {
	  			target.hide();
	  			$(this).html('show details');
	  		} else {
	  			target.show();
	  			$(this).html('hide details');
	  		}
      });
	  }
	};
})(jQuery);
