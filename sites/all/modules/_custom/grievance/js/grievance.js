(function($) {
	$(document).ready(function () {
		
		// Given a URL, embed a PDF with that URL.
		function grievance_embed_pdf(the_url){
			var myPDF = new PDFObject({ 
				url: the_url,
				height: "700px",
				id: "grievance-pdf-object",
			}).embed('grievance-pdf-container'); 
			
			// For some reason, ID doesn't seem to take
			$('#grievance-pdf-container object').attr('id', 'grievance-pdf-object');
		}

		// Load PDF on load()
		url = $('#grievance-pdf-url').text();
		if (url.length > 3) {
			grievance_embed_pdf(url);
		}

	});
}(jQuery));
