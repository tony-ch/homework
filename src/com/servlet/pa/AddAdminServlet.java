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
 * Servlet implementation class AddAdminServlet
 */
@WebServlet("/pa/addAdminServlet")
public class AddAdminServlet extends HttpServlet {				//人事管理 ——>添加管理员
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public AddAdminServlet() {
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
		String name=request.getParameter("name");
		String password=request.getParameter("password");
		String tel=request.getParameter("telnum");

		AdminDao adminDao = new AdminDao();
		
		try{
			if(adminDao.findAdminByLoginName(loginname)!=null){
				session.setAttribute("message", "登录名已被占用，请使用其他登录名");
				//request.setAttribute("activeTab", "adtab");
				request.getRequestDispatcher("/pa/listAdmServlet").forward(request, response);		
			}else{
				Admin admin=new Admin(0, name,loginname, password, tel);
				adminDao.addAdmin(admin);
		  		session.setAttribute("message", "管理员添加成功！");
				//request.setAttribute("activeTab", "adtab");
				request.getRequestDispatcher("/pa/listAdmServlet").forward(request, response);	
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("se", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
