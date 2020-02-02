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

  /**
   * @todo - move these into their own modules, it's silly to load them all.
   */

	Drupal.behaviors.grievance_worker = {
	    attach: function(context, settings) {
			$('.grievance-worker-id-for-insert').once().click(function(event) {
				event.preventDefault(); 
				grievance_worker_handle_recipient_click($(this));
			});

			function grievance_worker_handle_recipient_click(elt) {
				val = elt.html();

				var jqxhr = $.getJSON('/grievance/worker/lookup/' + val);
				jqxhr.complete(function(data) {
					console.log(data);
					console.log(data.responseJSON);
					console.log(data.responseJSON.first_name);
					$('.form-item-field-grievance-first-name-und-0-value input').val(data.responseJSON.first_name);
					$('.form-item-field-grievance-last-name-und-0-value input').val(data.responseJSON.last_name);
					$('.form-item-field-grievance-city-und-0-value input').val(data.responseJSON.city);
					$('.form-item-field-grievance-state-und-0-value input').val(data.responseJSON.state);
					$('.form-item-field-grievance-zip-und-0-value input').val(data.responseJSON.zip);
					$('.form-item-field-grievance-address-und-0-value input').val(data.responseJSON.street);
					$('.form-item-field-grievance-gender-und select').val(data.responseJSON.gender);
					$('.form-item-field-grievance-phone-und-0-value input').val(data.responseJSON.phone);
					$('.form-item-field-grievance-email-und-0-email input').val(data.responseJSON.email);
					$('.form-item-field-grievance-min-und-0-value input').val(data.responseJSON.id);
				});

			}
		},
	}

	Drupal.behaviors.grievance_timss = {
	    attach: function(context, settings) {
			$('.grievance-timss-id-for-insert').once().click(function(event) {
				event.preventDefault(); 
				grievance_timss_handle_recipient_click($(this));
			});

			function grievance_timss_handle_recipient_click(elt) {
				val = elt.html();

				broughtby_tid = $('#grievance-timss-broughtby-tid-for-insert').html();
				if (!broughtby_tid) { broughtby_tid = 'DEFAULT'; }

				var jqxhr = $.getJSON('/grievance/timss/lookup/' + val + '?broughtby_tid=' + broughtby_tid);
				jqxhr.complete(function(data) {
					var json = jQuery.parseJSON(data.responseJSON);
					$('.form-item-field-grievance-first-name-und-0-value input').val(json[0].first_Name);
					$('.form-item-field-grievance-last-name-und-0-value input').val(json[0].last_Name);

					$('.form-item-field-grievance-city-und-0-value input').val(json[0].city);
					$('.form-item-field-grievance-state-und-0-value input').val(json[0].state);
					$('.form-item-field-grievance-zip-und-0-value input').val(json[0].zip);
					$('.form-item-field-grievance-address-und-0-value input').val(json[0].street_Address);
					$('.form-item-field-grievance-gender-und select').val(json[0].gender);

					/*
					tmp = json[0].house_Address.street_Address;
					tmp = tmp.replace(/(?:\r\n|\r|\n)/g, ', ');
					$('.form-item-field-grievance-city-und-0-value input').val(json[0].house_Address.city);
					$('.form-item-field-grievance-state-und-0-value input').val(json[0].house_Address.state);
					$('.form-item-field-grievance-address-und-0-value input').val(tmp);
					$('.form-item-field-grievance-zip-und-0-value input').val(json[0].house_Address.zip);
					*/
					$('.form-item-field-grievance-phone-und-0-value input').val(json[0].phone_Number);
					$('.form-item-field-grievance-email-und-0-email input').val(json[0].email_Address);
					$('.form-item-field-grievance-department-und-0-value input').val(json[0].department);
					$('.form-item-field-grievance-classification-und-0-value input').val(json[0].classification);
					$('.form-item-field-grievance-min-und-0-value input').val(json[0].timsS_Member_ID);
				});

			}
		},
	}


	Drupal.behaviors.grievance_imis = {
	    attach: function(context, settings) {
			$('.grievance-imis-id-for-insert').once().click(function(event) {
				event.preventDefault(); 
				grievance_imis_handle_recipient_click($(this));
			});

			function grievance_imis_handle_recipient_click(elt) {
				val = elt.html();

				broughtby_tid = $('#grievance-timss-broughtby-tid-for-insert').html();
				if (!broughtby_tid) { broughtby_tid = 'DEFAULT'; }
				var jqxhr = $.getJSON('/grievance/imis/lookup/' + val + '?broughtby_tid=' + broughtby_tid);
				jqxhr.complete(function(data) {
					var json = jQuery.parseJSON(data.responseJSON);
					console.log(json);
					$('.form-item-field-grievance-first-name-und-0-value input').val(json.FirstName);
					$('.form-item-field-grievance-last-name-und-0-value input').val(json.LastName);

					if (json.WorkerAddress) {
						$('.form-item-field-grievance-city-und-0-value input').val(json.WorkerAddress.City);
						$('.form-item-field-grievance-state-und-0-value input').val(json.WorkerAddress.State);
						$('.form-item-field-grievance-zip-und-0-value input').val(json.WorkerAddress.Zip);
						$('.form-item-field-grievance-address-und-0-value input').val(json.WorkerAddress.Address1);
						$('.form-item-field-grievance-address-2-und-0-value input').val(json.WorkerAddress.Address2);
						$('.form-item-field-grievance-gender-und select').val(json.gender);
					}

					$('.form-item-field-grievance-phone-und-0-value input').val(json.Phones[0].Phone);
					$('.form-item-field-grievance-email-und-0-email input').val(json.Emails[0].Email);
					// $('.form-item-field-grievance-department-und-0-value input').val(json[0].department);
					// $('.form-item-field-grievance-classification-und-0-value input').val(json[0].classification);
					$('.form-item-field-grievance-min-und-0-value input').val(json.ImisId);
				});

			}
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

