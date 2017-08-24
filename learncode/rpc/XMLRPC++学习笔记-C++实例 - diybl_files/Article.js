        var ajaxUrl = "";
        var ID = 0;
        $(document).ready(function() {
        isLogin();//获得用户登陆数据是否进行显示
        displayData(); //AJAX显示数据
        });
        //AJAX显示数据
        function displayData() {
            ajaxUrl = "/Ajax/Comment.ashx";  //$("#ajaxUrl").val();
            ID = $("#articleID").val();
            $.get(ajaxUrl,
            { action: "GetAll", id: ID },
            function(data) {
                if (data == "") {
                    jQuery("#divComment").empty().append("暂无留言！");
                }
                else {
                    jQuery("#divComment").empty().append(data);
                }
            });
            $.get(ajaxUrl, { action: "ArticleTimes", id: ID },//这里显示评论
            function(data) {
                jQuery("#articleTimes").empty().append(data);
            });
        }
        //提交留言
        function submit_click() {
           
            var codeImg = jQuery("#tbCode").val();
            var name = escape(jQuery("#tbName").val());
            var content = escape(jQuery("#tbContent").val());
            if (codeImg == "") {
                alert("请输入验证码！");
                return;
            }
            if (name == "") {
                alert("请输入昵称！");
                return;
            }
            if (content == "") {
                alert("请输入内容！");
                return;
            }
            var codeImg = jQuery("#tbCode").val();
            if (codeImg.toLowerCase() != (getCookie("CheckCode")).toLowerCase()) {
                alert("验证码错误！");
                return;
            }
            //alert(name);
            $("#bt_submit").attr("disabled", "disabled"); ///禁用

            //----------------------------------------------------
            
            $.get("/Ajax/Comment.ashx",
            { action: "Add", id: ID, name: name, content: content },
            function(data) {
            //alert("获取数据！");
            $("#art_comm").empty().append(data);
            $("#bt_submit").attr("disabled", false);//取消禁用
                reset_click();
                $("img", "dl").click();
                alert("留言成功！");
                displayData(); //AJAX显示数据
            });
        }
        function reset_click() {
            $("#tbCode").val("");
            $("#tbName").val("");
            $("#tbContent").val("");
        }
        function getCookie(name)//取cookies函数        
        {
            var arr = document.cookie.match(new RegExp("(^| )" + name + "=([^;]*)(;|$)"));
            if (arr != null) return unescape(arr[2]); return null;
        }
        function replay(id) {
            $("#tbContent").val("回复：第" + id + "楼|");
        }
        function ReportError() {
            $.get(ajaxUrl, { action: "ReportError", id: ID },
            function(data) {
                alert("报错成功！");
            });
        }


        $(document).ready(function() {
            //验证码
            $("#txtValNum").focus(function() {
                ValNum();

            });
            
            //登陆
            $("#btnChen").click(function() {

                userLogin(); //这里进行登陆模块

            });
            //提交留言
            $("#btnCommit").click(function() {
                //alert("chenqiao");
                submit_click(); //提交留言
            });
            //=================================动态添加验证码================
            $("#tbCode").click(function() {

                //if ($("#spanCode").val() == "") {
                    // $("#spanCode").html("<img id=\"img1\" src=\"/CheckCode.aspx?\" alt=\"看不清？点击更换\" onclick=\"this.src=this.src+'?'\" style=\"cursor: pointer\" />");
                    var imgVar = document.getElementById("Img2");
                    imgVar.style.display = "block";
                    if (imgVar.src.toString() == "") {
                       
                        imgVar.src = "/CheckCode.aspx?";
                    }
                    $("#spanCode").html(imgVar);
              //  }
            });
        });
        //这里获得验证码
        function ValNum() {
      
        var imgVar = document.getElementById("valNumImg");
        //var imgvar2 = $("#valNumImg");
        var inputNum = $("#txtValNum");
        var offset = inputNum.position();
        var left = offset.left- $(inputNum).width();
       // var down = offset.top - $(inputNum).height();

       
        imgVar.style.display = "block";
        imgVar.style.position = "absolute";
        imgVar.style.top = 66 + "px";
        imgVar.style.left = (left)+"px";
        if (imgVar.src.toString() == "") {
            imgVar.src = "http://user.diybl.com/CheckCode.aspx";
        }
 
          //  $("#spanLoginCode").empty().append("<img src='http://user.diybl.com/CheckCode.aspx' onclick=\"this.src=this.src+'?'\" style='postion:absolute;'/>");
        }
        //这里进行登陆模块
        function userLogin() {

            var code = $("#txtValNum").val();
            var userName = $("#txtUserName").val();
            var userPwd = $("#txtUserPwd").val();
            if (code == "" || userName == "" || userPwd == "") {
                alert("登陆信息不能有为空的！");
                return;
            }
            else if (code.toLowerCase() != (getCookie("CheckCode")).toLowerCase()) {
                alert("验证码错误！");
                return;
            }
            else {
               
                //===============================================================
                $.get("http://user.diybl.com/webSiteLogin.ashx",
                                    { userName: $("#txtUserName").val(), userPwd: $("#txtUserPwd").val(), valNum: $("#txtValNum").val(), action: "login" }, function(data) {
                                       // alert(data);
                                        
                                        window.location.reload(); //不返回数据，刷新数据
                                    });
             //==================================================================
            }
        }
        //判断是否登陆
       
        //AJAX显示数据
        function isLogin() {
            $.get("/Ajax/LoginHandler.ashx",
            { action: "displayInfo"},
            function(data) {
//                alert(data);
                if (data != "") {
                    $("#divLoginHolder").empty().append(data);
                }
            });
        }
        
function ChgTab(id)
{
    for (var i = 1; i <= 16; i++) {
	    if(i == id){
    	document.getElementById("index" + i).style.display  = "block";
		}else{
		document.getElementById("index" + i).style.display = "none";
		}
    }
	
}

//转化成十六进制, 调用的是vbscript的Hex函数
function Hex(n)
{   
  c = n;
  execScript("c = Hex(c)", "vbscript");
  return c;
}
//返回文字的AscaII编码, 调用的是vbscript的Asc函数
function Asc(s)
{
  c = s;
  execScript("c = Asc(c)", "vbscript");
  return c;
}
//获取文字的gb2312编码
function gb2312Encode(str)
{
  var string = "";
  c = s = "";
  var high = "";
  var low = "";
  for(var i = 0; i < str.length; i++)
  {
  c = Asc(str.charAt(i));
  if(Math.abs(c) < 0xFF)
     string += str.charAt(i);
  else
  {
     if(c < 0) c += 0x10000;
     high = ((c & 0xFF00) >> 8) & 0x00FF;
     low = c & 0xFF;
     string += "%" + Hex(high) + "%" + Hex(low);
  }
  }
  return string;
}


function search(v){
  self.location="http://so.diybl.com/s?w="+gb2312Encode(v);
}

function ChgSoTabb(strDivIdBlock,strDivStyleBlock,strDivStyleNone,displayNumber,allNumber)
{
    for (var i = 1; i <= allNumber; i++) {
    	document.getElementById(strDivIdBlock + i).className = strDivStyleNone;
		//document.getElementById(strListIdBlock + i).style.display = "none";
    }
	document.getElementById(strDivIdBlock + displayNumber).className = strDivStyleBlock;
	//document.getElementById(strListIdBlock + displayNumber).style.display = "block";
	document.fso.action="http://so.diybl.com/"+getActionUrl(displayNumber);
}
function getActionUrl(id)
{
    switch(id){
      case 1:
        return "s";
      case 2:
        return "d";
      case 3:
        return "e";
      case 4:
        return "b";
    }
}