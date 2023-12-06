(function ($) {
	Drupal.behaviors.sirius_edls_sheet_edit = {
		attach: function (context) {
			/*
			var settings = Drupal.settings.sirius_edls_sheet_edit;

			console.log(settings);

			$('#sirius_edls_sheet_department').change(function() {
				department_tid = $('#sirius_edls_sheet_department').val();

				elt = $('.sirius_edls_sheet_task');
				elt.val('');
				elt.html('');
				elt.append('<option value="">-- Select --</option>');
				if (settings['tasks_by_department'][department_tid]) {
					for (i=0; i<settings['tasks_by_department'][department_tid].length; ++i) {
						task = settings['tasks_by_department'][department_tid][i];
						elt.append('<option value="' + task + '">' + task + '</option>');
					}
				}
			});
			*/

			// None of this works.
		}
	};
})(jQuery);
