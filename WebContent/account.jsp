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
	if (issame(password.value,password1.value)==false)
	{password1.focus();alert("password dismatch");return false}
  }
}
</script>
  <head>
 <c:if test="${!(empty sessionScope.message)}">
	<script type="text/javascript">
		alert('<c:out value="${sessionScope.message}"/>');
	</script>
	<c:remove var="message" scope="session"/>
</c:if>
    <meta charset="utf-8" />
    <title>用户中心</title>
    
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <meta name="apple-mobile-web-app-capable" content="yes" />    
    
    <link href="./css/bootstrap.min.css" rel="stylesheet" />
    <link href="./css/bootstrap-responsive.min.css" rel="stylesheet" />
    
    <link href="http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600" rel="stylesheet" />
    <link href="./css/font-awesome.css" rel="stylesheet" />
    
    <link href="./css/adminia.css" rel="stylesheet" /> 
    <link href="./css/adminia-responsive.css" rel="stylesheet" /> 
    
    
    <link href="./css/pages/plans.css" rel="stylesheet" /> 

    <!-- Le HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
      <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->
	
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" /></head>

<body>
	
<div class="navbar navbar-fixed-top">
	
	<div class="navbar-inner">
		
		<div class="container">
			
			<a class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse"> 
				<span class="icon-bar"></span> 
				<span class="icon-bar"></span> 
				<span class="icon-bar"></span> 				
			</a>
			
			<a class="brand" href="./index.jsp">BlAdmin</a>
			
			<div class="nav-collapse">
			
				<ul class="nav pull-right">
					<li>
						<a href="./index.jsp"><span class="badge badge-warning">7</span></a>
					</li>
					
					<li class="divider-vertical"></li>
					
					<li class="dropdown">
						
						<a data-toggle="dropdown" class="dropdown-toggle " href="#">
							罗宾逊 <b class="caret"></b>							
						</a>
						
						<ul class="dropdown-menu">
							<li>
								<a href="./account.jsp"><i class="icon-user"></i> 账号设置  </a>
							</li>
							
							<li>
								<a href="./account.jsp"><i class="icon-lock"></i> 修改密码</a>
							</li>
							
							<li class="./divider"></li>
							
							<li>
								<a href="./logoutServlet"><i class="icon-off"></i> 退出</a>
							</li>
						</ul>
					</li>
				</ul>
				
			</div> <!-- /nav-collapse -->
			
		</div> <!-- /container -->
		
	</div> <!-- /navbar-inner -->
	
</div> <!-- /navbar -->




<div id="content">
	
	<div class="container">
		
		<div class="row">
			
			<div class="span3">
				
				<div class="account-container">
				
					<div class="account-avatar">
						<img src="./img/headshot.png" alt="" class="thumbnail" />
					</div> <!-- /account-avatar -->
				
					<div class="account-details">
					
						<span class="account-name">罗宾逊</span>
						
						<span class="account-role">管理员</span>
						
						<span class="account-actions">
							<a href="./account.jsp">资料</a> |
							
							<a href="./account.jsp">编辑设置</a>
						</span>
					
					</div> <!-- /account-details -->
				
				</div> <!-- /account-container -->
				
				<hr />
				
				<ul id="main-nav" class="nav nav-tabs nav-stacked">
					
					<li>
						<a href="./index.jsp">
							<i class="icon-home"></i>
							首页 		
						</a>
					</li>
					
					<li>
						<a href="./faq.jsp">
							<i class="icon-pushpin"></i>
							帮助页面	
						</a>
					</li>
					
					<li>
						<a href="./plans.jsp">
							<i class="icon-th-list"></i>
							价目表单		
						</a>
					</li>
					
					<li>
						<a href="./grid.jsp">
							<i class="icon-th-large"></i>
							网格布局
							<span class="label label-warning pull-right">5</span>
						</a>
					</li>
					
					<li>
						<a href="./listAdmServlet">
							<i class="icon-signal"></i>
							管理系统
						</a>
					</li>
					
					<li class="active">
						<a href="./account.jsp">
							<i class="icon-user"></i>
							用户账号					
						</a>
					</li>
					
					<li>
						<a href="./login.jsp">
							<i class="icon-lock"></i>
							登录页面
						</a>
					</li>
					
				</ul>	
				
				
				
				<hr />
				
				<div class="sidebar-extra">
					<p>这里是提示信息文字这里是提示信息文字这里是提示信息文字这里是提示信息文字这里是提示信息文字这里是提示信息文字这里是提示信息文字这里是提示信息文字.</p>
				</div> <!-- .sidebar-extra -->
				
				<br />
		
			</div> <!-- /span3 -->
			
			
			
			<div class="span9">
				
				<h1 class="page-title">
					<i class="icon-th-large"></i>
					用户设置			
				</h1>
				
				
				<div class="row">
					
					<div class="span9">
				
						<div class="widget">
							
							<div class="widget-header">
								<h3>基本信息</h3>
							</div> <!-- /widget-header -->
									
							<div class="widget-content">
								
								
								
								<form id="edit-profile" onsubmit="return validate_form(this)" action="./updateServlet"  class="form-horizontal" method="post" >
									<fieldset>
										
										<div class="control-group">											
											<label class="control-label" for="username">用户名：</label>
											<div class="controls">
												<input type="text" class="input-medium disabled" 
												name="loginname"
												id="loginname" value="${sessionScope.person.loginname }" readonly />
												<p class="help-block">登录名是为登录而用，不能修改.</p>
											</div> <!-- /controls -->				
										</div> <!-- /control-group -->
										
										
										<div class="control-group">											
											<label class="control-label" for="name">姓名：</label>
											<div class="controls">
												<input type="text" class="input-medium" 
												maxlength=18
												name="name"
												id="name" 
												value="${sessionScope.person.name }" />
											</div> <!-- /controls -->				
										</div> <!-- /control-group -->
										
										
										<c:if test="${sessionScope.person.type }!=2 && ${sessionScope.person.type }!=3">
										<div class="control-group">											
											<label class="control-label" for="tel">手机号码：</label>
											<div class="controls">
												<input type="text" class="input-large" 
												id="tel" 
												name="tel"
												pattern="[0-9]{11}"
												maxlength=11
												value="${sessionScope.person.tel }" />
												<p class="help-block">输入11位手机号码</p>
											</div> <!-- /controls -->				
										</div> <!-- /control-group -->
										
										</c:if>
										
										
										<br /><br />
										
										<div class="control-group">											
											<label class="control-label" for="password">密码：</label>
											<div class="controls">
												<input type="password" 
												pattern=".{6,18}"
												name="password"
												maxlength=18
												class="input-medium" id="password" 
												 required/>
												 <p class="help-block">输入6-18位密码</p>
											</div> <!-- /controls -->				
										</div> <!-- /control-group -->
										
										
										<div class="control-group">											
											<label class="control-label" for="password1">确认密码：</label>
											<div class="controls">
												<input type="password" 
												name="password1"
												pattern=".{6,18}"
												maxlength=18
												class="input-medium" 
												id="password1"  required/>
											</div> <!-- /controls -->				
										</div> <!-- /control-group -->
										
										
											
											<br />
										
											
										<div class="form-actions">
											<button type="submit" class="btn btn-primary">保存</button> 
											<button type="reset" class="btn">重置</button>
										</div> <!-- /form-actions -->
									</fieldset>
								</form>
								
								
								
								
								
								
								
								
							</div> <!-- /widget-content -->
							
						</div> <!-- /widget -->
						
					</div> <!-- /span9 -->
					
				</div> <!-- /row -->
				
				
				
				
				
				
				
				
				
			</div> <!-- /span9 -->
			
			
		</div> <!-- /row -->
		
	</div> <!-- /container -->
	
</div> <!-- /content -->
					
	
<div id="footer">
	
	<div class="container">				
		<hr />
		<p>&copy; 2016 Bicycle.</p>
	</div> <!-- /container -->
	
</div> <!-- /footer -->


    

<!-- Le javascript
================================================== -->
<!-- Placed at the end of the document so the pages load faster -->
<script src="./js/jquery-1.7.2.min.js"></script>


<script src="./js/bootstrap.js"></script>

  </body>
</html>
