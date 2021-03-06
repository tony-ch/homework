<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <title>登录页面</title>
    
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
		
		<h3>登录</h3>
		
	</div> <!-- /login-header -->
	
	<div id="login-content" class="clearfix">
	<form name="loginform" action="/bicycle/loginServlet"  method="post">
				<fieldset>
					<div class="control-group">
						<label class="control-label" for="loginname">登录名</label>
						<div class="controls">
							<input type="text" maxlength="20" class="" id="loginname" name="loginname" required/>
						</div>
					</div>
					<div class="control-group">
						<label class="control-label" for="password">密码</label>
						<div class="controls">
							<input type="password" maxlength="20" class="" id="password" name="password" required/>
						</div>
					</div>
				</fieldset>
				
				<!-- <div id="remember-me" class="pull-left">
					<input type="checkbox" name="remember" id="remember" />
					<label id="remember-label" for="remember">记住密码</label>
				</div> -->
				<div id="remember-me" class="pull-left">
					<label><input name="type" type="radio" value="user" checked/>用户 </label> 
					<label><input name="type" type="radio" value="worker" />工人 </label> 
					<label><input name="type" type="radio" value="admin" />管理员 </label> 
				</div>
				
				<div class="pull-right">
					<label></label>
					<button type="button" onclick="location.href='/bicycle/face_login.jsp'" class="btn btn-large">刷脸登录</button>
					<button type="submit" class="btn btn-warning btn-large">
						登录
					</button>
				</div>
	</form>
			
		</div> <!-- /login-content -->
		
		
		<div id="login-extra">
			
			<p>还没有账号? <a href="/bicycle/register.jsp;">马上注册.</a></p>
			<!--<p>已经录入人脸信息？ <a href="/bicycle/face_login.jsp;">刷脸登录.</a></p>-->

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
