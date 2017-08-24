document.writeln("<script src=\'http:\/\/w.cnzz.com\/c.php?id=30010366\' language=\'JavaScript\' charset=\'gb2312\'><\/script>");
document.body.oncopy = function () { 
	setTimeout( function () { 
		var text = clipboardData.getData("text");
		if (text) { 
			text = text + "\r\nÎÄÕÂ³ö´¦£º·ÉÅµÍø(www.diybl.com):"+location.href;
			clipboardData.setData("text", text);
		} 
				}, 100 ) 
}
