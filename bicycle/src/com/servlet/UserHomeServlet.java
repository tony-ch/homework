package com.servlet;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.javabean.dao.BikeDao;
import com.javabean.dao.OrderDao;
import com.javabean.dao.UserDao;
import com.javabean.entity.Bike;
import com.javabean.entity.Order;
import com.javabean.entity.Person;
import com.javabean.entity.User;

/**
 * Servlet implementation class UserHomeServlet
 */
@WebServlet("/userHomeServlet")
public class UserHomeServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UserHomeServlet() {
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
		try {
			HttpSession session=request.getSession();
			Person person=(Person)session.getAttribute("person");
			String loginname=person.getLoginname();
			OrderDao orderDao=new OrderDao();
			UserDao userDao=new UserDao();
			User user=userDao.findUserByLoginName(loginname);
			Order order=orderDao.findOrderByUserId(user.getId());
			if(order!=null){
				request.setAttribute("order", order);
				Bike bike=new BikeDao().findBikeById(order.getBike());
				request.setAttribute("key", bike.getKey());
			}
			request.getRequestDispatcher("/user.jsp").forward(request, response);
		} catch (Exception e) {
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
