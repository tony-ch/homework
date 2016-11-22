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
 * Servlet implementation class ModifyWorkerServlet
 */
@WebServlet("/pa/modifyWorkerServlet")
public class ModifyWorkerServlet extends HttpServlet {				//人事管理 ——>修改员工
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public ModifyWorkerServlet() {
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
		
		String loginname=request.getParameter("loginname");		//传参，确定改哪个员工的信息
		String password=request.getParameter("password");
		String name=request.getParameter("name");
		String _salary=request.getParameter("salary");
		double salary=Double.parseDouble(_salary);
		//String tel=request.getParameter("tel");			//员工没有电话属性
		
		WorkerDao workerDao = new WorkerDao();
		Worker worker = workerDao.findWorkerByLoginName(loginname);
		
		try{
			if(worker==null){
				session.setAttribute("message", "该员工不存在，请重试");
				request.setAttribute("activeTab", "wktab");
				request.getRequestDispatcher("/manager.jsp").forward(request, response);		
			}else{
				Worker newWorker = new Worker();//(0, name, loginname, password, tel);
				newWorker.setPassword(password);
				newWorker.setName(name);
				newWorker.setSalary(salary);
				
				workerDao.updateWorker(newWorker);
								
		  		session.setAttribute("message", "员工信息修改成功！");
				request.setAttribute("activeTab", "wktab");
				request.getRequestDispatcher("/manager.jsp").forward(request, response);	
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("se", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
