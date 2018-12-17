<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"
isErrorPage="true"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<html>
  <head>
    <meta charset="utf-8" />
    <title>异常页面</title>
    
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
 <style>
    .wrapper{padding-top: 0px;color:#2e2e2e;}
    .wrapper h1{color:#2e2e2e;font-size:24px;text-align: center;font-weight:100;}
    .wrapper h2{font-size:16px;margin: 30px 0 20px;font-weight:100;}
    .wrapper p{color:#2e2e2e;}
</style>
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
<br>
<br>
<div id="content">
	
	<div class="container">
		
		<div class="row" >
			
					<div class="span12">
									
						<div class="widget">
							
							<div class="widget-header" align="center">
								<h3>出现错误</h3>
							</div>
							<!-- /widget-header -->

							<div class="widget-content">
								${requestScope.exception}
								<%
								StackTraceElement[] sElements=((Exception)request.getAttribute("exception")).getStackTrace();
								for(StackTraceElement aElement : sElements){
									out.print(aElement);%>
									<br>
								<%
								}
								 %>
							</div>
							<!-- /widget-content -->
							
						</div> <!-- /widget -->
						
					</div> <!-- /span9 -->
					
		</div> <!-- /row -->
		
	</div> <!-- /container -->
	
</div> <!-- /content -->

  </body>
</html>
