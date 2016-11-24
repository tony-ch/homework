<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
    <%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <title>管理系统</title>
    
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
					<li>
						<a href="/bicycle/ma/listMaintainServlet">
							<i class="icon-check"></i>
							报修系统
							<!-- <span class="label label-warning pull-right">5</span> -->
						</a>
					</li>
					</c:if>
					
					<c:if test="${sessionScope.person.type==0 }">
					<li class="active">
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
					<p>邮箱: @buaa.edu.cn</p>
				</div> <!-- .sidebar-extra -->
				
				<br />
		
			</div> <!-- /span3 -->
			
			
			
			<div class="span9">
				
				<h1 class="page-title">
					<i class="icon-th-list"></i>
					管理系统					
				</h1>
				
				
				
				
				<div class="widget">
					
					<div class="widget-header">
						<h3>信息</h3>
					</div> <!-- /widget-header -->
														
					<div class="widget-content">
						
						<div class="tabbable">
						<ul class="nav nav-tabs">
						  <li <c:if test="${activeTab=='admtab'}">class="active"</c:if>>
						    <a href="/bicycle/pa/listAdminServlet">管理员</a><!-- admtab data-toggle="tab" -->
						  </li>
						  <li <c:if test="${activeTab=='wktab'}">class="active"</c:if>>
						  	<a href="/bicycle/pa/listWorkerServlet">员工</a><!-- wktab data-toggle="tab" -->
						  </li>
						  <li <c:if test="${activeTab=='ustab'}">class="active"</c:if>>
						  	<a href="/bicycle/pa/listUserServlet">用户</a><!-- ustab data-toggle="tab" -->
						  </li>
						   <li <c:if test="${activeTab=='biketab'}">class="active"</c:if>>
						  	<a href="/bicycle/pa/listBikeServlet">车辆</a><!-- ustab data-toggle="tab" -->
						  </li>
						  <li <c:if test="${activeTab=='ordtab'}">class="active"</c:if>>
						  	<a href="/bicycle/pa/listOrderServlet">用户订单</a><!-- ustab data-toggle="tab" -->
						  </li>
						  
						  <li <c:if test="${activeTab=='mattab'}">class="active"</c:if>>
						  	<a href="/bicycle/pa/listAllMaintainServlet">报修单</a><!-- ustab data-toggle="tab" -->
						  </li>
						   <li <c:if test="${activeTab=='purtab'}">class="active"</c:if>>
						  	<a href="/bicycle/pa/listPurchaseServlet">采购单</a><!-- ustab data-toggle="tab" -->
						  </li>
						  <li <c:if test="${activeTab=='acttab'}">class="active"</c:if>>
						  	<a href="/bicycle/pa/listComAccountServlet">账户变动</a><!-- ustab data-toggle="tab" -->
						  </li>
						  
						</ul>
						
							<div class="tab-content">
								<div class="tab-pane <c:if test="${activeTab=='admtab'}">active</c:if>"   id="admtab">
										<table class="table table-striped table-bordered">
											<thead>
											<tr>
												<th style="text-align: center;">id</th>
												<th style="text-align: center;">登录名</th>
												<th style="text-align: center;">姓名</th>
												<th style="text-align: center;">密码</th>
												<th style="text-align: center;">手机号码</th>
												<th style="text-align: center;">操作</th>
											</tr>
											</thead>
											
											<tbody>		
											<c:forEach items="${requestScope.adlist}" var="admin">						
											<tr>
												<td style="text-align: center;">
													${admin.id }
												</td>
												<td style="text-align: center;">
													<a href='<c:url value="/pa/showPersonServlet?type=admin&loginname=${admin.loginname }"></c:url>'>${admin.loginname }</a>
												</td>
												<td style="text-align: center;">
													${admin.name }
												</td>
												<td style="text-align: center;">
													${admin.password }
												</td>
												<td style="text-align: center;">
													${admin.tel }
												</td>
												<td style="text-align: center;">
													<a href='<c:url value="/pa/deleteAdminServlet?loginname=${admin.loginname }"></c:url>'><font color=red>删除</font></a> | 
													<a href='<c:url value="/pa/showPersonServlet?type=admin&loginname=${admin.loginname }"></c:url>'><font color=red>查看/修改</font></a>
												</td>
											</tr>
											</c:forEach>							
											
											</tbody>						
											<tbody>	
											<tr>
												<td style="text-align: center;" align="right" colspan="9">
												<a href='<c:url value="/pa/addPerson.jsp?type=admin"></c:url>'><font color=red>添加</font></a>
												</td>
											</tr>
											<tr>
												
												<td style="text-align: center;" colspan="9">
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
									<table class="table table-striped table-bordered">
											<thead>
											<tr>
												<th style="text-align: center;">id</th>
												<th style="text-align: center;">登录名</th>
												<th style="text-align: center;">姓名</th>
												<th style="text-align: center;">密码</th>
												<th style="text-align: center;">部门</th>
												<th style="text-align: center;">操作</th>
											</tr>
											</thead>
											<tbody>		
											<c:forEach items="${requestScope.wklist}" var="worker">						
											<tr>
												<td style="text-align: center;">
													${worker.id }
												</td>
												<td style="text-align: center;">
													<a href='<c:url value="/pa/showPersonServlet?type=worker&loginname=${worker.loginname }"></c:url>'>${worker.loginname }</a>
												</td>
												<td style="text-align: center;">
													${worker.name }
												</td>
												<td style="text-align: center;">
													${worker.password }
												</td>
												<td style="text-align: center;">
													<c:if test="${worker.department=='B' }">采购员</c:if>
													<c:if test="${worker.department=='M' }">维修工</c:if>
												</td>
												<td style="text-align: center;">
													<a href='<c:url value="/pa/deleteWorkerServlet?loginname=${worker.loginname }"></c:url>'><font color=red>删除</font></a> | 
													<a href='<c:url value="/pa/showPersonServlet?type=worker&loginname=${worker.loginname }"></c:url>'><font color=red>查看/修改</font></a>
												</td>
											</tr>
											</c:forEach>							
											
											</tbody>						
											<tbody>	
											<tr>
												<td style="text-align: center;" align="right" colspan="9">
												<a href='<c:url value="/pa/addPerson.jsp?type=worker"></c:url>'><font color=red>添加</font></a>
												</td>
											</tr>
											<tr>
												
												<td style="text-align: center;" colspan="9">
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=1}">
													<a href='<c:url value="/pa/checkWorkerServlet?curPage=1&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>首页</a>
													<a href='<c:url value="/pa/checkWorkerServlet?curPage=${requestScope.curPage-1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>前一页</a>
												</c:if>
												<c:if test="${requestScope.pageCount==0||requestScope.curPage==1}">首页 前一页</c:if>
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=requestScope.pageCount}">
													<a href='<c:url value="/pa/checkWorkerServlet?curPage=${requestScope.curPage+1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>下一页</a>
													<a href='<c:url value="/pa/checkWorkerServlet?curPage=${requestScope.pageCount}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>尾页</a>
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
								<!-- 用户 -->	
								<div class="tab-pane <c:if test="${activeTab=='ustab'}">active</c:if>" id="ustab" >
									<table class="table table-striped table-bordered">
											<thead>
											<tr>
												<th style="text-align: center;">id</th>
												<th style="text-align: center;">登录名</th>
												<th style="text-align: center;">姓名</th>
												<th style="text-align: center;">密码</th>
												<th style="text-align: center;">手机号码</th>
												<th style="text-align: center;">操作</th>
											</tr>
											</thead>
											<tbody>		
											<c:forEach items="${requestScope.uslist}" var="user">						
											<tr>
												<td style="text-align: center;">
													${user.id }
												</td>
												<td style="text-align: center;">
													<a href='<c:url value="/pa/showPersonServlet?type=user&loginname=${user.loginname }"></c:url>'>${user.loginname }</a>
												</td>
												<td style="text-align: center;">
													${user.name }
												</td>
												<td style="text-align: center;">
													${user.password }
												</td>
												<td style="text-align: center;">
													${user.tel }
												</td>
												<td style="text-align: center;">
													<a href='<c:url value="/pa/deleteUserServlet?loginname=${user.loginname }"></c:url>'><font color=red>删除</font></a> | 
													<a href='<c:url value="/pa/showPersonServlet?type=user&loginname=${user.loginname }"></c:url>'><font color=red>查看/修改</font></a>
												</td>
											</tr>
											</c:forEach>							
											</tbody>						
											<tbody>	
											<tr>
												<td style="text-align: center;" align="right" colspan="9">
												<a href='<c:url value="/pa/addPerson.jsp?type=user"></c:url>'><font color=red>添加</font></a>
												</td>
											</tr>
											<tr>
												<td style="text-align: center;" colspan="9">
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=1}">
													<a href='<c:url value="/pa/checkUserServlet?curPage=1&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>首页</a>
													<a href='<c:url value="/pa/checkUserServlet?curPage=${requestScope.curPage-1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>前一页</a>
												</c:if>
												<c:if test="${requestScope.pageCount==0||requestScope.curPage==1}">首页 前一页</c:if>
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=requestScope.pageCount}">
													<a href='<c:url value="/pa/checkUserServlet?curPage=${requestScope.curPage+1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>下一页</a>
													<a href='<c:url value="/pa/checkUserServlet?curPage=${requestScope.pageCount}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>尾页</a>
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
								<!-- 车辆 -->
								<div class="tab-pane <c:if test="${activeTab=='biketab'}">active</c:if>" id="biketab" >
									<table class="table table-striped table-bordered">
											<thead>
											
											<tr >
												<th style="text-align: center;" >id</th>
												<th style="text-align: center;">密码</th>
												<th style="text-align: center;">状态</th>
											</tr>
											</thead>
											
											<tbody>		
											<c:forEach items="${requestScope.bikelist}" var="bike">						
											<tr>
												<td style="text-align: center;" >
													${bike.id }
												</td>
												<td style="text-align: center;">
													${bike.key }
												</td>
												<td style="text-align: center;">
													<c:if test="${bike.state=='I' }">空闲</c:if>
													<c:if test="${bike.state=='U' }">占用</c:if>
													<c:if test="${bike.state=='R' }">维修</c:if>
												</td>
							
									
												<!-- 
												<td style="text-align: center;" class="action-td">
													<a href='<c:url value="/pa/relyPurchaseServlet?id=${purchase.id }&decision=true"></c:url>' class="btn btn-small btn-warning">
														<i class="icon-ok"></i>								
													</a>						
													<a href='<c:url value="/pa/relyPurchaseServlet?id=${purchase.id }&decision=false"></c:url>' class="btn btn-small">
														<i class="icon-remove"></i>						
													</a>
												</td>
												 -->
											</tr>
											</c:forEach>							
											
											</tbody>						
											<tbody>	
											<tr>
												
												<td style="text-align: center;" colspan="9">
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=1}">
													<a href='<c:url value="/pa/checkBikeServlet?curPage=1&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>首页</a>
													<a href='<c:url value="/pa/checkBikeServlet?curPage=${requestScope.curPage-1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>前一页</a>
												</c:if>
												<c:if test="${requestScope.pageCount==0||requestScope.curPage==1}">首页 前一页</c:if>
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=requestScope.pageCount}">
													<a href='<c:url value="/pa/checkBikeServlet?curPage=${requestScope.curPage+1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>下一页</a>
													<a href='<c:url value="/pa/checkBikeServlet?curPage=${requestScope.pageCount}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>尾页</a>
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
								<!-- 订单 -->
								<div class="tab-pane <c:if test="${activeTab=='ordtab'}">active</c:if>" id="ordtab" >
									<table class="table table-striped table-bordered" >
											<thead>
											
											<tr>
												<th style="text-align: center;">订单ID</th>
												<th style="text-align: center;">用户ID</th>
												<th style="text-align: center;">车辆ID</th>
												<th style="text-align: center;">开始时间</th>
											</tr>
											</thead>
											
											<tbody>		
											<c:forEach items="${requestScope.ordlist}" var="order">						
											<tr>
												<td style="text-align: center;">
													${order.id }
												</td>
												<td style="text-align: center;">
													${order.user }
												</td>
												<td style="text-align: center;">
													${order.bike }
												</td>
												<td style="text-align: center;">
													${order.start_time }
												</td>
									
												<!-- 
												<td style="text-align: center;" class="action-td">
													<a href='<c:url value="/pa/relyPurchaseServlet?id=${purchase.id }&decision=true"></c:url>' class="btn btn-small btn-warning">
														<i class="icon-ok"></i>								
													</a>						
													<a href='<c:url value="/pa/relyPurchaseServlet?id=${purchase.id }&decision=false"></c:url>' class="btn btn-small">
														<i class="icon-remove"></i>						
													</a>
												</td>
												 -->
											</tr>
											</c:forEach>							
											
											</tbody>						
											<tbody>	
											<tr>
												
												<td style="text-align: center;" colspan="9">
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=1}">
													<a href='<c:url value="/pa/checkOrderServlet?curPage=1&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>首页</a>
													<a href='<c:url value="/pa/checkOrderServlet?curPage=${requestScope.curPage-1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>前一页</a>
												</c:if>
												<c:if test="${requestScope.pageCount==0||requestScope.curPage==1}">首页 前一页</c:if>
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=requestScope.pageCount}">
													<a href='<c:url value="/pa/checkOrderServlet?curPage=${requestScope.curPage+1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>下一页</a>
													<a href='<c:url value="/pa/checkOrderServlet?curPage=${requestScope.pageCount}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>尾页</a>
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
								
								<!-- 报修单管理 -->
								<div class="tab-pane <c:if test="${activeTab=='mattab'}">active</c:if>" id="mattab" >
									<table class="table table-striped table-bordered">
											<thead>
											
											<tr>
												<th style="text-align: center;">id</th>
												<th style="text-align: center;">用户id</th>
												<th style="text-align: center;">车辆号</th>
												<th style="text-align: center;">地点</th>
												<th style="text-align: center;">时间</th>
												<th style="text-align: center;">维修工ID</th>
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
													${mat.maintainer }
												</td>
											</tr>
											</c:forEach>							
											
											</tbody>						
											<tbody>	
											<tr>
												
												<td style="text-align: center;" colspan="9">
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=1}">
													<a href='<c:url value="/pa/listAllMaintainServlet?curPage=1"/>'>首页</a>
													<a href='<c:url value="/pa/listAllMaintainServlet?curPage=${requestScope.curPage-1}"/>'>前一页</a>
												</c:if>
												<c:if test="${requestScope.pageCount==0||requestScope.curPage==1}">首页 前一页</c:if>
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=requestScope.pageCount}">
													<a href='<c:url value="/pa/listAllMaintainServlet?curPage=${requestScope.curPage+1}"/>'>下一页</a>
													<a href='<c:url value="/pa/listAllMaintainServlet?curPage=${requestScope.pageCount}"/>'>尾页</a>
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
								<!-- 采购单管理 -->
								<div class="tab-pane <c:if test="${activeTab=='purtab'}">active</c:if>" id="purtab" >
									<table class="table table-striped table-bordered">
											<thead>
											
											<tr>
												<th style="text-align: center;">id</th>
												<th style="text-align: center;">申请者id</th>
												<th style="text-align: center;">车辆数目</th>
												<th style="text-align: center;">金额</th>
												<th style="text-align: center;">时间</th>
												<th style="text-align: center;">操作</th>
											</tr>
											</thead>
											
											<tbody>		
											<c:forEach items="${requestScope.purlist}" var="purchase">						
											<tr>
												<td style="text-align: center;">
													${purchase.id }
												</td>
												<td style="text-align: center;">
													${purchase.buyer }
												</td>
												<td style="text-align: center;">
													${purchase.bikenum }
												</td>
												<td style="text-align: center;">
													${purchase.money }
												</td>
												<td style="text-align: center;">
													${purchase.time }
												</td>
												<td style="text-align: center;" class="action-td">
													<a href='<c:url value="/pa/relyPurchaseServlet?decision=true&id=${purchase.id }"></c:url>' class="btn btn-small btn-warning">
														<i class="icon-ok"></i>								
													</a>						
													<a href='<c:url value="/pa/relyPurchaseServlet?decision=false&id=${purchase.id }"></c:url>' class="btn btn-small">
														<i class="icon-remove"></i>						
													</a>
												</td>
											</tr>
											</c:forEach>							
											
											</tbody>						
											<tbody>	
											<tr>
												
												<td style="text-align: center;" colspan="9">
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=1}">
													<a href='<c:url value="/pa/checkPurchaseServlet?curPage=1&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>首页</a>
													<a href='<c:url value="/pa/checkPurchaseServlet?curPage=${requestScope.curPage-1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>前一页</a>
												</c:if>
												<c:if test="${requestScope.pageCount==0||requestScope.curPage==1}">首页 前一页</c:if>
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=requestScope.pageCount}">
													<a href='<c:url value="/pa/checkPurchaseServlet?curPage=${requestScope.curPage+1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>下一页</a>
													<a href='<c:url value="/pa/checkPurchaseServlet?curPage=${requestScope.pageCount}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>尾页</a>
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
								<!-- 账户变动 -->
								<div class="tab-pane <c:if test="${activeTab=='acttab'}">active</c:if>" id="acttab" >
									<table class="table table-striped table-bordered">
											<thead>
											
											<tr>
												<th style="text-align: center;">id</th>
												<th style="text-align: center;">金额</th>
												<th style="text-align: center;">时间</th>
												<th style="text-align: center;">管理员ID</th>
												<th style="text-align: center;">员工ID</th>
												<th style="text-align: center;">用途</th>
											</tr>
											</thead>
											
											<tbody>		
											<c:forEach items="${requestScope.actlist}" var="actrecord">						
											<tr>
												<td style="text-align: center;">
													${actrecord.id }
												</td>
												<td style="text-align: center;">
													${actrecord.money }
												</td>
												<td style="text-align: center;">
													${actrecord.time }
												</td>
												<td style="text-align: center;">
													<c:if test="${actrecord.admin!=0 }">${actrecord.admin}</c:if>
												</td>
												<td style="text-align: center;">
													${actrecord.worker }
												</td>
												<td style="text-align: center;">
													<c:if test="${actrecord.type=='S'  }">工资</c:if>
													<c:if test="${actrecord.type=='B'  }">采购</c:if>
												</td>
												<!-- 
												<td style="text-align: center;" class="action-td">
													<a href='<c:url value="/pa/relyPurchaseServlet?id=${purchase.id }&decision=true"></c:url>' class="btn btn-small btn-warning">
														<i class="icon-ok"></i>								
													</a>						
													<a href='<c:url value="/pa/relyPurchaseServlet?id=${purchase.id }&decision=false"></c:url>' class="btn btn-small">
														<i class="icon-remove"></i>						
													</a>
												</td>
												 -->
											</tr>
											</c:forEach>							
											
											</tbody>						
											<tbody>	
											<tr>
												
												<td style="text-align: center;" colspan="9">
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=1}">
													<a href='<c:url value="/pa/checkComAccountServlet?curPage=1&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>首页</a>
													<a href='<c:url value="/pa/checkComAccountServlet?curPage=${requestScope.curPage-1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>前一页</a>
												</c:if>
												<c:if test="${requestScope.pageCount==0||requestScope.curPage==1}">首页 前一页</c:if>
												<c:if test="${requestScope.pageCount!=0&&requestScope.curPage!=requestScope.pageCount}">
													<a href='<c:url value="/pa/checkComAccountServlet?curPage=${requestScope.curPage+1}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>下一页</a>
													<a href='<c:url value="/pa/checkComAccountServlet?curPage=${requestScope.pageCount}&loginname=${requestScope.loginname}&name=${requestScope.name}&password=${requestScope.password}"/>'>尾页</a>
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
