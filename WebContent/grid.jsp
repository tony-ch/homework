<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
    <%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <title>网格布局页面_Bootstrap响应式布局后台模板BlAdmin - JS代码网</title>
    
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <meta name="apple-mobile-web-app-capable" content="yes" />    
    
    <link href="/bicycle/css/bootstrap.min.css" rel="stylesheet" />
    <link href="/bicycle/css/bootstrap-responsive.min.css" rel="stylesheet" />
    
    <link href="http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600" rel="stylesheet" />
    <link href="/bicycle/css/font-awesome.css" rel="stylesheet" />
    
    <link href="/bicycle/css/adminia.css" rel="stylesheet" /> 
    <link href="/bicycle/css/adminia-responsive.css" rel="stylesheet" /> 
    
    
    <link href="/bicycle/css/pages/plans.css" rel="stylesheet" /> 

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
			
			<a class="brand" href="/bicycle/inex.jsp">BlAdmin</a>
			
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
					
					<li >
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
					
					<li class="active">
						<a href="/bicycle/grid.jsp">
							<i class="icon-th-large"></i>
							网格布局
							<span class="label label-warning pull-right">5</span>
						</a>
					</li>
					
					<li >
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
					<i class="icon-th-large"></i>
					网格布局				
				</h1>
				
				
				<div class="row">
					
					<div class="span9">
				
						<div class="widget">
									
							<div class="widget-content">
								
								<h3>9列布局</h3>
								
								<p>关系型数据库服务（Relational Database Service，简称RDS）是一种稳定可靠、可弹性伸缩的在线数据库服务。RDS采用即开即用方式，兼容MySQL、SQL Server两种关系型数据库，并提供数据库在线扩容、备份回滚、性能监测及分析功能。RDS与云服务器搭配使用I/O性能倍增，内网互通避免网络瓶颈。.</p>
								
								<p>开放存储服务（OpenStorageService，简称OSS），是阿里云对外提供的海量，安全，低成本，高可靠的云存储服务。用户可以通过简单的API（REST方式的接口），在任何时间、任何地点、任何互联网设备上进行数据上传和下载。.</p>
							</div> <!-- /widget-content -->
							
						</div> <!-- /widget -->
						
					</div> <!-- /span9 -->
					
				</div> <!-- /row -->
				
				
				
				
				<div class="row">
					
					<div class="span5">
				
						<div class="widget">
								
							<div class="widget-content">
								
								<h3>5列布局</h3>
								
								<p>开放数据处理服务（Open Data Processing Service，简称ODPS）提供云端数据仓库服务。适用于金融、零售、制造业和电商企业的BI团队进行海量数据分析和挖掘。.</p>
								
								<p>开放结构化数据服务（Open Table Service，简称OTS）是一种支持海量结构化和半结构化数据存储与实时查询的服务。.</p>
							</div> <!-- /widget-content -->
							
						</div> <!-- /widget -->
						
					</div> <!-- /span5 -->
					
					
					
					<div class="span4">
				
						<div class="widget">
																
							<div class="widget-content">
								
								<h3>4列布局</h3>
								
								<p>开放缓存服务（Open Cache Service，简称OCS）为在线缓存服务，实现热点数据的快速响应及数据的持久化保存；支持Key-Value的数据结构，兼容Memcached协议。.</p>
								
								<p>云监控高效全面的监控云服务器和站点，帮助用户时刻掌握云服务运行状态。</p>
							</div> <!-- /widget-content -->
							
						</div> <!-- /widget -->
						
					</div> <!-- /span4 -->
					
				</div> <!-- /row -->
					
				
				
				
				<div class="row">
					
					<div class="span3">
				
						<div class="widget">
																							
							<div class="widget-content">
								
								<h3>3列布局</h3>
								
								<p>云盾为客户提供基于云端的DDoS防御、入侵防御及网站的应用安全监测等全方位的安全防御服务。</p>
								
								<p>云监控高效全面的监控云服务器和站点，帮助用户时刻掌握云服务运行状态。.</p>
							</div> <!-- /widget-content -->
							
						</div> <!-- /widget -->
						
					</div> <!-- /span3 -->
					
					
					<div class="span3">
				
						<div class="widget">
							
																
							<div class="widget-content">
								
								<h3>3列布局</h3>
								
								<p>云盾为客户提供基于云端的DDoS防御、入侵防御及网站的应用安全监测等全方位的安全防御服务。</p>
								
								<p>云监控高效全面的监控云服务器和站点，帮助用户时刻掌握云服务运行状态.</p>
							</div> <!-- /widget-content -->
							
						</div> <!-- /widget -->
						
					</div> <!-- /span3 -->
					
					
					<div class="span3">
				
						<div class="widget">
																							
							<div class="widget-content">
								
								<h3>3列布局</h3>
								
								<p>云盾为客户提供基于云端的DDoS防御、入侵防御及网站的应用安全监测等全方位的安全防御服务.</p>
								
								<p>云监控高效全面的监控云服务器和站点，帮助用户时刻掌握云服务运行状态.</p>
							</div> <!-- /widget-content -->
							
						</div> <!-- /widget -->
						
					</div> <!-- /span3 -->
					
				</div> <!-- /row -->
				
				
				
				
				<div class="row">
					
					<div class="span6">
				
						<div class="widget">
																
							<div class="widget-content">
								
								<h3>6列布局</h3>
								
								<p>云服务器（Elastic Compute Service, 简称ECS）是一种处理能力可弹性伸缩的计算服务，其管理方式比物理服务器更简单高效。云服务器帮助您快速构建更稳定、安全的应用，降低开发运维的难度和整体IT成本，使您能够更专注于核心业务创新。</p>
								
								<p>内容分发网络（Content Delivery Network，简称CDN）将加速内容分发至离用户最近的节点，缩短用户查看对象的延迟，提高用户访问网站的响应速度与网站的可用性。</p>
							</div> <!-- /widget-content -->
							
						</div> <!-- /widget -->
						
					</div> <!-- /span5 -->
					
					
					
					<div class="span3">
				
						<div class="widget">
																
							<div class="widget-content">
								
								<h3>3列布局</h3>
								
								<p>开放结构化数据服务（Open Table Service，简称OTS）是一种支持海量结构化和半结构化数据存储与实时查询的服务。</p>
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


<script src="/bicycle/js/bootstrap.js"></script>

  </body>
</html>
