package com.servlet.pa;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.javabean.dao.*;
import com.javabean.entity.*;

//传参 loginname，确定改哪个的信息

/**
 * Servlet implementation class ModifyUserServlet
 */
@WebServlet("/ModifyUserServlet")
public class ModifyUserServlet extends HttpServlet {				//人事管理 ——>修改用户
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public ModifyUserServlet() {
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
		
		HttpSession session=request.getSession();
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		
		String loginname=request.getParameter("loginname");		//传参，确定改哪个user的信息
		String password=request.getParameter("password");
		String name=request.getParameter("name");
		String tel=request.getParameter("tel");
		
		UserDao userDao = new UserDao();
		User user = userDao.findUserByLoginName(loginname);
		
		try{
			if(user==null){
				session.setAttribute("message", "该用户不存在，请重试");
				request.setAttribute("activeTab", "ustab");
				request.getRequestDispatcher("/manager.jsp").forward(request, response);		
			}else{
				User newUser = new User(0, loginname, password, 0.0, name, tel);
				userDao.updateUserInformation(newUser);
								
		  		session.setAttribute("message", "用户信息修改成功！");
				request.setAttribute("activeTab", "ustab");
				request.getRequestDispatcher("/manager.jsp").forward(request, response);	
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("se", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
