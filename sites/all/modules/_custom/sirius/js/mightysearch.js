(function ($) {
	Drupal.behaviors.mightysearch = {
	  attach: function (context) {

	  	$('.sirius_mightsearch_clausepart_clearlink a').click(function(event) {
	  		event.preventDefault();
	  		console.log(event);
	  		$(this).parents('.sirius_mightysearch_clause').find('input,select').val('');
      });
	  }
	};
})(jQuery);
