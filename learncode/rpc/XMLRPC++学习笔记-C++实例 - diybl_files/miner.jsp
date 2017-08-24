


	
		var FOJAVALLYMMFLASHID = "flash4TIXA";
	    var FOJAVALLYMMHOME = FOJAVALLYMMFLASHID+'_div';
	    var _FOJAFROG_CONTROL_ = false;
	
	    //var url="http://union.fojafrog.com/foja/rmda_p.jsp?d="+Math.random()+"&pc=0&wid=9&cid=7&userID=&inIframe=0&pageUrl="+encodeURIComponent(document.referrer);
      var url="http://union.mingrunad.com/foja/rmda_p.jsp?d="+Math.random()+"&pc=0&wid=9&cid=7&userID=&inIframe=0&pageUrl="+encodeURIComponent(document.referrer);
	    var s = '<DIV id="'+FOJAVALLYMMHOME+'" style="DISPLAY: block; Z-INDEX: 2147483647; LEFT: 0px;  WIDTH: 354px; BORDER: 0px solid red; position: absolute; TOP: 0px; HEIGHT: 262px"></DIV>';
	    s+= '<script id="tixa_rm_jsp" type="text/javascript" src="'+url+'"><\/script>'
	    
	    if(document.location==top.document.location){
	        document.write(s);
	    }	    


