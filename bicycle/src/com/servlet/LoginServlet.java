package com.servlet;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.javabean.entity.*;
import com.common.*;
import com.javabean.dao.*;

/**
 * Servlet implementation class LoginServlet
 */
@WebServlet("/loginServlet")
public class LoginServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public LoginServlet() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doPost(request,response);
		
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		//response.getWriter().append("Served at: ").append(request.getContextPath());
		AdminDao adminDao = new AdminDao();
		WorkerDao workerDao = new WorkerDao();
		UserDao userDao = new UserDao();
		HttpSession session=request.getSession();//获得一个Session对象，用于存放一些提示信息返回到前台
		request.setCharacterEncoding("UTF-8");//设置请求的编码方式
		response.setCharacterEncoding("UTF-8");//设置响应的编码方式
		//通过request对象从前台登录页面表单获得登录名和密码数据
  		String loginname=request.getParameter("loginname");
  		String password=request.getParameter("password");
  		String type=request.getParameter("type");
  		Boolean successed=false;
  		try{
  			System.out.println(loginname);
  			System.out.println(password);
  			System.err.println("type: "+type);
	  		Person person=null;
	  		//根据登陆方法返回的值判断用户是哪种类型用户，并将学生、教师或者管理员对象转化为User对象并设置对应的登录名、真实姓名、用户类型属性值
	  		if(type.equalsIgnoreCase("admin")){
	  			Admin admin=adminDao.login(loginname,password);
	  			if(admin!=null){
	  				successed=true;
	  				person=new Person();
	  				person.setLoginname(admin.getLoginname());
	  				person.setName(admin.getName());
	  				person.setTel(admin.getTel());
	  				person.setType(0);
	  			}
	  		}else if(type.equalsIgnoreCase("worker")) {
	  			Worker worker=workerDao.login(loginname, password);
	  			if(worker!=null){
	  				successed=true;
	  				person=new Person();
	  				person.setLoginname(worker.getLoginname());
	  				person.setName(worker.getName());
	  				person.setTel("无");
	  				if(worker.getDepartment().equalsIgnoreCase("B")){
	  					person.setType(1);
	  				}else{
	  					person.setType(2);
	  				}
	  				
	  			}
	  		}else{
	  			User user=userDao.login(loginname, password);
	  			if(user!=null){
	  				successed=true;
	  				person=new Person();
	  				person.setLoginname(user.getLoginname());
	  				person.setName(user.getName());
	  				person.setTel(user.getTel());
	  				person.setType(3);
	  			}
	  		}
	  		if(successed){
	  			//如果登录成功将获得的User对象存在session对象中（十分重要，程序以后要使用）
				session.setAttribute("person",person);
				//如果登录成功跳转系统主页面index.jsp
				request.getRequestDispatcher("/index.jsp").forward(request, response);
			}else{
				//如果登录失败将登录失败提示信息放入session对象
				session.setAttribute("message","登录信息有误，请重新登录！！！");
				//如果登录失败跳转系统登录页面login.jsp，要求用户重新登录
				request.getRequestDispatcher("/login.jsp").forward(request, response);
			}
  		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
