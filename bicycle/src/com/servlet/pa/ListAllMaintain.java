package com.servlet.pa;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.common.Page;
import com.javabean.dao.MaintainDao;
import com.javabean.dao.WorkerDao;
import com.javabean.entity.Person;
import com.javabean.entity.Worker;

/**
 * Servlet implementation class ListAllMaintain
 */
@WebServlet("/pa/listAllMaintainServlet")
public class ListAllMaintain extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public ListAllMaintain() {
        super();
        // TODO Auto-generated constructor stub
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
		MaintainDao maintainDao = new MaintainDao();
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		int curPage=1;
		String temp=request.getParameter("curPage");
		if(temp!=null){
			curPage=Integer.parseInt(request.getParameter("curPage"));
		}
		try{
			Map map=maintainDao.findAllMaintain(curPage);
			ArrayList list=(ArrayList) map.get("list");
			Page pa=(Page) map.get("pa");
	  		request.setAttribute("curPage", pa.getCurPage());//向显示页传递当前页页码
	  		request.setAttribute("pageCount",pa.getPageCount());//向显示页传递总页数
	  		request.setAttribute("matlist", list);//向显示页传递结果集
	  		request.setAttribute("activeTab", "mattab");//adtab wktab ustab
	  		request.getRequestDispatcher("/manager.jsp").forward(request, response);
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
