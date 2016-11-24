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
 * Servlet implementation class ModifyAdminServlet
 */
@WebServlet("/pa/modifyAdminServlet")
public class ModifyAdminServlet extends HttpServlet {				//人事管理 ——>修改管理员
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public ModifyAdminServlet() {
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
		
		String loginname=request.getParameter("loginname");		//传参，确定改哪个admin的信息
		String password=request.getParameter("password");
		String name=request.getParameter("name");
		String tel=request.getParameter("tel");
		
		
		
		try{
			AdminDao adminDao = new AdminDao();
			Admin admin = adminDao.findAdminByLoginName(loginname);
			if(admin==null){
				session.setAttribute("message", "该管理员不存在，请重试");
				request.getRequestDispatcher("/pa/listAdminServlet").forward(request, response);		
			}else{
				Admin newAdmin = new Admin(0, name, loginname, password, tel);
				adminDao.updateAdmin(newAdmin);
								
		  		session.setAttribute("message", "管理员信息修改成功！");
				request.getRequestDispatcher("/pa/listAdminServlet").forward(request, response);	
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
