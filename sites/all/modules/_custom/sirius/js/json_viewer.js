(function ($) {
	Drupal.behaviors.json_viewer = {
	  attach: function (context) { if (context === document) {


	  	if (Drupal.settings.json_viewer.data) {
	  		for (var html_id in Drupal.settings.json_viewer.data) {
	  			$('#' + html_id).jsonViewer(Drupal.settings.json_viewer.data[html_id], Drupal.settings.json_viewer.options);
	  		}
	  	}

	  } }
	};
})(jQuery);
