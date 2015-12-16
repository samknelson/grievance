(function ($) {
	jQuery(document).ready(function() {

		$('.showhide_toggle a').click(function(event) {
			event.preventDefault();
			
			id = $(this).attr('id');
			
			$('#' + id + '_a').toggle('fast');
			$('#' + id + '_b').toggle('fast');
			if ($(this).parent().hasClass("showhide_toggle_alt")) {
				$(this).parent().removeClass("showhide_toggle_alt");
			} else {
				$(this).parent().addClass("showhide_toggle_alt");
			}
			
			return false;
		});	
		
		$(".showhide_b").hide();

	}); // END: DRF
}(jQuery));


