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
 * Servlet implementation class UsebikeServlet
 */
@WebServlet("/UsebikeServlet")

public class UsebikeServlet extends HttpServlet {				//用户用车
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UsebikeServlet() {
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
		BikeDao bikeDao = new BikeDao();
		
		HttpSession session=request.getSession();		//获得一个Session对象，用于获取和存放一些提示信息返回到前台
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		Person person = (Person)session.getAttribute("person");
		String bike=request.getParameter("bike");
		int bikeId = Integer.parseInt(bike);

		try{
			if(person==null){
				session.setAttribute("message", "对不起，只有登陆后才能访问系统");
				request.getRequestDispatcher("/login.jsp").forward(request, response);
			}
			else if(person.getType()!=3){
				session.setAttribute("message", "只有用户才可以使用自行车");
				request.getRequestDispatcher("/index.jsp").forward(request, response);		//返回主页
			}
			else if(bikeDao.findBikeById(bikeId)==null){
				session.setAttribute("message", "自行车编号错误，请重新输入");
				request.getRequestDispatcher("/usebike.jsp").forward(request, response);		
			}
			else if(bikeDao.findBikeById(bikeId).getState()==null){
				session.setAttribute("message", "该车已被占用或停用，请使用其他自行车");
				request.getRequestDispatcher("/usebike.jsp").forward(request, response);
			}else{
				UserDao userDao = new UserDao();
				OrderDao orderDao = new OrderDao();
				int userId = userDao.findUserByLoginName(person.getLoginName()).getId();
				Order order = new Order(0, userId, bikeId, null, null);
				orderDao.addOrder(order);
				String key = bikeDao.findBikeById(bikeId).getKey();				
		  		session.setAttribute("message", "用车成功，密码是！");
		  		request.getRequestDispatcher("/index.jsp").forward(request, response);
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("se", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}
	
	
}
