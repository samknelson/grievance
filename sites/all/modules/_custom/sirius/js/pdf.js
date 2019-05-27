function first_page_only() {
	var page_params = {};
    var x = document.location.search.substring(1).split('&');
    for (var i in x) { 
    	var z = x[i].split('=',2); 
    	page_params[z[0]] = unescape(z[1]);
    }
    
    var page = page_params.page || 1;

    if(page != 1) {
		document.getElementById('first-page-only').style.display = 'none';
    }
}
