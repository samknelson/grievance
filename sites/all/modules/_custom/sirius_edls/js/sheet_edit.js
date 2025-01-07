(function ($) {
	Drupal.behaviors.sirius_edls_sheet_edit = {
		attach: function (context) {
			// On the "sheet edit" form, you can select a department. Each department 
			// may be associated with one or more tasks. So we need to limit the available tasks
			// based on the curently selected department.

			var settings = Drupal.settings.sirius_edls_sheet_edit;
			// console.log(settings);

			//
			// Set the task options based on the department selection
			//

			function set_task_options() {
				department_tid = $('#sirius_edls_sheet_department').val();
				// console.log("Setting task options ...");
				$('.sirius_edls_sheet_task').each(function(idx) {
					// console.log("Item " + idx);
					current_value = $(this).val();
					$(this).val('');
					$(this).html('');
					found = false;
					$(this).append('<option value="">-- Select --</option>');
					if (settings['tasks_by_department'][department_tid]) {
						for (i=0; i<settings['tasks_by_department'][department_tid].length; ++i) {
							task = settings['tasks_by_department'][department_tid][i];
							$(this).append('<option value="' + task + '">' + task + '</option>');
							if (task == current_value) { found = true; }
						}
					}
					if (found) {
						$(this).val(current_value);
					} else {
						$(this).val('');
					}
				});
			}

			// When a new crew is added, set the options.
			$('body').on('sirius_ajax_generic_replace', function() {
				set_task_options();
			});

			// When the department is changed, set the options
			$('#sirius_edls_sheet_department').change(function() {
				set_task_options();
			});

			// When the page is loaded, set the options
			set_task_options();

			//
			// Set the supervisor default for each crew based on the sheet selection
			//

			function set_supervisor_default() {
				supervisor_uid = $('#sirius_edls_sheet_supervisor').val();
				if (!supervisor_uid) { return; }

				$('.sirius_edls_crew_supervisor').each(function(idx) {
					// Don't overwrite anything manually set
					if ($(this).val()) { return; }
					$(this).val(supervisor_uid);
				});
			}

			// When the sheet supervisor is changed, set the default for each crew
			$('#sirius_edls_sheet_supervisor').once().change(function() {
				set_supervisor_default();
			});

			// When a new crew is added, set the options.
			$('body').on('sirius_ajax_generic_replace', function() {
				set_supervisor_default();
			});
		}
	};
})(jQuery);
