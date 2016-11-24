package com.servlet;

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
import com.javabean.entity.Person;
import com.javabean.entity.User;
import com.javabean.entity.Worker;

/**
 * Servlet implementation class PersonDetailServlet
 */
@WebServlet("/personDetailServlet")
public class PersonDetailServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public PersonDetailServlet() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		doPost(request, response);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		HttpSession session=request.getSession();
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		
				
		try{
			Person person=(Person)session.getAttribute("person");
			String loginname=person.getLoginname();		//传参，确定改哪个admin的信息
			int type=person.getType();
			if(type==0){//admin
				AdminDao adminDao = new AdminDao();
				Admin admin = adminDao.findAdminByLoginName(loginname);				
		  		request.setAttribute("type", "admin");
				request.setAttribute("admin", admin);
				request.getRequestDispatcher("/account.jsp").forward(request, response);	
			}else if(type==2 || type==1){//worker
				WorkerDao workerDao=new WorkerDao();
				Worker worker = workerDao.findWorkerByLoginName(loginname);				
		  		request.setAttribute("type", "worker");
				request.setAttribute("worker", worker);
				request.getRequestDispatcher("/account.jsp").forward(request, response);	
			}else{
				UserDao userDao=new UserDao();
				User user = userDao.findUserByLoginName(loginname);				
		  		request.setAttribute("type", "user");
				request.setAttribute("user", user);
				request.getRequestDispatcher("/account.jsp").forward(request, response);
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
