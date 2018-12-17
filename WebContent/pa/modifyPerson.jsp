<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html>
<html lang="en">
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
					<p>邮箱: xyf1@buaa.edu.cn</p>
				</div> <!-- .sidebar-extra -->
				
				<br />
		
			</div> <!-- /span3 -->
			
			
			
			<div class="span9">
				
				<h1 class="page-title">
					<i class="icon-th-list"></i>
					修改
					<c:if test="${requestScope.type=='admin'}">管理员</c:if>
					<c:if test="${requestScope.type=='worker'}">工人</c:if>
					<c:if test="${requestScope.type=='user'}">用户</c:if>
					信息			
				</h1>
				
				
				<div class="row">
					
					<div class="span9">
				
						<div class="widget">
							
							<div class="widget-header">
								<h3>基本信息</h3>
							</div> <!-- /widget-header -->
									
							<div class="widget-content">
								
								
								
								<form id="edit-profile" 
								<c:if test="${requestScope.type=='admin' }">action="/bicycle/pa/modifyAdminServlet"</c:if>
								<c:if test="${requestScope.type=='worker' }">action="/bicycle/pa/modifyWorkerServlet"</c:if>
								<c:if test="${requestScope.type=='user' }">action="/bicycle/pa/modifyUserServlet"</c:if>
								class="form-horizontal" method="post" >
								<fieldset>
										
										<div class="control-group">											
											<label class="control-label" for="loginname">登录名：</label>
											<div class="controls">
												<input type="text" class="input-medium disabled" 
												name="loginname"
												id="loginname" 
												<c:if test="${requestScope.type=='admin' }">value="${requestScope.admin.loginname }"</c:if>
												<c:if test="${requestScope.type=='worker' }">value="${requestScope.worker.loginname }"</c:if>
												<c:if test="${requestScope.type=='user' }">value="${requestScope.user.loginname }"</c:if>
												 readonly />
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
												<c:if test="${requestScope.type=='admin' }">value="${requestScope.admin.name }"</c:if>
												<c:if test="${requestScope.type=='worker' }">value="${requestScope.worker.name }"</c:if>
												<c:if test="${requestScope.type=='user' }">value="${requestScope.user.name }"</c:if>
												/>
											</div> <!-- /controls -->				
										</div> <!-- /control-group -->
										
										
										<c:if test="${requestScope.type!='worker' }">
										<div class="control-group">											
											<label class="control-label" for="tel">手机号码：</label>
											<div class="controls">
												<input type="text" class="input-large" 
												id="tel" 
												name="tel"
												pattern="[0-9]{11}"
												maxlength=11
												<c:if test="${requestScope.type=='admin' }">value="${requestScope.admin.tel }"</c:if>
												<c:if test="${requestScope.type=='user' }">value="${requestScope.user.tel }"</c:if>
												/>
												<p class="help-block">输入11位手机号码</p>
											</div> <!-- /controls -->				
										</div> <!-- /control-group -->
										
										</c:if>
										
										
										<br />
										
										<div class="control-group">											
											<label class="control-label" for="password">密码：</label>
											<div class="controls">
												<input type="text" 
												pattern=".{6,18}"
												name="password"
												maxlength=18
												class="input-medium" id="password" 
												<c:if test="${requestScope.type=='admin' }">value="${requestScope.admin.password }"</c:if>
												<c:if test="${requestScope.type=='worker' }">value="${requestScope.worker.password }"</c:if>
												<c:if test="${requestScope.type=='user' }">value="${requestScope.user.password }"</c:if>
												 required/>
												 <p class="help-block">输入6-18位密码</p>
											</div> <!-- /controls -->				
										</div> <!-- /control-group -->
										
										<c:if test="${requestScope.type=='worker'}">
											<div class="control-group">
												<label class="control-label" for="department">员工类型</label>
												<div class="controls">
													<label>
														<input type="radio" name="department" value="B"
														<c:if test="${requestScope.worker.department=='B' }">checked="checked"</c:if>
														 id="department" 
														 disabled
														 />
														采购员
													</label>
													<label>
														<input type="radio" name="department" value="M"
														<c:if test="${requestScope.worker.department=='M' }">checked="checked"</c:if>
														 disabled
														 />
														维修工
													</label>
													<p class="help-block">部门不支持更改</p>
												</div>
											</div>
											</c:if>	
											
											<c:if test="${requestScope.type=='worker'}">
											<div class="control-group">
												<label class="control-label" for="salary">月工资</label>
												<div class="controls">
													<input type="text" 
														pattern="[0-9]{1,6}"
														name="salary"
														maxlength=6
														class="input-medium" id="salary" 
														value='${requestScope.worker.salary }'
														 />
													<p class="help-block">工资暂不支持更改</p>
												</div>
											</div>
											<c:if test="${requestScope.worker.department=='M' }">
											<div class="control-group">
												<label class="control-label" for="cnt">处理维修次数</label>
												<div class="controls">
													<input type="text" 
														pattern="[0-9]{1,4}"
														name="cnt"
														
														class="input-medium" id="cnt" 
														value='${requestScope.worker.cnt }'
														 readonly/>
													<p class="help-block">不支持更改</p>
												</div>
											</div>
											</c:if>
											</c:if>	
											
											<c:if test="${requestScope.type!='admin'}">
											<div class="control-group">
												<label class="control-label" for="balance">余额</label>
												<div class="controls">
													<input type="text" 
														pattern="[0-9]{1,10}"
														name="balance"
														maxlength=10
														class="input-medium" id="balance" 
														<c:if test="${requestScope.type=='worker' }">value='${requestScope.worker.balance }'</c:if>
														<c:if test="${requestScope.type=='user' }">value='${requestScope.user.balance }'</c:if>
														 readonly/>
													<p class="help-block">余额暂不支持更改</p>
												</div>
											</div>
											</c:if>	
											
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
