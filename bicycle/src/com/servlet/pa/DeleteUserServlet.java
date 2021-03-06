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

/**
 * Servlet implementation class DeleteUserServlet
 */
@WebServlet("/pa/deleteUserServlet")
public class DeleteUserServlet extends HttpServlet {				//人事管理 ——>删除用户
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public DeleteUserServlet() {
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
		
		String loginname=request.getParameter("loginname");
		
		UserDao userDao = new UserDao();
		User user = userDao.findUserByLoginName(loginname);
		
		try{
			if(user==null){
				session.setAttribute("message", "该用户不存在，请重试");
				request.getRequestDispatcher("/pa/listUserServlet").forward(request, response);		
			}else{
				userDao.delUser(loginname);
		  		session.setAttribute("message", "用户删除成功！");
				//request.setAttribute("activeTab", "ustab");
				request.getRequestDispatcher("/pa/listUserServlet").forward(request, response);		
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
