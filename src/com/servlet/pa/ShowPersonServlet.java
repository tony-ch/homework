package com.servlet.pa;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.javabean.dao.AdminDao;
import com.javabean.dao.UserDao;
import com.javabean.dao.WorkerDao;
import com.javabean.entity.Admin;
import com.javabean.entity.User;
import com.javabean.entity.Worker;

/**
 * Servlet implementation class ShowPerson
 */
@WebServlet("/pa/showPersonServlet")
public class ShowPersonServlet extends HttpServlet {
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public ShowPersonServlet() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doPost(request, response);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		HttpSession session=request.getSession();
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		
		String loginname=request.getParameter("loginname");		//传参，确定改哪个admin的信息
		String type=request.getParameter("type");		
		try{
			if(type==null){
				session.setAttribute("message", "出现错误，请重试");
				request.setAttribute("activeTab", "adtab");
				request.getRequestDispatcher("/manager.jsp").forward(request, response);		
			}else if(type.equalsIgnoreCase("admin")){
				AdminDao adminDao = new AdminDao();
				Admin admin = adminDao.findAdminByLoginName(loginname);				
		  		request.setAttribute("type", "admin");
				request.setAttribute("admin", admin);
				request.getRequestDispatcher("/pa/modifyPerson.jsp").forward(request, response);	
			}else if(type.equalsIgnoreCase("worker")){
				WorkerDao workerDao=new WorkerDao();
				Worker worker = workerDao.findWorkerByLoginName(loginname);				
		  		request.setAttribute("type", "worker");
				request.setAttribute("worker", worker);
				request.getRequestDispatcher("/pa/modifyPerson.jsp").forward(request, response);	
			}else{
				UserDao userDao=new UserDao();
				User user = userDao.findUserByLoginName(loginname);				
		  		request.setAttribute("type", "user");
				request.setAttribute("user", user);
				request.getRequestDispatcher("/pa/modifyPerson.jsp").forward(request, response);
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("se", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
