package com.servlet;

import java.io.IOException;
import java.sql.Date;
import java.sql.Timestamp;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.javabean.entity.*;
import com.javabean.dao.*;

/**
 * Servlet implementation class personal_bikeServlet
 */
@WebServlet("/personal_bikeServlet")

public class Personal_bikeServlet extends HttpServlet {				//发布个人自行车信息
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Personal_bikeServlet() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doPost(request,response);
		
	}
	
	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		HttpSession session=request.getSession();		//获得一个Session对象，用于获取和存放一些提示信息返回到前台
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		Person person = (Person)session.getAttribute("person");

		try{
			if(person==null){
				session.setAttribute("message", "对不起，只有登陆后才能访问系统");
				request.getRequestDispatcher("/login.jsp").forward(request, response);
			}
			else if(person.getType()!=3){
				session.setAttribute("message", "对不起，只有用户才能访问该界面");
				request.getRequestDispatcher("/index.jsp").forward(request, response);
			}
			else{	//用户
				UserDao userDao = new UserDao();
				Personal_bikeDao personal_bikeDao = new Personal_bikeDao();
				User user = userDao.findUserByLoginName(person.getLoginname());
				String tel=request.getParameter("tel");
				String _start=request.getParameter("start_time");
				String _end=request.getParameter("end_time");
				String _rent=request.getParameter("rent");
				String desc=request.getParameter("desc");
				
				
				int userId=user.getId();
				Date start_time = Date.valueOf(_start);
				Date end_time = Date.valueOf(_end);
				int rent = Integer.parseInt(_rent);
				
				System.out.println(start_time);
				System.out.println(end_time);
				
				Personal_bike personal_bike = new Personal_bike(0, userId, tel, start_time, end_time, rent, desc);
				personal_bikeDao.addPersonal_bike(personal_bike);
				
				/*System.out.println(personal_bike.getId());
				System.out.println(personal_bike.getUser());
				System.out.println(personal_bike.getTel());
				System.out.println(personal_bike.getStart_time());
				System.out.println(personal_bike.getEnd_time());
				System.out.println(personal_bike.getRent());
				System.out.println(personal_bike.getDesc());*/
				
				
				session.setAttribute("message", "个人自行车信息发布成功");
				request.getRequestDispatcher("/userHomeServlet").forward(request, response);		//跳转到个人信息发布页面（即刷新当前界面）
				
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}
	
}
	