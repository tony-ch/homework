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
 * Servlet implementation class DeleteWorkerSevlet
 */
@WebServlet("/DeleteWorkerSevlet")
public class DeleteWorkerSevlet extends HttpServlet {				//人事管理 ——>删除员工
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public DeleteWorkerSevlet() {
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
		
		WorkerDao workerDao = new WorkerDao();
		Worker worker = workerDao.findWorkerByLoginName(loginname);
		
		try{
			if(worker==null){
				session.setAttribute("message", "该员工不存在，请重试");
				request.setAttribute("activeTab", "wktab");
				request.getRequestDispatcher("/manager.jsp").forward(request, response);		
			}else{
				workerDao.delWorker(loginname);
		  		session.setAttribute("message", "员工删除成功！");
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
