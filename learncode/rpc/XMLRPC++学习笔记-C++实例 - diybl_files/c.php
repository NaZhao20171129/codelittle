function cnzz_hf(str){var b=37851;var a=63689;var h=0;var i=0;
for(i=0;i<str.length;i++){var tp=h+(a*(str.charAt(i).charCodeAt()))>>2;if(tp){h=tp;a+=b;}}
return  h+'';}
function gv_cnzz(of){
	var es = document.cookie.indexOf(";",of);
	if(es==-1) es=document.cookie.length;
	return unescape(document.cookie.substring(of,es));
}
function gc_cnzz(n){
	var arg=n+"=";
	var alen=arg.length;
	var clen=document.cookie.length;
	var i=0;
	while(i<clen){
	var j=i+alen;
	if(document.cookie.substring(i,j)==arg) return gv_cnzz(j);
	i=document.cookie.indexOf(" ",i)+1;
	if(i==0)	break;
	}
	return -1;
}
var cnzz_ed=new Date();
var cnzz_now=parseInt(cnzz_ed.getTime());
var cnzz_data='&r='+escape(document.referrer.substr(0,512))+'&lg='+escape(navigator.systemLanguage)+'&ntime=0.64620800 1329909353';
var cnzz_a=gc_cnzz("cnzz_a30010366");
if(cnzz_a!=-1) cnzz_a=parseInt(cnzz_a)+1;
else cnzz_a=0;
var cnzz_rt=parseInt(gc_cnzz("rtime30010366"));
var cnzz_lt=parseInt(gc_cnzz("ltime30010366"));
var cnzz_st = parseInt((cnzz_now-cnzz_lt)/1000);
var cnzz_sin = gc_cnzz("sin30010366");
if(cnzz_sin==-1) cnzz_sin='none';
if( document.referrer.split('/')[2]!=document.domain ) cnzz_sin=document.referrer;
var h=cnzz_hf(document.location.href);
var cnzz_eid=gc_cnzz("cnzz_eid30010366");
if(cnzz_eid==-1) cnzz_eid=Math.floor(Math.random()*100000000)+"-"+1329909353+"-"+document.referrer.substr(0,64);
if(cnzz_lt<1000000){cnzz_rt=0;cnzz_lt=0;}
if(cnzz_rt<1) cnzz_rt=0;
if(((cnzz_now-cnzz_lt)>500*86400)&&(cnzz_lt>0)) cnzz_rt++;
cnzz_data=cnzz_data+'&repeatip='+cnzz_a+'&rtime='+cnzz_rt+'&cnzz_eid='+escape(cnzz_eid)+'&showp='+escape(screen.width+'x'+screen.height)+'&st='+cnzz_st+'&sin='+escape(cnzz_sin.substr(0,512))+'&res=0';
document.write('<img src="http://hqs1.cnzz.com/stat.htm?id=30010366'+cnzz_data+'" border=0 width=0 height=0>');

var cnzz_et=(86400-cnzz_ed.getHours()*3600-cnzz_ed.getMinutes()*60-cnzz_ed.getSeconds());
cnzz_ed.setTime(cnzz_now+1000*(cnzz_et-cnzz_ed.getTimezoneOffset()*60));
document.cookie="cnzz_a30010366="+cnzz_a+";expires="+cnzz_ed.toGMTString()+ "; path=/";
document.cookie="sin30010366="+escape(cnzz_sin)+ ";expires="+cnzz_ed.toGMTString()+";path=/";
cnzz_ed.setTime(cnzz_now+1000*86400*182);
document.cookie="rtime30010366="+cnzz_rt+";expires="+cnzz_ed.toGMTString()+ ";path=/";
document.cookie="ltime30010366="+cnzz_now+";expires=" + cnzz_ed.toGMTString()+ ";path=/";
document.cookie="cnzz_eid30010366="+escape(cnzz_eid)+ ";expires="+cnzz_ed.toGMTString()+";path=/";
