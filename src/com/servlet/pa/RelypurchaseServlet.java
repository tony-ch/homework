package com.servlet.pa;

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
 * Servlet implementation class RelypurchaseServlet
 */
@WebServlet("/pa/relyPurchaseServlet")

public class RelypurchaseServlet extends HttpServlet {				//admin批复采购单，传参decission，true or false
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public RelypurchaseServlet() {
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
			PurchaseDao purchaseDao = new PurchaseDao();
			Company_accountDao company_accountDao = new Company_accountDao();
			AdminDao adminDao = new AdminDao();
			
			Admin admin = adminDao.findAdminByLoginName(person.getLoginname());
			
			if(admin==null){
				session.setAttribute("message", "无效admin，请重新登录");
				request.getRequestDispatcher("/login.jsp").forward(request, response);		//跳转到主页
			}
			else{
				int adminID = admin.getId();
				String _id = request.getParameter("id");		//采购单编号
				int id = Integer.parseInt(_id);
				Purchase purchase = purchaseDao.findPurchaseById(id);
				if(purchase==null){
					session.setAttribute("message", "无效采购单编号，请重新确认");
					request.getRequestDispatcher("/pa/listPurchaseServlet").forward(request, response);		//刷新本页
				}
				else{
					int bikenum = purchase.getBikenum();
					int purchaserId = purchase.getBuyer();
					String _decision = request.getParameter("decision");
					boolean decision = _decision.equalsIgnoreCase("true");
					
					System.out.println("rely :decision:"+decision+"bikenum:"+bikenum+"purchaseId:"+purchaserId+" adminID:"+adminID);
					company_accountDao.addCompany_account(adminID, bikenum, purchase.getId(), decision);
					
					session.setAttribute("message", "采购单批复成功");
					request.getRequestDispatcher("/pa/listPurchaseServlet").forward(request, response);		//刷新本页
					
				}
				
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}
	
}
		