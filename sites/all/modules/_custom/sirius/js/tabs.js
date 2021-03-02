(function ($) {
	Drupal.behaviors.sirius_jtab = {
	  attach: function (context) { if (context === document) {
	  	console.log("Here.");
			$('.sirius_jtabs').tabs(); 
	  } }
	};
})(jQuery);
