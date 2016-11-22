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
 * Servlet implementation class SubmitpurchaseServlet
 */
@WebServlet("/submitpurchaseServlet")

public class SubmitpurchaseServlet extends HttpServlet {				//采购员提交采购单
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public SubmitpurchaseServlet() {
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
			else if(person.getType()!=1){		//如果不是采购员，拒绝访问
				session.setAttribute("message", "对不起，您没有权限访问该界面");
				request.getRequestDispatcher("/index.jsp").forward(request, response);
			}
			else{	//采购员
				PurchaseDao purchaseDao = new PurchaseDao();
				WorkerDao workerDao = new WorkerDao();
				
				String _bikenum=request.getParameter("bikenum");
				String _money=request.getParameter("money");
				
				int bikenum = Integer.parseInt(_bikenum);
				int money = Integer.parseInt(_money);
				String buyerLogiNname = person.getLoginname();
				int buyerId = workerDao.findWorkerByLoginName(buyerLogiNname).getId();		//已经登陆成功，则一定有此人ID
				
				Purchase purchase = new Purchase(0, buyerId, bikenum, money, null);
				purchaseDao.addPurchase(purchase);		//申报采购单
				session.setAttribute("message", "申报采购单成功");
				request.getRequestDispatcher("/index.jsp").forward(request, response);		//跳转到主页
				
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("se", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}
	
}
		