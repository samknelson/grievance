(function($) {
	// console.log("Entered...");

	$(document).ready(function () {

		// Worker Sync
		Drupal.behaviors.grievance_timss = {
	    attach: function(context, settings) {
			$('.grievance_workersync_id_for_insert').once().click(function(event) {
				event.preventDefault(); 
				grievance_workersync_handle_recipient_click($(this));
			});

			function grievance_workersync_handle_recipient_click(elt) {
				val = elt.html();
				console.log("Looking up: " + val);

				var jqxhr = $.getJSON('/grievance/workersync/lookup/' + val);
				jqxhr.complete(function(data) {
					$('.form-item-field-grievance-first-name-und-0-value input').val(data.responseJSON.first_name);
					$('.form-item-field-grievance-last-name-und-0-value input').val(data.responseJSON.last_name);
					$('.form-item-field-grievance-city-und-0-value input').val(data.responseJSON.city);
					$('.form-item-field-grievance-state-und-0-value input').val(data.responseJSON.state);
					$('.form-item-field-grievance-zip-und-0-value input').val(data.responseJSON.zip);
					$('.form-item-field-grievance-address-und-0-value input').val(data.responseJSON.street);
					$('.form-item-field-grievance-address-2-und-0-value input').val(data.responseJSON.street2);
					$('.form-item-field-grievance-gender-und select').val(data.responseJSON.gender);
					$('.form-item-field-grievance-phone-und-0-value input').val(data.responseJSON.phone);
					$('.form-item-field-grievance-email-und-0-email input').val(data.responseJSON.email);
					$('.form-item-field-grievance-department-und-0-value input').val(data.responseJSON.department);
					$('.form-item-field-grievance-classification-und-0-value input').val(data.responseJSON.classification);
					$('.form-item-field-grievance-min-und-0-value input').val(data.responseJSON.id);
				});

			}
		},
	}


		$.fn.grievance_bind_classification_term_map = function() {
			// console.log("Binding...");

			if (Drupal.jsAC) {
				// console.log("Bound.");
			  Drupal.jsAC.prototype.select = function (node) {
			    this.input.value = $(node).data('autocompleteValue');
			    // Custom: add an event trigger
			    $(this.input).trigger('autocompleteSelect', $(node).data('autocompleteValue'));
			  };
			}

			$('.form-item-field-grievance-class-tid-und input').once().change(function() {
				// console.log("Handler for .change() called.");
			});

			$('.form-item-field-grievance-class-tid-und input').bind('autocompleteSelect', function() {
				// Get our map
				map = Drupal.settings.grievance.grievance_classification_department_map;
				if (!map) { return; }
				// console.log(map);

				// Start by enabling everything.
				$(".form-item-field-grievance-department-tid-und option").each(function() { 
					$(this).attr('disabled', false);
					$(this).attr("hidden", false);
				});

				// Get our classification name
				class_name = $(this).val();
				// console.log("Searching for " + class_name);
				if (!class_name) { return; }
				if (!map[class_name]) { return; }

				// Iterate over all the departments
				$(".form-item-field-grievance-department-tid-und option").each(function() {
					// Get the department TID
					dept_tid = $(this).val();
					if (dept_tid == '_none') { return; }
					if (dept_tid == '') { return; }
					// console.log("Searching for " + class_name + " / " + dept_tid + ": "); 
					// console.log(map[class_name][dept_tid]);

					// If it's not in the map, disable it.
					if (map[class_name][dept_tid]) {
						// console.log("Found it."); 
					} else {
						$(this).attr("disabled", true);
						$(this).attr("hidden", true);
						// console.log('Not found.  Disable.');
					};

				}); // each
			}); // bind
		}; // $.fn

		// console.log("Ready...");
		$.fn.grievance_bind_classification_term_map();

 		window.sirius_clause_pick = function(nid) {
			$('.field-name-field-grievance-clauseref-form .form-item select.form-select').each(function(idx, elt) {
				// Stupid row weight select fields. There doesn't seem to be a simpler way to skip 'em.'
				current_class = $(this).attr('class');
				if (current_class.includes('order')) { return true; }
				current_val = $(this).val();
				if (current_val && current_val != '_none') { return true; }
				$(this).val(nid);
  				return false;
			});
		}

		$.fn.grievance_bind_irset_map = function() {
			if (Drupal.settings && Drupal.settings.grievance_form_irsets) {
				$('.form-item-irsets input').change(function() {
					nid = $(this).val();
					checked = $(this).prop('checked');
					tids = Drupal.settings.grievance_form_irsets[nid];
					if (tids.length) {
						for (i=0; i<tids.length; ++i) {
							tid = tids[i];
							$('.field-name-field-grievance-document-types-form .form-item-field-grievance-document-types-und-' + tid + ' input:checkbox').prop('checked', checked); 
						}
					}
				});
				for (nid in Drupal.settings.grievance_form_irsets) {
				}
			}
		}

		$.fn.grievance_bind_irset_map();

	});
}(jQuery));
