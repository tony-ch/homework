package com.servlet.pa;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.common.Page;
import com.common.RTException;
import com.javabean.dao.BikeDao;
import com.javabean.dao.OrderDao;

/**
 * Servlet implementation class CheckBike
 */
@WebServlet("/pa/checkBikeServlet")
public class CheckBike extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public CheckBike() {
        super();
        // TODO Auto-generated constructor stub
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
		BikeDao bikeDao = new BikeDao();
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		String id=request.getParameter("id");
		String state=request.getParameter("state");
		request.setAttribute("id", id);
		request.setAttribute("state", state);
		int curPage=1;
		String temp=request.getParameter("curPage");
		if(temp!=null){
			curPage=Integer.parseInt(request.getParameter("curPage"));
		}
  		try{
  			Map map = bikeDao.findAllBikeByMostCon(id, state, curPage);
			ArrayList list=(ArrayList) map.get("list");
			Page pa=(Page) map.get("pa");
			request.setAttribute("curPage", pa.getCurPage());//向显示页传递当前页页码
			request.setAttribute("pageCount",pa.getPageCount());//向显示页传递总页数
			request.setAttribute("bikelist", list);//向显示页传递结果集
			request.setAttribute("activeTab", "biketab");//adtab wktab ustab
	  		request.getRequestDispatcher("/manager.jsp").forward(request, response);
  		}catch (RTException e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
