<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
    <%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <title>Bicycle </title>
    
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
			
			<a class="brand" href="/bicycle/index.jsp">BlAdmin</a>
			
			<div class="nav-collapse">
			
				<ul class="nav pull-right">
					<li>
						<a href="/bicycle/index.jsp"><span class="badge badge-warning">7</span></a>
					</li>
					
					<li class="divider-vertical"></li>
					
					<li class="dropdown">
						
						<a data-toggle="dropdown" class="dropdown-toggle " href="#">
							罗宾逊 <b class="caret"></b>							
						</a>
						
						<ul class="dropdown-menu">
							<li>
								<a href="/bicycle/account.jsp"><i class="icon-user"></i> 账号设置  </a>
							</li>
							
							<li>
								<a href="/bicycle/account.jsp"><i class="icon-lock"></i> 修改密码</a>
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
						<img src="/bicycle/img/headshot.png" alt="" class="thumbnail" />
					</div> <!-- /account-avatar -->
				
					<div class="account-details">
					
						<span class="account-name">罗宾逊</span>
						
						<span class="account-role">管理员</span>
						
						<span class="account-actions">
							<a href="/bicycle/account.jsp">资料</a> |
							
							<a href="/bicycle/account.jsp">编辑设置</a>
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
					
					<li>
						<a href="/bicycle/faq.jsp">
							<i class="icon-pushpin"></i>
							帮助页面	
						</a>
					</li>
					
					<li>
						<a href="/bicycle/plans.jsp">
							<i class="icon-th-list"></i>
							价目表单		
						</a>
					</li>
					
					<li>
						<a href="/bicycle/grid.jsp">
							<i class="icon-th-large"></i>
							网格布局
							<span class="label label-warning pull-right">5</span>
						</a>
					</li>
					
					<li>
						<a href="/bicycle/pa/listAdmServlet">
							<i class="icon-signal"></i>
							管理系统
						</a>
					</li>
					
					<li>
						<a href="/bicycle/account.jsp">
							<i class="icon-user"></i>
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
					<p>这里是提示信息文字这里是提示信息文字这里是提示信息文字这里是提示信息文字这里是提示信息文字这里是提示信息文字这里是提示信息文字这里是提示信息文字.</p>
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
							<span>564</span>							
							销售订单						
						</div> <!-- /stat -->						
					</div> <!-- /stat-holder -->
					
					<div class="stat-holder">						
						<div class="stat">							
							<span>423</span>							
							增加订单							
						</div> <!-- /stat -->						
					</div> <!-- /stat-holder -->
					
					<div class="stat-holder">						
						<div class="stat">							
							<span>96</span>							
							退货订单							
						</div> <!-- /stat -->						
					</div> <!-- /stat-holder -->
					
					<div class="stat-holder">						
						<div class="stat">							
							<span>2</span>							
							退款数量							
						</div> <!-- /stat -->						
					</div> <!-- /stat-holder -->
					
				</div> <!-- /stat-container -->
				
				<div class="widget">
										
					<div class="widget-header">
						<i class="icon-signal"></i>
						<h3>图表统计</h3>
					</div> <!-- /widget-header -->
														
					<div class="widget-content">					
						<div id="bar-chart" class="chart-holder"></div> <!-- /bar-chart -->				
					</div> <!-- /widget-content -->
					
				</div> <!-- /widget -->
				
				<div class="widget widget-table">
										
					<div class="widget-header">
						<i class="icon-th-list"></i>
						<h3>表格</h3>
					</div> <!-- /widget-header -->
					
					<div class="widget-content">
					
						<table class="table table-striped table-bordered">
							<thead>
								<tr>
									<th></th>
									<th>名</th>
									<th>姓</th>
									<th>用户名</th>
									<th>公司</th>
									<th>操作</th>
								</tr>
							</thead>
							
							<tbody>
								<tr>
									<td>1</td>
									<td>Michael</td>
									<td>Jordan</td>
									<td>@mjordan</td>
									<td>Chicago Bulls</td>
									<td class="action-td">
										<a href="javascript:;" class="btn btn-small btn-warning">
											<i class="icon-ok"></i>								
										</a>					
										<a href="javascript:;" class="btn btn-small">
											<i class="icon-remove"></i>						
										</a>
									</td>
								</tr>
								<tr>
									<td>2</td>
									<td>Magic</td>
									<td>Johnson</td>
									<td>@mjohnson</td>
									<td>Los Angeles Lakers</td>
									<td class="action-td">
										<a href="javascript:;" class="btn btn-small btn-warning">
											<i class="icon-ok"></i>								
										</a>						
										<a href="javascript:;" class="btn btn-small">
											<i class="icon-remove"></i>						
										</a>
									</td>
								</tr>
								<tr>
									<td>3</td>
									<td>Charles</td>
									<td>Barkley</td>
									<td>@cbarkley</td>
									<td>Phoenix Suns</td>
									<td class="action-td">
										<a href="javascript:;" class="btn btn-small btn-warning">
											<i class="icon-ok"></i>								
										</a>						
										<a href="javascript:;" class="btn btn-small">
											<i class="icon-remove"></i>						
										</a>
									</td>
								</tr>
								<tr>
									<td>4</td>
									<td>Karl</td>
									<td>Malone</td>
									<td>@kmalone</td>
									<td>Utah Jazz</td>
									<td class="action-td">
										<a href="javascript:;" class="btn btn-small btn-warning">
											<i class="icon-ok"></i>								
										</a>					
										<a href="javascript:;" class="btn btn-small">
											<i class="icon-remove"></i>						
										</a>
									</td>
								</tr>
								<tr>
									<td>5</td>
									<td>David</td>
									<td>Robinson</td>
									<td>@drobinson</td>
									<td>San Antonio Spurs</td>
									<td class="action-td">
										<a href="javascript:;" class="btn btn-small btn-warning">
											<i class="icon-ok"></i>								
										</a>						
										<a href="javascript:;" class="btn btn-small">
											<i class="icon-remove"></i>						
										</a>
									</td>
								</tr>
								<tr>
									<td>6</td>
									<td>Reggie</td>
									<td>Miller</td>
									<td>@rmiller</td>
									<td>Indiana Pacers</td>
									<td class="action-td">
										<a href="javascript:;" class="btn btn-small btn-warning">
											<i class="icon-ok"></i>								
										</a>						
										<a href="javascript:;" class="btn btn-small">
											<i class="icon-remove"></i>						
										</a>
									</td>
								</tr>
								<tr>
									<td>7</td>
									<td>Clyde</td>
									<td>Drexler</td>
									<td>@cdrexler</td>
									<td>Portland Trail Blazers</td>
									<td class="action-td">
										<a href="javascript:;" class="btn btn-small btn-warning">
											<i class="icon-ok"></i>								
										</a>						
										<a href="javascript:;" class="btn btn-small">
											<i class="icon-remove"></i>						
										</a>
									</td>
								</tr>
								<tr>
									<td>8</td>
									<td>Hakeem</td>
									<td>Olajuwon</td>
									<td>@holajuwon</td>
									<td>Houston Rockets</td>
									<td class="action-td">
										<a href="javascript:;" class="btn btn-small btn-warning">
											<i class="icon-ok"></i>								
										</a>						
										<a href="javascript:;" class="btn btn-small">
											<i class="icon-remove"></i>						
										</a>
									</td>
								</tr>
							</tbody>
						</table>
					
					</div> <!-- /widget-content -->
					
				</div> <!-- /widget -->
				
				
				
				
				<div class="row">
					
					<div class="span5">
									
						<div class="widget">
							
							<div class="widget-header">
								<h3>5列标题</h3>
							</div> <!-- /widget-header -->
																
							<div class="widget-content">
								<p>5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示5列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示.</p>
							</div> <!-- /widget-content -->
							
						</div> <!-- /widget -->
						
					</div> <!-- /span5 -->
		
					<div class="span4">
						
						<div class="widget">
							
							<div class="widget-header">
								<h3>4列标题</h3>
							</div> <!-- /widget-header -->
																
							<div class="widget-content">
								<p>4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示4列布局内容显示.</p>
							</div> <!-- /widget-content -->
							
						</div> <!-- /widget -->
					</div> <!-- /span4 -->
					
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
