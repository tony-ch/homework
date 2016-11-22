<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
    <%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <title>统计图表页面_Bootstrap响应式布局后台模板BlAdmin - JS代码网</title>
    
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <meta name="apple-mobile-web-app-capable" content="yes" />    
    
    <link href="/bicycle/css/bootstrap.min.css" rel="stylesheet" />
    <link href="/bicycle/css/bootstrap-responsive.min.css" rel="stylesheet" />
    
    <link href="http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600" rel="stylesheet" />
    <link href="/bicycle/css/font-awesome.css" rel="stylesheet" />
    
    <link href="/bicycle/css/adminia.css" rel="stylesheet" /> 
    <link href="/bicycle/css/adminia-responsive.css" rel="stylesheet" /> 
    
    
    <link href="/bicycle/css/jquery.visualize.css" rel="stylesheet" /> 

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
					
					<li>
						<a href="/bicycle/grid.jsp">
							<i class="icon-th-large"></i>
							网格布局
							<span class="label label-warning pull-right">5</span>
						</a>
					</li>
					
					<li class="active">
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
					<i class="icon-signal"></i>
					管理系统					
				</h1>
				
				
				
				
				<div class="widget">
					
					<div class="widget-header">
						<h3>信息</h3>
					</div> <!-- /widget-header -->
														
					<div class="widget-content">
						
						<div class="tabbable">
						<ul class="nav nav-tabs">
						  <li <c:if test="${activeTab=='adtab'}">class="active"</c:if>>
						    <a href="/bicycle/pa/listAdmServlet">管理员</a><!-- adtab  -->
						  </li>
						  <li <c:if test="${activeTab=='wktab'}">class="active"</c:if>>
						  	<a href="#wktab" data-toggle="tab">员工</a><!-- wktab -->
						  </li>
						  <li <c:if test="${activeTab=='ustab'}">class="active"</c:if>>
						  	<a href="#ustab" data-toggle="tab">用户</a><!-- ustab -->
						  </li>
						</ul>
						
							<div class="tab-content">
								<div class="tab-pane <c:if test="${activeTab=='adtab'}">active</c:if>"   id="adtab">
										
										<table class="table table-striped table-bordered">
											<thead>
											
											<tr>
												<th>id</th>
												<th>登录名</th>
												<th>姓名</th>
												<th>密码</th>
												<th>手机号码</th>
												<th>操作</th>
											</tr>
											</thead>
											
											<tbody>		
											<c:forEach items="${requestScope.list}" var="admin">						
											<tr>
												<td>
													${admin.id }
												</td>
												<td>
													<a href='<c:url value="/pa/showPersonServlet?type=admin&loginname=${admin.loginname }"></c:url>'>${admin.loginname }</a>
												</td>
												<td>
													${admin.name }
												</td>
												<td>
													${admin.password }
												</td>
												<td>
													${admin.tel }
												</td>
												<td>
													<a href='<c:url value="/pa/deleteAdminServlet?loginname=${admin.loginname }"></c:url>'><font color=red>删除</font></a> | 
													<a href='<c:url value="/pa/showPersonServlet?type=admin&loginname=${admin.loginname }"></c:url>'><font color=red>查看/修改</font></a>
												</td>
											</tr>
											</c:forEach>							
											
											</tbody>						
											<tbody>	
											<tr>
												<td align="right" colspan="9">
												<a href='<c:url value="/pa/addPerson.jsp?type=admin"></c:url>'><font color=red>添加</font></a>
												</td>
											</tr>
											<tr>
												
												<td colspan="9">
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=1}">
													<a href='<c:url value="/pa/checkAdminServlet?curPage=1&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>首页</a>
													<a href='<c:url value="/pa/checkAdminServlet?curPage=${requestScope.curPage-1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>前一页</a>
												</c:if>
												<c:if test="${requestScope.pageCount==0||requestScope.curPage==1}">首页 前一页</c:if>
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=requestScope.pageCount}">
													<a href='<c:url value="/pa/checkAdminServlet?curPage=${requestScope.curPage+1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>下一页</a>
													<a href='<c:url value="/pa/checkAdminServlet?curPage=${requestScope.pageCount}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>尾页</a>
												</c:if>
												<c:if test="${requestScope.pageCount==0||requestScope.curPage==requestScope.pageCount}">下一页 尾页</c:if>
												&nbsp;
												第${requestScope.curPage}页/共${requestScope.pageCount}页
												&nbsp;
								               </td>
							
								               
											</tr>
											</tbody>	
											
									  </table>
						
								</div>
								
								<div class="tab-pane <c:if test="${activeTab=='wktab'}">active</c:if>" id="wktab" >
									this is a tab pane 2
								</div>	
								<div class="tab-pane <c:if test="${activeTab=='wktab'}">active</c:if>" id="ustab" >
									this is a tab pane 2
								</div>					
				
						
										
					</div> <!-- /widget-content -->
					
				</div> <!-- /widget -->
				

				
				
				
				
				
				
				
				
			</div> <!-- /span9 -->
			
			
		</div> <!-- /row -->
		
	</div> <!-- /container -->
	
</div> <!-- /content -->
					
	</div>
</div>
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
<script src="/bicycle/js/charts/area.js"></script>
<script src="/bicycle/js/charts/line.js"></script>
<script src="/bicycle/js/charts/pie.js"></script>



  </body>
</html>
