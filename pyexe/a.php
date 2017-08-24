<?php
function Post($curlPost,$url){
		$curl = curl_init();
		curl_setopt($curl, CURLOPT_URL, $url);
		curl_setopt($curl, CURLOPT_HEADER, false);
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($curl, CURLOPT_NOBODY, true);
		curl_setopt($curl, CURLOPT_POST, true);
		curl_setopt($curl, CURLOPT_POSTFIELDS, $curlPost);
		$return_str = curl_exec($curl);
		curl_close($curl);
		return $return_str;
}

$target = "http://106.ihuyi.com/webservice/sms.php?method=Submit";
//替换成自己的测试账号,参数顺序和wenservice对应
$post_data = "account=用户名&password=密码&mobile=手机号码&content=".rawurlencode("您的验证码是：4852。请不要把验证码泄露给其他人。");
//$binarydata = pack("A", $post_data);
echo $gets = Post($post_data, $target);
//请自己解析$gets字符串并实现自己的逻辑
?>

