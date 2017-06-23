(function($) {
	$(document).ready(function () {

		if (Drupal.jsAC) {
		  Drupal.jsAC.prototype.select = function (node) {
		    this.input.value = $(node).data('autocompleteValue');
		    // Custom: add an event trigger
		    $(this.input).trigger('autocompleteSelect', $(node).data('autocompleteValue'));
		  };
		}


		// alert('Hello, world.');
		// console.log(Drupal.settings.grievance.grievance_classification_department_map);

		/*
		$('.form-item-field-grievance-class-tid-und input').once().change(function() {
		  alert( "Handler for .change() called." );
		});

		$('.form-item-field-grievance-class-tid-und input').bind('autocompleteSelect', function() {
			alert("Autocomplete select with " + $(this).val());
		});
		*/




	});
}(jQuery));
