<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html>
<html lang="en">
<script type="text/javascript">

function issame(str1,str2)  
{  
if (str1==str2)  
{return(true);}  
else  
{return(false);}  
}

function validate_form(thisform)
{
with (thisform)
  {
	if (issame(password.value,password2.value)==false)
	{password2.focus();alert("password dismatch");return false}
  }
}
</script>

  <head>
    <meta charset="utf-8" />
    <title>注册页面</title>
    
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <meta name="apple-mobile-web-app-capable" content="yes" />    
    
    <link href="/bicycle/css/bootstrap.min.css" rel="stylesheet" />
    <link href="/bicycle/css/bootstrap-responsive.min.css" rel="stylesheet" />
    
    <link href="/bicycle/css/font-awesome.css" rel="stylesheet" />
    
    <link href="/bicycle/css/adminia.css" rel="stylesheet" /> 
    <link href="/bicycle/css/adminia-responsive.css" rel="stylesheet" /> 
    
    <link href="/bicycle/css/pages/login.css" rel="stylesheet" /> 

    <!-- Le HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
      <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->
	
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" /></head>
<c:if test="${!(empty sessionScope.message)}">
	<script type="text/javascript">
		alert('<c:out value="${sessionScope.message}"/>');
	</script>
	<c:remove var="message" scope="session"/>
</c:if>
<body>
	
<div class="navbar navbar-fixed-top">
	
	<div class="navbar-inner">
		
		<div class="container">
			
			<a class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse"> 
				<span class="icon-bar"></span> 
				<span class="icon-bar"></span> 
				<span class="icon-bar"></span> 				
			</a>
			
			<a class="brand" href="/bicycle/index.jsp">Bicycle</a>
			
			<div class="nav-collapse">
			
				<ul class="nav pull-right">
					
					<li class="">
						
						<a href="/bicycle/index.jsp"><i class="icon-chevron-left"></i> 返回首页</a>
					</li>
				</ul>
				
			</div> <!-- /nav-collapse -->
			
		</div> <!-- /container -->
		
	</div> <!-- /navbar-inner -->
	
</div> <!-- /navbar -->


<div id="login-container">
	
	
	<div id="login-header">
		
		<h3>注册</h3>
		
	</div> <!-- /login-header -->
	
	<div id="login-content" class="clearfix">
	
	<form action="/bicycle/regServlet" onsubmit="return validate_form(this)" method="post">
				<fieldset>
					<div class="control-group">
						<label class="control-label"  for="loginname">登录名</label>
						<div class="controls">
							<input type="text" class="" maxlength="18" 
							placeholder="必填,6-18位字母或数字"
							pattern="[0-9A-Za-z]{6,18}"
							id="loginname" name="loginname" required/>
						</div>
					</div>
					<div class="control-group">
						<label class="control-label" for="username">真实名</label>
						<div class="controls">
							<input type="text" class="" maxlength="18" id="username" name="username"/>
						</div>
					</div>
					<div class="control-group">
						<label class="control-label" for="password">密码</label>
						<div class="controls">
							<input type="password" class="" maxlength="18" 
							placeholder="必填,6-18位"
							pattern=".{6,18}"
							id="password" name="password" required/>
						</div>
					</div>
					<div class="control-group">
						<label class="control-label" for="password2">确认密码</label>
						<div class="controls">
							<input type="password" class="" maxlength="18" 
							placeholder="再次输入密码"
							pattern=".{6,18}"
							id="password2" name="password2" required/>
						</div>
					</div>
					<div class="control-group">
						<label class="control-label" for="telnum">手机号</label>
						<div class="controls">
							<input type="text" class="" 
							pattern="[0-9]{11}"
							maxlength=11
							placeholder="请输入11位手机号码"
							id="telnum" name="telnum"/>
						</div>
					</div>
				</fieldset>
				
				<!-- <div id="remember-me" class="pull-left">
					<label id="remember-label" for="remember">提示：</label>
				</div> -->
				
				<div class="pull-right">
					<button type="submit" class="btn btn-warning btn-large">
						注册
					</button>
					<button type="reset" class="btn btn-warning btn-large">
						重置
					</button>
				</div>
	</form>
			
		</div> <!-- /login-content -->
		
		
		<div id="login-extra">
			
			<p>已有账号? <a href="/bicycle/login.jsp;">马上登录.</a></p>
			
			<!--<p>忘记密码? <a href="forgot_password.html">发送邮件.</a></p>  -->
			
		</div> <!-- /login-extra -->
	
</div> <!-- /login-wrapper -->

    

<!-- Le javascript
================================================== -->
<!-- Placed at the end of the document so the pages load faster -->
<script src="/bicycle/js/jquery-1.7.2.min.js"></script>


<script src="/bicycle/js/bootstrap.js"></script>

  </body>
</html>
