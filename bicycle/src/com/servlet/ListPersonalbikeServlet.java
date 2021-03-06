package com.servlet;
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
import com.javabean.dao.Personal_bikeDao;
import com.javabean.dao.UserDao;
import com.javabean.entity.Person;
import com.javabean.entity.User;
@WebServlet("/listpersonalbikeServlet")
public class ListPersonalbikeServlet extends HttpServlet {

	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		doPost(request, response);
	}
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		
		Personal_bikeDao personal_bikeDao = new Personal_bikeDao();
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		int curPage=1;
		String temp=request.getParameter("curPage");
		if(temp!=null){
			curPage=Integer.parseInt(request.getParameter("curPage"));
		}
		try{
			HttpSession session=request.getSession();
			Person person=(Person)session.getAttribute("person");
			User user=new UserDao().findUserByLoginName(person.getLoginname());
			request.setAttribute("uid", user.getId());
			
			Map map=personal_bikeDao.findAllPersonal_bike(curPage);
			ArrayList list=(ArrayList) map.get("list");
			Page pa=(Page) map.get("pa");
	  		request.setAttribute("curPage", pa.getCurPage());//向显示页传递当前页页码
	  		request.setAttribute("pageCount",pa.getPageCount());//向显示页传递总页数
	  		request.setAttribute("pblist", list);//向显示页传递结果集
	  		request.setAttribute("activeTab", "pbtab");//PersonalBike tab
	  		request.getRequestDispatcher("/user.jsp").forward(request, response);
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}
}
