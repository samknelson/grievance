(function ($) {

	$.fn.grievance_select_set = function(select_id, value) {
		// Don't set to empty
		if (!value) { return; }

		// Don't overwrite an existing selection
		old_value = $('#' + select_id).val();
		if (old_value && (old_value != '_none')) { return; }

		// Set the new value.
		$('#' + select_id).val(value);
	}

	// This should move into the grievance module.  But for now....
  Drupal.behaviors.grievance = {

    attach: function(context, settings) {
			$('#grievance-form-contacts-link .grievance-recipient').off('click').on('click', function() { grievance_handle_contact_click($(this)); });

			function grievance_handle_contact_click(elt) {
				val = elt.find('.field-name-field-grievance-co-name .field-item').html();
				$('.form-item-field-grievance-co-name-und-0-value input').val(val);

				val = elt.find('.field-name-field-grievance-co-role .field-item').html();
				$('.form-item-field-grievance-co-role-und-0-value input').val(val);

				val = elt.find('.field-name-field-grievance-co-email .field-item').html();
				$('.form-item-field-grievance-co-email-und-0-email input').val(val);

				val = elt.find('.field-name-field-grievance-co-phone .field-item').html();
				$('.form-item-field-grievance-co-phone-und-0-value input').val(val);

				val = elt.find('.field-name-field-grievance-co-address .field-item').html();
				$('.form-item-field-grievance-co-address-und-0-value input').val(val);

				val = elt.find('.field-name-field-grievance-co-address-2 .field-item').html();
				$('.form-item-field-grievance-co-address-2-und-0-value input').val(val);

				val = elt.find('.field-name-field-grievance-co-city .field-item').html();
				$('.form-item-field-grievance-co-city-und-0-value input').val(val);

				val = elt.find('.field-name-field-grievance-co-state .field-item').html();
				$('.form-item-field-grievance-co-state-und-0-value input').val(val);

				val = elt.find('.field-name-field-grievance-co-zip .field-item').html();
				$('.form-item-field-grievance-co-zip-und-0-value input').val(val);
			};

			$('#grievance-form-sts-link .grievance-recipient').off('click').on('click', function() { grievance_handle_st_click($(this)); });

			function grievance_handle_st_click(elt) {

				val = elt.find('.field-name-field-grievance-co-name .field-item').html();
				$('.field-name-field-grievance-st-name-form input').val(val);

				val = elt.find('.field-name-field-grievance-co-email .field-item').html();
				$('.field-name-field-grievance-st-email-form input').val(val);

				val = elt.find('.field-name-field-grievance-co-phone .field-item').html();
				$('.field-name-field-grievance-st-phone-form input').val(val);
			};

	    $('.grievance_assignee_notes_dropdown').once().change(function () {
	    	$('.field-name-field-grievance-assignee-notes-form textarea').val($(this).val());
	    });

	    $('.grievance_fax_recipient_dropdown').once().change(function () {
	    	tmp = $(this).val();
	    	number = tmp.substring(tmp.indexOf('|')+1);
	    	$('.grievance_fax_recipient').val(number);
	    });

			// Hide and show contacts as needed
			$("#grievance-contact-hide-shop").click(function(event) {
				event.preventDefault();
				$("#grievance-contact-hide-shop").hide();
				$("#grievance-contact-show-shop").show();
				$(".grievance-recipient-shop").hide();
			});
			$("#grievance-contact-show-shop").click(function() {
				event.preventDefault();
				$("#grievance-contact-hide-shop").show();
				$("#grievance-contact-show-shop").hide();
				$(".grievance-recipient-shop").show();
			});
			$("#grievance-contact-hide-company").click(function(event) {
				event.preventDefault();
				$("#grievance-contact-hide-company").hide();
				$("#grievance-contact-show-company").show();
				$(".grievance-recipient-company").hide();
			});
			$("#grievance-contact-show-company").click(function() {
				event.preventDefault();
				$("#grievance-contact-hide-company").show();
				$("#grievance-contact-show-company").hide();
				$(".grievance-recipient-company").show();
			});

			// Set "corrected" when demographic information changes
			/*
			$('.node-grievance-form .field-name-field-grievance-first-name-form input, .node-grievance-form .field-name-field-grievance-last-name-form input, .node-grievance-form .field-name-field-grievance-phone-name-form input, .node-grievance-form .field-name-field-grievance-email-name-form input, .node-grievance-form .field-name-field-grievance-address-name-form input, .node-grievance-form .field-name-field-grievance-address-2-name-form input, .node-grievance-form .field-name-field-grievance-city-name-form input, .node-grievance-form .field-name-field-grievance-state-name-form input, .node-grievance-form .field-name-field-grievance-hire-date-name-form input, .node-grievance-form .field-name-field-grievance-ein-form input, .node-grievance-form .field-name-field-grievance-min-form input, .node-grievance-form .field-name-field-grievance-classification-form input').change(function() {
				if ($('.node-grievance-form .field-name-field-grievance-corrected-form select').length) {
					$('.node-grievance-form .field-name-field-grievance-corrected-form select').val('Pending');
				}
			});
			*/

			/*
			$('.node-grievance-form .field-name-field-grievance-status-date-form').once().hide();
			$('.node-grievance-form #edit-field-grievance-status-und').change(function(event) {
				$('.field-name-field-grievance-status-date-form').show();
			});
*/
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


function sirius_popup(url, name, width, height) {
	remote = window.open(url, name, 'width=' + width + ',height=' + height +',resizable=yes,scrollbars=yes');
	if (remote != null) {
        if (remote.opener == null) {
        	remote.opener = self;
        }
    	remote.location.href = url;
	}
}

