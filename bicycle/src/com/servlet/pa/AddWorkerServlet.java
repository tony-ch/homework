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
 * Servlet implementation class AddWorkerServlet
 */
@WebServlet("/pa/addWorkerServlet")
public class AddWorkerServlet extends HttpServlet {				//人事管理 ——>添加员工
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public AddWorkerServlet() {
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
		//String tel=request.getParameter("tel");
		String department = request.getParameter("department");

		WorkerDao workerDao = new WorkerDao();
		
		try{
			if(workerDao.findWorkerByLoginName(loginname)!=null){
				session.setAttribute("message", "登录名已被占用，请使用其他登录名");
				request.getRequestDispatcher("/pa/listWorkerServlet").forward(request, response);		
			}else{
				Worker worker=new Worker();
				worker.setLoginname(loginname);
				worker.setPassword(password);
				worker.setDepartment(department);
				worker.setName(name);
				
				workerDao.addWorker(worker);
		  		session.setAttribute("message", "员工添加成功！");
				//request.setAttribute("activeTab", "wktab");
				request.getRequestDispatcher("/pa/listWorkerServlet").forward(request, response);	
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
