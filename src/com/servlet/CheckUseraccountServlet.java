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
import com.common.RTException;
import com.javabean.dao.User_accountDao;
import com.javabean.dao.UserDao;
import com.javabean.entity.Person;
import com.javabean.entity.User;

@WebServlet("/checkUserAccountServlet")
public class CheckUseraccountServlet extends HttpServlet {
	public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doPost(request, response);
	}

	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		//声明Service对象
		User_accountDao service = new User_accountDao();
		UserDao userDao = new UserDao();
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		String loginName=request.getParameter("loginName");
		String name=request.getParameter("name");
		String password=request.getParameter("password");
		request.setAttribute("loginName", loginName);
		request.setAttribute("name", name);
		request.setAttribute("password", password);
		int curPage=1;
		
		int userId=-1;
		String loginname=null;
		String temp=request.getParameter("curPage");
		HttpSession session=request.getSession();		//获得一个Session对象，用于获取和存放一些提示信息返回到前台
		Person person = (Person)session.getAttribute("person");
		if(person!=null){
			loginname = person.getLoginname();
		}
		if(temp!=null){
			curPage=Integer.parseInt(request.getParameter("curPage"));
		}
		if(loginname!=null){
			User user = userDao.findUserByLoginName(loginname);
			if(user!=null){
				userId=user.getId();
				
			}
		}
  		try{
			Map map = service.findUser_account(1,userId);
			ArrayList list=(ArrayList) map.get("list");
			Page pa=(Page) map.get("pa");
			request.setAttribute("curPage", pa.getCurPage());//向显示页传递当前页页码
			request.setAttribute("pageCount",pa.getPageCount());//向显示页传递总页数
			request.setAttribute("uaist", list);//向显示页传递结果集
			request.setAttribute("activeTab", "uatab");
			request.setAttribute("loginname", loginname);
	  		request.getRequestDispatcher("/listuseraccountServlet").forward(request, response);
  		}catch (RTException e) {
			e.printStackTrace();
			request.setAttribute("exceptiton", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}
}
