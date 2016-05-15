(function ($) {

	// This should move into the grievance module.  But for now....
  Drupal.behaviors.grievance = {
    attach: function(context, settings) {
			$('#grievance-form-contacts-link .grievance-recipient').click(function() { grievance_handle_recipient_click($(this)); });

			function grievance_handle_recipient_click(elt) {
				val = elt.find('.field-name-field-grievance-co-name .field-item').html();
				$('#edit-field-grievance-co-name-und-0-value').val(val);

				val = elt.find('.field-name-field-grievance-co-email .field-item').html();
				$('#edit-field-grievance-co-email-und-0-email').val(val);

				val = elt.find('.field-name-field-grievance-co-phone .field-item').html();
				$('#edit-field-grievance-co-phone-und-0-value').val(val);

				val = elt.find('.field-name-field-grievance-co-address .field-item').html();
				$('#edit-field-grievance-co-address-und-0-value').val(val);

				val = elt.find('.field-name-field-grievance-co-address-2 .field-item').html();
				$('#edit-field-grievance-co-address-2-und-0-value').val(val);

				val = elt.find('.field-name-field-grievance-co-city .field-item').html();
				$('#edit-field-grievance-co-city-und-0-value').val(val);

				val = elt.find('.field-name-field-grievance-co-state .field-item').html();
				$('#edit-field-grievance-co-state-und-0-value').val(val);

				val = elt.find('.field-name-field-grievance-co-zip .field-item').html();
				$('#edit-field-grievance-co-zip-und-0-value').val(val);
			};

    },
  }


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


