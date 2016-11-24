<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
    <%@ page import="com.common.CountListener"%>
    <%@ page import="com.common.Counter"%>
    <%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <title>首页 </title>
    
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <meta name="apple-mobile-web-app-capable" content="yes" />    
    
    <link href="/bicycle/css/bootstrap.min.css" rel="stylesheet" />
    <link href="/bicycle/css/bootstrap-responsive.min.css" rel="stylesheet" />
    
    <link href="http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600" rel="stylesheet" />
    <link href="/bicycle/css/font-awesome.css" rel="stylesheet" />
    
    <link href="/bicycle/css/adminia.css" rel="stylesheet" /> 
    <link href="/bicycle/css/adminia-responsive.css" rel="stylesheet" /> 
    
    <link href="/bicycle/css/pages/dashboard.css" rel="stylesheet" /> 
    

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
					
					<li class="divider-vertical"></li>
					
					<li class="dropdown">
						
						<a data-toggle="dropdown" class="dropdown-toggle " href="#">
							<c:if test="${sessionScope.person!=null }">
							<font size=2>${sessionScope.person.loginname }</font><b class="caret"></b>
							</c:if>
							
							<c:if test="${sessionScope.person==null }">
							未登录<b class="caret"></b>
							</c:if>	
						</a>
						
						<ul class="dropdown-menu">
							<li>
								<a href="/bicycle/personDetailServlet"><i class="icon-user"></i> 账号设置  </a>
							</li>
							
							
							<li class="divider"></li>
							
							<li>
								<a href="/bicycle/logoutServlet"><i class="icon-off"></i> 退出</a>
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
						<!-- <img src="/bicycle/img/headshot.png" alt="" class="thumbnail" /> -->
					</div> <!-- /account-avatar -->
				
					<div class="account-details">
					
						<span class="account-name">
							<c:if test="${sessionScope.person.loginname!=null }">
							<font size=4>${sessionScope.person.loginname }</font>
							</c:if>
							
							<c:if test="${sessionScope.person==null }">
							<font size=4>未登录</font>
							</c:if>						
						</span>
						
						<span class="account-role">
						<font size=0>
							<c:if test="${sessionScope.person.loginname==null }">
							${"&nbsp"}
							</c:if>
							
							<c:if test="${sessionScope.person.loginname!=null }">
								<c:if test="${sessionScope.person.type==3}">
								用户
								</c:if>
								
								<c:if test="${sessionScope.person.type==2}">
								维修工
								</c:if>
								
								<c:if test="${sessionScope.person.type==1}">
								采购员
								</c:if>
								
								<c:if test="${sessionScope.person.type==0}">
								管理员
								</c:if>
							</c:if> 
							</font>
							</span>
						
						<span class="account-actions">
							<a href="/bicycle/personDetailServlet">资料</a> 
						</span>
					
					</div> <!-- /account-details -->
				
				</div> <!-- /account-container -->
				
				<hr />
				
				<ul id="main-nav" class="nav nav-tabs nav-stacked">
					
					<li class="active">
						<a href="/bicycle/index.jsp">
							<i class="icon-home"></i>
							首页 		
						</a>
					</li>
					
					<!-- 
					<li>
						<a href="/bicycle/faq.jsp">
							<i class="icon-pushpin"></i>
							帮助页面	
						</a>
					</li>
					 -->
					
					<c:if test="${empty sessionScope.person || sessionScope.person.type==3 }">
					<li>
						<a href="/bicycle/userHomeServlet">
							<i class="icon-user"></i>
							用车系统		
						</a>
					</li>
					</c:if>
					
					<c:if test="${empty sessionScope.person || sessionScope.person.type==1 }">
					<li>
						<a href="/bicycle/purchase.jsp">
							<i class="icon-shopping-cart"></i>
							采购系统		
						</a>
					</li>
					</c:if>
					
					<c:if test="${empty sessionScope.person || sessionScope.person.type==2 }">
					<li>
						<a href="/bicycle/ma/listMaintainServlet">
							<i class="icon-check"></i>
							报修系统
							<!-- <span class="label label-warning pull-right">5</span> -->
						</a>
					</li>
					</c:if>
					
					<c:if test="${empty sessionScope.person || sessionScope.person.type==0 }">
					<li>
						<a href="/bicycle/pa/listAdminServlet">
							<i class="icon-th-list"></i>
							管理系统
						</a>
					</li>
					</c:if>
					
					<li>
						<a href="/bicycle/personDetailServlet">
							<i class="icon-cog"></i>
							用户账号					
						</a>
					</li>
					
					<li>
						<a href="/bicycle/login.jsp">
							<i class="icon-lock"></i>
							登录页面
						</a>
					</li>
					
				</ul>	
				
				<hr />
				
				<div class="sidebar-extra">
					<p>联系我们:</p>
					<p>邮箱: changtao@buaa.edu.cn</p>
					<p>邮箱: xyf1@buaa.edu.cn</p>
				</div> <!-- .sidebar-extra -->
				
				<br />
		
			</div> <!-- /span3 -->
			
			
			
			<div class="span9">
				
				<h1 class="page-title">
					<i class="icon-home"></i>
					首页					
				</h1>
				
				<div class="stat-container">
										
					<div class="stat-holder">						
						<div class="stat">							
							<span><%=Counter.countAdmin()%></span>							
							管理人员数目						
						</div> <!-- /stat -->						
					</div> <!-- /stat-holder -->
					
					<div class="stat-holder">						
						<div class="stat">							
							<span><%=Counter.countWorker()%></span>							
							员工总数							
						</div> <!-- /stat -->						
					</div> <!-- /stat-holder -->
					
					<div class="stat-holder">						
						<div class="stat">							
							<span><%=Counter.countUser()%></span>							
							用户总数							
						</div> <!-- /stat -->						
					</div> <!-- /stat-holder -->
					
					<div class="stat-holder">						
						<div class="stat">							
							<span><%=CountListener.getLinedNumber()%></span>							
							在线人数							
						</div> <!-- /stat -->						
					</div> <!-- /stat-holder -->
					
				</div> <!-- /stat-container -->		
					
			</div> <!-- /span9 -->
			<div class="row">
					
					<div class="span9">
									
						<div class="widget">
							
							<div class="widget-header">
								<h3>公司介绍</h3>
							</div>
							<!-- /widget-header -->

							<div class="widget-content">
								<p>&#12288;&#12288;“方涛任我行”将移动互联网技术引入自行车，让优质的自行车触手可得。无论都市通勤，还是休闲出游，通过“方涛任我行”，你可以轻松享受到品质、舒适、智能的自行车
									。</p>
								<p>&#12288;&#12288;“方涛任我行”的使命是，让用户“随时随地有车骑”。在未来“方涛任我行”希望不生产自行车，只连接自行车，让人们在全世界的每一个角落都可以通过“方涛任我行”解
									锁自行车，满足短途代步的需求。</p>
								<p>&#12288;&#12288;“方涛任我行”同样以开放平台和共享精神，欢迎用户共享自己的单车加入“方涛任我行”，以共享经济的互联网创新模式调动城市单车存量市场，提高自行车使用效率，为城
									市节约更多间。“方涛任我行”倡导文明用车，通过技术手段引导用户规范使用“方涛任我行”共享单车，与市民和政府协同优化共享单车出行解决方案，让城市更美好。</p>
								<p>&#12288;&#12288;“方涛任我行”相信通过移动互联网技术和Bicycle&#32;Sharing的理念，将有效提升城市交通效率，让人们享受到真正愉悦的骑行，实实在在为低碳环保的生态做贡献。Share
									&#32;Green&#32;Fun！</p>

							</div>
							<!-- /widget-content -->
							
						</div> <!-- /widget -->
						
					</div> <!-- /span9 -->
					
				</div> <!-- /row -->
			
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
<script src="/bicycle/js/jquery-1.7.2.min.js"></script>
<script src="/bicycle/js/excanvas.min.js"></script>
<script src="/bicycle/js/jquery.flot.js"></script>
<script src="/bicycle/js/jquery.flot.pie.js"></script>
<script src="/bicycle/js/jquery.flot.orderBars.js"></script>
<script src="/bicycle/js/jquery.flot.resize.js"></script>


<script src="/bicycle/js/bootstrap.js"></script>
<script src="/bicycle/js/charts/bar.js"></script>

  </body>
</html>
