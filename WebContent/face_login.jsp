<%@ page language="java" contentType="text/html; charset=UTF-8"
         pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8" />
    <title>登录页面</title>

    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <meta name="apple-mobile-web-app-capable" content="yes" />

    <link href="/bicycle/css/bootstrap.min.css" rel="stylesheet" />
    <link href="/bicycle/css/bootstrap-responsive.min.css" rel="stylesheet" />

    <%--<link href="http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600" rel="stylesheet" />--%>
    <link href="/bicycle/css/font-awesome.css" rel="stylesheet" />

    <link href="/bicycle/css/adminia.css" rel="stylesheet" />
    <link href="/bicycle/css/adminia-responsive.css" rel="stylesheet" />

    <link href="/bicycle/css/pages/login.css" rel="stylesheet" />

    <!-- Le HTML5 shim, for IE6-8 support of HTML5 elements -->
    <%--<!--[if lt IE 9]>--%>
    <%--<script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>--%>
    <%--<![endif]-->--%>

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

                    <li class="">

                        <a href="/bicycle/index.jsp"><i class="icon-chevron-left"></i> 返回首页</a>
                    </li>
                </ul>

            </div> <!-- /nav-collapse -->

        </div> <!-- /container -->

    </div> <!-- /navbar-inner -->

</div> <!-- /navbar -->


<div id="login-container">


    <div id="login-header">

        <h3>登录</h3>

    </div> <!-- /login-header -->

    <div id="login-content" class="clearfix">
        <form name="loginform" action="/bicycle/faceLoginServlet"  method="post">
            <fieldset>
                <div class="control-group">
                    <label class="control-label" for="loginname">登录名</label>
                    <div class="controls">
                        <input type="text" maxlength="20" class="" id="loginname" name="loginname" required/>
                    </div>
                    <div class="controls">
                        <input type="hidden" class="" id="imgurl" name="imgurl" value="123"/>
                    </div>
                    <div class="controls">
                        <div id="my_camera"></div>
                    </div>

                </div>
            </fieldset>

            <!-- <div id="remember-me" class="pull-left">
                <input type="checkbox" name="remember" id="remember" />
                <label id="remember-label" for="remember">记住密码</label>
            </div> -->
            <div id="remember-me" class="pull-left">
                <label><input name="type" type="radio" value="user" checked/>用户 </label>
                <label><input name="type" type="radio" value="worker" />工人 </label>
                <label><input name="type" type="radio" value="admin" />管理员 </label>
            </div>

            <div class="pull-right">
                <button type="button" onclick="location.href='/bicycle/login.jsp'" class="btn btn-large">密码登录</button>
                <button type="submit" onclick="take_snapshot()" class="btn btn-warning btn-large">
                    登录
                </button>
            </div>
        </form>

    </div> <!-- /login-content -->


    <div id="login-extra">

        <p>还没有账号? <a href="/bicycle/register.jsp;">马上注册.</a></p>
        <!--<p>没有录入人脸信息？ <a href="/bicycle/login.jsp;">账号密码登录.</a></p>-->

        <!--<p>忘记密码? <a href="forgot_password.html">发送邮件.</a></p>  -->

    </div> <!-- /login-extra -->

</div> <!-- /login-wrapper -->

<!-- Le javascript
================================================== -->
<!-- Placed at the end of the document so the pages load faster -->
<script src="/bicycle/js/jquery-1.7.2.min.js"></script>


<script src="/bicycle/js/bootstrap.js"></script>
<!-- First, include the Webcam.js JavaScript Library -->
<script type="text/javascript" src="/bicycle/js/webcam.js"></script>

<!-- Configure a few settings and attach camera -->
<script language="JavaScript">
    var os = function() {
        var ua = navigator.userAgent,
            isWindowsPhone = /(?:Windows Phone)/.test(ua),
            isSymbian = /(?:SymbianOS)/.test(ua) || isWindowsPhone,
            isAndroid = /(?:Android)/.test(ua),
            isFireFox = /(?:Firefox)/.test(ua),
            isChrome = /(?:Chrome|CriOS)/.test(ua),
            isTablet = /(?:iPad|PlayBook)/.test(ua) || (isAndroid && !/(?:Mobile)/.test(ua)) || (isFireFox && /(?:Tablet)/.test(ua)),
            isPhone = /(?:iPhone)/.test(ua) && !isTablet,
            isPc = !isPhone && !isAndroid && !isSymbian && !isWindowsPhone && !isTablet;
        return {
            isTablet: isTablet,
            isPhone: isPhone,
            isAndroid : isAndroid,
            isPc : isPc
        };
    }();
    if(os.isPc){
        //alert("PC");
        Webcam.set({
            width: 400,
            height: 300,
            image_format: 'jpeg',
            jpeg_quality: 90
        });
    }else {
        //alert("Mobile");
        Webcam.set({
            width: 300,
            height: 400,
            image_format: 'jpeg',
            jpeg_quality: 90
        });
    }
    Webcam.attach( '#my_camera' );
</script>
<!-- Code to handle taking the snapshot and displaying it locally -->
<script language="JavaScript">
    function take_snapshot() {
        Webcam.freeze();
        // take snapshot and get image data
        Webcam.snap( function(data_uri) {
            // display results in page
            // Webcam.upload(data_uri,"/bicycle/uploadServlet");
            //document.getElementById('results').innerHTML =
            //		'<h2>Here is your image:</h2>' +
            //		'<img src="'+data_uri+'"/>';
            var imgurl = document.getElementById('imgurl');
            //alert("获取到的value：" + imgurl.value+" ,"+$("#imgurl").val()); //获取
            imgurl.value = data_uri;
            //alert("设置后的value："+imgurl.value);

        } );
        Webcam.freeze();
        //alert("upload success");
        //window.location.href="/bicycle/personDetailServlet";
    }
    function freeze_cam() {
        Webcam.freeze();
    }
    function unfreeze_cam() {
        Webcam.unfreeze();
    }
</script>

</body>
</html>
