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



		$('.grievance-recipient .form-type-radios input').change(function () {

			$(this).closest('.grievance-recipient').removeClass('grievance-recipient-none');
			$(this).closest('.grievance-recipient').removeClass('grievance-recipient-primary');
			$(this).closest('.grievance-recipient').removeClass('grievance-recipient-secondary');
			$(this).closest('.grievance-recipient').removeClass('grievance-recipient-secret');


			$(this).closest('.grievance-recipient').addClass('grievance-recipient-' + this.value);
    });

	}); // END: DRF
}(jQuery));


