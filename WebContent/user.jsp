<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8" />
<title>用车系统</title>

<meta name="viewport"
	content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
<meta name="apple-mobile-web-app-capable" content="yes" />

<link href="/bicycle/css/bootstrap.min.css" rel="stylesheet" />
<link href="/bicycle/css/bootstrap-responsive.min.css" rel="stylesheet" />

<link
	href="http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600"
	rel="stylesheet" />
<link href="/bicycle/css/font-awesome.css" rel="stylesheet" />

<link href="/bicycle/css/adminia.css" rel="stylesheet" />
<link href="/bicycle/css/adminia-responsive.css" rel="stylesheet" />


<link href="/bicycle/css/pages/plans.css" rel="stylesheet" />

<!-- Le HTML5 shim, for IE6-8 support of HTML5 elements -->
<!--[if lt IE 9]>
      <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
</head>
<c:if test="${!(empty sessionScope.message)}">
	<script type="text/javascript">
		alert('<c:out value="${sessionScope.message}"/>');
	</script>
	<c:remove var="message" scope="session" />
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
					<li class="active">
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
						<i class="icon-user"></i> 用户功能
					</h1>


					<%
						///////////////////////////////////////////////////////////////////
					%>
					<div class="tabbable">
						<ul class="nav nav-tabs">
							<li
								<c:if test="${activeTab!='pbtab' && activeTab!='uatab' }">class="active"</c:if>>
								<a href="/bicycle/userHomeServlet">功能</a> <!-- pbtab  -->
							</li>
							<li <c:if test="${activeTab=='uatab'}">class="active"</c:if>>
								<a href="/bicycle/listuseraccountServlet">个人账户记录</a> <!-- wktab data-toggle="tab" -->
							</li>
							<li <c:if test="${activeTab=='pbtab'}">class="active"</c:if>>
								<a href="/bicycle/listpersonalbikeServlet">个人自行车信息</a> <!-- wktab data-toggle="tab" -->
							</li>

						</ul>

						<div class="tab-content">
							<div
								class="tab-pane <c:if test="${activeTab!='pbtab' && activeTab!='uatab'}">active</c:if>"
								id="pbtab">
								<div class="widget">

									<div class="widget-header">
										<h3>用户用车</h3>
									</div>
									<!-- /widget-header -->

									<c:if test="${empty requestScope.order}">
										<div class="widget-content">
											<form id="edit-profile" action="/bicycle/usebikeServlet"
												class="form-horizontal" method="post">
												<fieldset>
		
													<div class="control-group">
		
		
														<label class="control-label" for="bike">自行车编号：</label>
														<div class="controls">
															<input type="text" pattern="[0-9]{1,11}" name="bike"
																class="input-medium" maxlength=11
																onkeyup="value=value.replace(/[^\d]/g,'') " required />
														</div>
														<!-- /controls -->
													</div>
													<!-- /control-group -->
		
													<div class="form-actions">
														<button type="submit" class="btn btn-primary">确定</button>
														<button type="reset" class="btn">重置</button>
													</div>
													<!-- /form-actions -->
												</fieldset>
											</form>
										</div>
									</c:if>
									<c:if test="${!(empty requestScope.order)}">
										<div class="widget-content">
										
										
											<table class="table table-striped table-bordered" >
											<thead>
											<tr>
												<th style="text-align: center;">订单ID</th>
 												<th style="text-align: center;">车辆ID</th>
 												<th style="text-align: center;">密码</th>
												<th style="text-align: center;">开始时间</th>
											</tr>
											</thead>
											<tbody>		
											<tr>
												<td style="text-align: center;">
													${requestScope.order.id }
												</td>
												<td style="text-align: center;">
													${requestScope.order.bike }
												</td>
												<td style="text-align: center;">
													${requestScope.key }
												</td>
												<td style="text-align: center;">
													${requestScope.order.start_time }
												</td>
											</tr>
											
											</tbody>						
									  </table>
											
											
											
											<form id="edit-profile" action="/bicycle/endUsebikeServlet?bike=${requestScope.order.bike}"
												class="form-horizontal" method="post">
												<fieldset>
													<div class="form-actions">
														<button type="submit" class="btn btn-primary">结束用车</button>
													</div>
													<!-- /form-actions -->
												</fieldset>
											</form>
										</div>
									</c:if>
									<!-- /widget-content -->

								</div>
								<!-- /widget -->


								<div class="widget">

									<div class="widget-header">
										<h3>用户充值</h3>
									</div>
									<!-- /widget-header -->

									<div class="widget-content">



										<form id="edit-profile" action="/bicycle/useraccountServlet"
											class="form-horizontal" method="post">
											<fieldset>

												<div class="control-group">


													<label class="control-label" for="bike">金额：</label>
													<div class="controls">
														<input type="text" pattern="[0-9]{1,5}(\.[0-9]{1,2})?"
															placeholder=输入金额，如:10或10.1或1000.01
															name="money" class="input-medium" maxlength=11 required />
													</div>
													<!-- /controls -->
												</div>
												<!-- /control-group -->

												<div class="form-actions">
													<button type="submit" class="btn btn-primary">确定</button>
													<button type="reset" class="btn">重置</button>
												</div>
												<!-- /form-actions -->
											</fieldset>
										</form>

									</div>
									<!-- /widget-content -->

								</div>
								<!-- /widget -->



								<div class="widget">

									<div class="widget-header">
										<h3>用户报修</h3>
									</div>
									<!-- /widget-header -->

									<div class="widget-content">



										<form id="edit-profile" onsubmit="return validate_form(this)"
											action="/bicycle/usermaintainServlet" class="form-horizontal"
											method="post">
											<fieldset>

												<div class="control-group">


													<label class="control-label" for="bike">自行车编号：</label>
													<div class="controls">
														<input type="text" pattern="[0-9A-Za-z]{1,11}" name="bike"
															class="input-medium" maxlength=11
															onkeyup="value=value.replace(/[^\d]/g,'') " required />
													</div>
													<!-- /controls -->
												</div>
												<!-- /control-group -->



												<div class="control-group">
													<label class="control-label" for="place">报修地点：</label>
													<div class="controls">
														<input type="text"
															name="place" class="input-medium" maxlength=100 required />
													</div>
													<!-- /controls -->
												</div>
												<!-- /control-group -->


												<div class="form-actions">
													<button type="submit" class="btn btn-primary">确定</button>
													<button type="reset" class="btn">重置</button>
												</div>
												<!-- /form-actions -->
											</fieldset>
										</form>

									</div>
									<!-- /widget-content -->

								</div>
								<!-- /widget -->



								<div class="widget">

									<div class="widget-header">
										<h3>个人自行车信息发布</h3>
									</div>
									<!-- /widget-header -->

									<div class="widget-content">



										<form id="edit-profile" onsubmit="return validate_form(this)"
											action="/bicycle/personal_bikeServlet"
											class="form-horizontal" method="post">
											<fieldset>

												<div class="control-group">
													<label class="control-label" for="tel">联系方式：</label>
													<div class="controls">
														<input type="text" pattern="[0-9A-Za-z]{1,13}" name="tel"
															class="input-medium" maxlength=13
															onkeyup="value=value.replace(/[^\d]/g,'') " required />
													</div>
													<!-- /controls -->
												</div>
												<!-- /control-group -->


												<div class="control-group">
													<label class="control-label" for="start_time">起始时间：</label>
													<div class="controls">
														<input type="date" name="start_time" class="input-medium"
															required />
													</div>
													<!-- /controls -->
												</div>
												<!-- /control-group -->


												<div class="control-group">
													<label class="control-label" for="end_time">终止时间：</label>
													<div class="controls">
														<input type="date" name="end_time" class="input-medium"
															required />
													</div>
													<!-- /controls -->
												</div>
												<!-- /control-group -->


												<div class="control-group">
													<label class="control-label" for="rent">租金：</label>
													<div class="controls">
														<input type="text" pattern="[0-9A-Za-z]{1,10}" name="rent"
															class="input-medium" maxlength=10
															onkeyup="value=value.replace(/[^\d]/g,'') " required />
													</div>
													<!-- /controls -->
												</div>
												<!-- /control-group -->


												<div class="control-group">
													<label class="control-label" for="desc">自行车简介：</label>
													<div class="controls">
														<input type="text" name="desc" class="input-medium"
															maxlength=200 required />
													</div>
													<!-- /controls -->
												</div>
												<!-- /control-group -->



												<div class="form-actions">
													<button type="submit" class="btn btn-primary">确定</button>
													<button type="reset" class="btn">重置</button>
												</div>
												<!-- /form-actions -->
											</fieldset>
										</form>

									</div>
									<!-- /widget-content -->

								</div>
								<!-- /widget -->







							</div>


														<div
								class="tab-pane <c:if test="${activeTab=='pbtab'}">active</c:if>"
								id="pbtab">

								<table class="table table-striped table-bordered">
									<thead>

										<tr>
											<th style="text-align: center;">id</th>
											<th style="text-align: center;">用户id</th>
											<th style="text-align: center;">手机号码</th>
											<th style="text-align: center;">起始时间</th>
											<th style="text-align: center;">终止时间</th>
											<th style="text-align: center;">租金</th>
											<th style="text-align: center;">自行车简介</th>
										</tr>
									</thead>

									<tbody>
										<c:forEach items="${requestScope.pblist}" var="personal_bike">
											<tr>
												<td style="text-align: center;">${personal_bike.id }</td>
												<td style="text-align: center;">${personal_bike.user }</td>
												<td style="text-align: center;">${personal_bike.tel }</td>
												<td style="text-align: center;">${personal_bike.start_time }</td>
												<td style="text-align: center;">${personal_bike.end_time }</td>
												<td style="text-align: center;">${personal_bike.rent }</td>
												<td style="text-align: center;">${personal_bike.desc }</td>
											</tr>
										</c:forEach>

									</tbody>
									<tbody>
										<tr>

											<td style="text-align: center;" colspan="9"><c:if
													test="${requestScope.pageCount!=0&&requestScope.curPage!=1}">
													<a
														href='<c:url value="/checkPersonalbikeServlet?curPage=1"/>'>首页</a>
													<a
														href='<c:url value="/checkPersonalbikeServlet?curPage=${requestScope.curPage-1}"/>'>前一页</a>
												</c:if> <c:if
													test="${requestScope.pageCount==0||requestScope.curPage==1}">首页 前一页</c:if>
												<c:if
													test="${requestScope.pageCount!=0&&requestScope.curPage!=requestScope.pageCount}">
													<a
														href='<c:url value="/checkPersonalbikeServlet?curPage=${requestScope.curPage+1}"/>'>下一页</a>
													<a
														href='<c:url value="/checkPersonalbikeServlet?curPage=${requestScope.pageCount}"/>'>尾页</a>
												</c:if> <c:if
													test="${requestScope.pageCount==0||requestScope.curPage==requestScope.pageCount}">下一页 尾页</c:if>
												&nbsp; 第${requestScope.curPage}页/共${requestScope.pageCount}页
												&nbsp;</td>


										</tr>
									</tbody>

								</table>


							</div>

							<div
								class="tab-pane <c:if test="${activeTab=='uatab'}">active</c:if>"
								id="uatab">


								<table class="table table-striped table-bordered">
									<thead>

										<tr>
											<th style="text-align: center;">id</th>
											<th style="text-align: center;">交易金额</th>
											<th style="text-align: center;">时间</th>
										</tr>
									</thead>

									<tbody>
										<c:forEach items="${requestScope.ualist}" var="user_account">
											<tr>
												<td style="text-align: center;">${user_account.id }</td>
												<td style="text-align: center;">${user_account.money }</td>
												<td style="text-align: center;">${user_account.time }</td>
											</tr>
										</c:forEach>

									</tbody>
									<tbody>
										<tr>

											<td style="text-align: center;" colspan="9"><c:if
													test="${requestScope.pageCount!=0&&requestScope.curPage!=1}">
													<a
														href='<c:url value="/checkUserAccountServlet?curPage=1&loginname=${requestScope.loginname}"/>'>首页</a>
													<a
														href='<c:url value="/checkUserAccountServlet?curPage=${requestScope.curPage-1}&loginname=${requestScope.loginname}"/>'>前一页</a>
												</c:if> <c:if
													test="${requestScope.pageCount==0||requestScope.curPage==1}">首页 前一页</c:if>
												<c:if
													test="${requestScope.pageCount!=0&&requestScope.curPage!=requestScope.pageCount}">
													<a
														href='<c:url value="/checkUserAccountServlet?curPage=${requestScope.curPage+1}&loginname=${requestScope.loginname}"/>'>下一页</a>
													<a
														href='<c:url value="/checkUserAccountServlet?curPage=${requestScope.pageCount}&loginname=${requestScope.loginname}"/>'>尾页</a>
												</c:if> <c:if
													test="${requestScope.pageCount==0||requestScope.curPage==requestScope.pageCount}">下一页 尾页</c:if>
												&nbsp; 第${requestScope.curPage}页/共${requestScope.pageCount}页
												&nbsp;</td>


										</tr>
									</tbody>

								</table>

							</div>
						</div>


						<%
							///////////////////////////////////////////////////////////////
						%>








					</div>
					<!-- /span9 -->


				</div>
				<!-- /row -->

			</div>
			<!-- /container -->

		</div>
	</div>	<!-- /content -->


		<div id="footer">

			<div class="container">
				<hr />
				<p>&copy; 2016 Bicycle.</p>
			</div>
			<!-- /container -->

		</div>
		<!-- /footer -->




		<!-- Le javascript
================================================== -->
		<!-- Placed at the end of the document so the pages load faster -->
		<script src="/bicycle/js/jquery-1.7.2.min.js"></script>


		<script src="/bicycle/js/bootstrap.js"></script>
</body>
</html>
