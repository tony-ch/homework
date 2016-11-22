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
 * Servlet implementation class UsermaintaintServlet
 */
@WebServlet("/usermaintaintServlet")

public class UsermaintaintServlet extends HttpServlet {				//用户报修
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UsermaintaintServlet() {
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
				MaintainDao maintainDao = new MaintainDao();
				BikeDao bikeDao = new BikeDao();
				User user = userDao.findUserByLoginName(person.getLoginname());
				int userId=user.getId();
				String bike=request.getParameter("bike");
				String place=request.getParameter("place");
				int bikeId = Integer.parseInt(bike);
				if(bikeDao.findBikeById(bikeId)==null){
					session.setAttribute("message", "自行车编号错误，请重新输入");
					request.getRequestDispatcher("/usermaintain.jsp").forward(request, response);		//跳转到用户维修界面（即刷新当前界面）
				}
				else if(bikeDao.findBikeById(bikeId).getState()!="I"){
					session.setAttribute("message", "对不起，该车正在使用或者已经停用维修");
					request.getRequestDispatcher("/usermaintain.jsp").forward(request, response);		//跳转到用户维修界面（即刷新当前界面）
				}
				else{
					Maintain maintain = new Maintain(0, userId, bikeId, 0, place, null);
					maintainDao.addMaintain(maintain);		//申报维修单
					session.setAttribute("message", "报修成功");
					request.getRequestDispatcher("/index.jsp").forward(request, response);		//跳转到主页
				}
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("se", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}
	
}
		