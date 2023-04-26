(function ($) {
	Drupal.behaviors.sirius_table = {
	  attach: function (context) {

	    // Align tables with the specified class so that all columns line up across tables.
	    // Find the max width of each column across all tables. Only look at 'th' or 'td' cells in the first row of each table.
	    var col_widths = [];
	    $(".sirius_table_align").each(function(index, element) {
        $(element).find("tr:first th, tr:first td").each(function(index2, element2) {
          col_widths[index2] = Math.max(col_widths[index2] || 0, $(element2).width());
        });
	    });

	    // Set each column in each table to the max width of that column across all tables.
	    $(".sirius_table_align").each(function(index, element) {
        $(element).find("tr:first th, tr:first td").each(function(index2, element2) {
          $(element2).width(col_widths[index2]);
        });
	    });
	  }
	};
})(jQuery);
