(function($) {
	// console.log("Entered...");

	$(document).ready(function () {

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

	});
}(jQuery));
