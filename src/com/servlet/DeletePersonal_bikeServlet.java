package com.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.javabean.entity.*;
import com.javabean.dao.*;

/**
 * Servlet implementation class DeletePersonal_bikeServlet
 */
@WebServlet("/DeletePersonal_bikeServlet")

public class DeletePersonal_bikeServlet extends HttpServlet {				//删除个人自行车信息
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public DeletePersonal_bikeServlet() {
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
				request.getRequestDispatcher("/login.jsp").forward(request, response);
			}
			else{	//用户
				String _id = request.getParameter("id");	//这个id 是个人信息的编号
				int id = Integer.parseInt(_id);
				
				UserDao userDao = new UserDao();
				Personal_bikeDao personal_bikeDao = new Personal_bikeDao();
				User user = userDao.findUserByLoginName(person.getLoginName());
				
				Personal_bike personal_bike = personal_bikeDao.findPersonal_bikeById(id);
				if(personal_bike!=null && personal_bike.getUser()==user.getId()){	//信息存在，且这条信息是这个user发布的
					personal_bikeDao.delPersonal_bike(id);
					session.setAttribute("message", "个人自行车信息删除成功");
					request.getRequestDispatcher("/personal_bike.jsp").forward(request, response);		//跳转到个人信息发布页面（即刷新当前界面）
				}
				else{
					session.setAttribute("message", "对不起，您不能删除此条个人自行车信息");
					request.getRequestDispatcher("/personal_bike.jsp").forward(request, response);		//跳转到个人信息发布页面（即刷新当前界面）
				}
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("se", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}
	
}
	