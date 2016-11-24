package com.servlet;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.javabean.dao.*;
import com.javabean.entity.*;

//传参 loginname，确定改哪个的信息

/**
 * Servlet implementation class UseraccountServlet
 */
@WebServlet("/useraccountServlet")
public class UseraccountServlet extends HttpServlet { // 用户自行充值
	private static final long serialVersionUID = 1L;

	/**
	 * @see HttpServlet#HttpServlet()
	 */
	public UseraccountServlet() {
		super();
	}

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse
	 *      response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		// response.getWriter().append("Served at:
		// ").append(request.getContextPath());
		doPost(request, response);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse
	 *      response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {

		HttpSession session = request.getSession();
		request.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
		UserDao userDao = new UserDao();
		Person person = (Person) session.getAttribute("person");
		

		

		try {
			User user = userDao.findUserByLoginName(person.getLoginname());
			if (user == null) {
				session.setAttribute("message", "该用户不存在，请重试");
				request.getRequestDispatcher("/userHomeServlet").forward(request, response);
			} else {
				String _money = request.getParameter("money");
				double money = Double.parseDouble(_money);
				double balance = userDao.findUserByLoginName(user.getLoginname()).getBalance();
				//System.out.println("login name:"+user.getLoginname());
				//System.out.println("balance before:"+balance);
				balance +=money;
				userDao.updateUserBalance(user, balance);
				//System.out.println("balance after:"+userDao.findUserByLoginName(user.getLoginname()).getBalance());
				//System.out.println(money);

				session.setAttribute("message", "充值成功！");
				request.getRequestDispatcher("/userHomeServlet").forward(request, response);
			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
