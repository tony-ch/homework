package com.servlet;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.javabean.dao.UserDao;
import com.javabean.entity.User;

/**
 * Servlet implementation class RegisterServlet
 */
@WebServlet("/regServlet")
public class RegisterServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public RegisterServlet() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		//response.getWriter().append("Served at: ").append(request.getContextPath());
		doPost(request,response);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		HttpSession session=request.getSession();
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		//String uid=request.getParameter("id");
		String loginname=request.getParameter("loginname");
		String name=request.getParameter("username");
		String password=request.getParameter("password");
		String tel=request.getParameter("telnum");
		UserDao userDao=new UserDao();
		try{
			if(userDao.findUserByLoginName(loginname)!=null){
				session.setAttribute("message", "登录名已被占用，请使用其他登录名");
				request.getRequestDispatcher("/register.jsp").forward(request, response);
			}else{
				User user=new User(0, loginname, password, 0, name, tel);
				userDao.addUser(user);
		  		session.setAttribute("message", "注册成功，请登录！");
		  		request.getRequestDispatcher("/login.jsp").forward(request, response);
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
