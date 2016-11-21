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
 * Servlet implementation class RelymaintainServlet
 */
@WebServlet("/RelymaintainServlet")
//维修工批复维修单，不用传参，不用
//准确说，不叫做批复，叫处理维修单，修改自行车状态
public class RelymaintainServlet extends HttpServlet {				
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public RelymaintainServlet() {
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
			else if(person.getType()!=0){		//如果不是维修工，拒绝访问
				session.setAttribute("message", "对不起，您没有权限访问该界面");
				request.getRequestDispatcher("/index.jsp").forward(request, response);
			}
			else{	//维修工
				MaintainDao maintainDao = new MaintainDao();
				WorkerDao workerDao = new WorkerDao();
				Worker maintainer = workerDao.findWorkerByLoginName(person.getLoginName());
				
				if(maintainer==null){
					session.setAttribute("message", "无效maintainer，请重新登录");
					request.getRequestDispatcher("/login.jsp").forward(request, response);		//跳转到主页
				}
				else{
					int maintainerId = maintainer.getId();
					String _id = request.getParameter("id");		//维修单编号
					int id = Integer.parseInt(_id);
					Maintain maintain = maintainDao.findMaintainById(id);

					if(maintain==null){
						session.setAttribute("message", "无效维修单编号，请重新确认");
						request.getRequestDispatcher("/purchase.jsp").forward(request, response);		//刷新本页
					}
					else{
						if(maintain.getMaintainer()!=maintainerId){		//（维修单指派维修工id）与（实际批复的维修工id）不同
							session.setAttribute("message", "对不起，您不能批复该维修单");
							request.getRequestDispatcher("/purchase.jsp").forward(request, response);		//刷新本页
						}
						else{
							session.setAttribute("message", "维修单批复成功");
							request.getRequestDispatcher("/purchase.jsp").forward(request, response);		//刷新本页
						}
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
		