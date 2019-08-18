(function($) { $(document).ready(function () {
	$("input.sirius-worker-filter").keyup(function () {
		var filter = $(this).val().toUpperCase();
		$(".sirius-worker-filter-target .form-type-checkbox").each(function(index) {
			// No filter means show everything
  			if (filter.length < 1) {
  				$(this).show();
  				return;
  			}

  			// Keep anything checked
  			if ($('input', this).is(':checked')) {
  				$(this).show();
  				return;
  			} 

  			// Keep anything where the text matches the input
  			txt = $(this).text().toUpperCase();
  			if (txt.includes(filter)) {
  				$(this).show();
  				return; 
  			}

  			$(this).hide();
		})
	});
}); }(jQuery));

