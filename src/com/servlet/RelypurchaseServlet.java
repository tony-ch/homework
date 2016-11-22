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
 * Servlet implementation class RelypurchaseServlet
 */
@WebServlet("/relypurchaseServlet")

public class RelypurchaseServlet extends HttpServlet {				//admin批复采购单，传参decission，true or false
	private static final long serialVersionUID = 1L;
       
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
			if(person==null){
				session.setAttribute("message", "对不起，只有登陆后才能访问系统");
				request.getRequestDispatcher("/login.jsp").forward(request, response);
			}
			else if(person.getType()!=0){		//如果admin，拒绝访问
				session.setAttribute("message", "对不起，您没有权限访问该界面");
				request.getRequestDispatcher("/index.jsp").forward(request, response);
			}
			else{	//admin
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
						request.getRequestDispatcher("/purchase.jsp").forward(request, response);		//刷新本页
					}
					else{
						int bikenum = purchase.getBikenum();
						int purchaserId = purchase.getBuyer();
						String _decission = request.getParameter("decission");
						boolean decission = Boolean.parseBoolean(_decission);
						
						company_accountDao.addCompany_account(adminID, bikenum, purchaserId, decission);
						
						session.setAttribute("message", "采购单批复成功");
						request.getRequestDispatcher("/purchase.jsp").forward(request, response);		//刷新本页
						
					}
					
				}
				
			}
		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("se", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}
	
}
		