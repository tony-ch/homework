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
 * Servlet implementation class DeleteAdminServlet
 */
@WebServlet("/pa/deleteAdminServlet")
public class DeleteAdminServlet extends HttpServlet {				//人事管理 ——>删除管理员
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public DeleteAdminServlet() {
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
		
		AdminDao adminDao = new AdminDao();
		Admin admin = adminDao.findAdminByLoginName(loginname);
		

		Person person = (Person)session.getAttribute("person");
		
		try{
			if(admin==null){
				session.setAttribute("message", "该管理员不存在，请重试");
				request.getRequestDispatcher("/pa/listAdminServlet").forward(request, response);	
			}else{
				if(person.getLoginname().equals(loginname)){
					session.setAttribute("message", "对不起，您不能删除自己");
					request.getRequestDispatcher("/pa/listAdminServlet").forward(request, response);	
				}
				else{
					adminDao.delAdmin(loginname);
					session.setAttribute("message", "管理员删除成功！");
					request.getRequestDispatcher("/pa/listAdminServlet").forward(request, response);	
				}
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
