package com.servlet.pa;

import java.io.IOException;
import java.sql.Date;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.common.Page;
import com.common.RTException;
import com.javabean.dao.OrderDao;
import com.javabean.dao.UserDao;

/**
 * Servlet implementation class CheckOrderServlet
 */
@WebServlet("/pa/checkOrderServlet")
public class CheckOrderServlet extends HttpServlet {
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public CheckOrderServlet() {
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
		OrderDao orderDao = new OrderDao();
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		String _start_time=request.getParameter("start_time");
		String _end_time=request.getParameter("end_time");
		
		Date start_date=null;
		Date end_date=null;
		String start_t=null;
		String end_t=null;
		try {
			DateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
			if (_start_time!=null && !_start_time.equals("")) {
				start_date = Date.valueOf(_start_time);
				start_t=sdf.format(start_date);
			}
			if (_end_time!=null && !_end_time.equals("")) {
				end_date = Date.valueOf(_end_time);
				end_t=sdf.format(end_date);
			}
		} catch (Exception e) {
			e.printStackTrace();
			HttpSession session=request.getSession();
			session.setAttribute("message", "输入条件不正确");
			request.getRequestDispatcher("/pa/listOrderServlet").forward(request, response);
			return;
		}
		
		request.setAttribute("start_time", _start_time);
		request.setAttribute("end_time", _end_time);
		int curPage=1;
		String temp=request.getParameter("curPage");
		if(temp!=null){
			curPage=Integer.parseInt(request.getParameter("curPage"));
		}
  		try{
  			//System.out.println(start_t);
  			//System.out.println(end_t);
  			Map map = orderDao.findAllOrderByTime(start_t, end_t, curPage);
			ArrayList list=(ArrayList) map.get("list");
			Page pa=(Page) map.get("pa");
			request.setAttribute("curPage", pa.getCurPage());//向显示页传递当前页页码
			request.setAttribute("pageCount",pa.getPageCount());//向显示页传递总页数
			request.setAttribute("ordlist", list);//向显示页传递结果集
			request.setAttribute("activeTab", "ordtab");//adtab wktab ustab
	  		request.getRequestDispatcher("/manager.jsp").forward(request, response);
  		}catch (RTException e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
