<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
    <%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <title>报修系统</title>
    
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <meta name="apple-mobile-web-app-capable" content="yes" />    
    
    <link href="/bicycle/css/bootstrap.min.css" rel="stylesheet" />
    <link href="/bicycle/css/bootstrap-responsive.min.css" rel="stylesheet" />
    
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
					
					<li>
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
					
					<c:if test="${sessionScope.person.type==3 }">
					<li>
						<a href="/bicycle/userHomeServlet">
							<i class="icon-user"></i>
							用车系统		
						</a>
					</li>
					</c:if>
					
					<c:if test="${sessionScope.person.type==1 }">
					<li>
						<a href="/bicycle/purchase.jsp">
							<i class="icon-shopping-cart"></i>
							采购系统		
						</a>
					</li>
					</c:if>
					
					<c:if test="${sessionScope.person.type==2 }">
					<li class="active">
						<a href="/bicycle/ma/listMaintainServlet">
							<i class="icon-check"></i>
							报修系统
							<!-- <span class="label label-warning pull-right">5</span> -->
						</a>
					</li>
					</c:if>
					
					<c:if test="${sessionScope.person.type==0 }">
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
					<i class="icon-check"></i>
					报修系统				
				</h1>
				
					<div class="widget-header">
							<h3>基本信息</h3>
					</div> <!-- /widget-header -->
					
					<div class="widget-content">
						<table class="table table-striped table-bordered">
							<thead>
							
							<tr>
								<th style="text-align: center;">id</th>
								<th style="text-align: center;">用户ID</th>
								<th style="text-align: center;">车辆编号</th>
								<th style="text-align: center;">位置</th>
								<th style="text-align: center;">时间</th>
								<th style="text-align: center;">接单维修工</th>
								<th style="text-align: center;">操作</th>
							</tr>
							</thead>
							
							<tbody>		
							<c:forEach items="${requestScope.matlist}" var="mat">						
								
									<tr>
									<td style="text-align: center;">
										${mat.id }
									</td>
									<td style="text-align: center;">
										${mat.user }
									</td>
									<td style="text-align: center;">
										${mat.bike }
									</td>
									<td style="text-align: center;">
										${mat.place }
									</td>
									<td style="text-align: center;">
										${mat.time }
									</td>
									
									<td style="text-align: center;">
									<c:if test="${requestScope.mid==mat.maintainer }">
										<a href="/bicycle/personDetailServlet">本人</a>
									</c:if>
									<c:if test="">
										${mat.maintainer }
									</c:if>
									</td>
									<c:if test="${requestScope.mid==mat.maintainer }">
									<td style="text-align: center;" class="action-td">
										<a href='<c:url value="/ma/relyMaintainServlet?id=${mat.id }"></c:url>' class="btn btn-small btn-warning">
											<i class="icon-ok"></i>								
										</a>						
										<!-- <a href='<c:url value="/pa/relyPurchaseServlet?decision=false&id=${purchase.id }"></c:url>' class="btn btn-small">
											<i class="icon-remove"></i>						
										</a> -->
									</td>
									</c:if>
									<c:if test="${requestScope.mid!=mat.maintainer }">
									<td style="text-align: center;" class="action-td">他人接单</td>
									</c:if>
								</tr>
								
								
								
							</c:forEach>							
							
							</tbody>						
							<tbody>	
							<tr>
								
								<td style="text-align: center;" colspan="9">
								<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=1}">
									<a href='<c:url value="/ma/checkMaintainServlet?curPage=1&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>首页</a>
									<a href='<c:url value="/ma/checkMaintainServlet?curPage=${requestScope.curPage-1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>前一页</a>
								</c:if>
								<c:if test="${requestScope.pageCount==0||requestScope.curPage==1}">首页 前一页</c:if>
								<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=requestScope.pageCount}">
									<a href='<c:url value="/ma/checkMaintainServlet?curPage=${requestScope.curPage+1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>下一页</a>
									<a href='<c:url value="/ma/checkMaintainServlet?curPage=${requestScope.pageCount}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>尾页</a>
								</c:if>
								<c:if test="${requestScope.pageCount==0||requestScope.curPage==requestScope.pageCount}">下一页 尾页</c:if>
								&nbsp;
								第${requestScope.curPage}页/共${requestScope.pageCount}页
								&nbsp;
				               </td>
			
				               
							</tr>
							</tbody>	
							
					  </table>
					
					
					
					</div><!-- widget-content -->
				
				</div> <!-- /span9 -->
			
			
		</div> <!-- /row -->
		
	</div> <!-- /container -->
	
</div> <!-- /content -->
					
	
<div id="footer">
	
	<div class="container">				
		<hr />
		<p>&copy; 2018 Bicycle.</p>
	</div> <!-- /container -->
	
</div> <!-- /footer -->


    

<!-- Le javascript
================================================== -->
<!-- Placed at the end of the document so the pages load faster -->
<script src="/bicycle/js/jquery-1.7.2.min.js"></script>


<script src="/bicycle/js/bootstrap.js"></script>

  </body>
</html>
