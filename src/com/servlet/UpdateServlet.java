package com.servlet;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.javabean.dao.AdminDao;
import com.javabean.dao.UserDao;
import com.javabean.dao.WorkerDao;
import com.javabean.entity.Admin;
import com.javabean.entity.Person;
import com.javabean.entity.User;
import com.javabean.entity.Worker;

/**
 * Servlet implementation class UpdateServlet
 */
@WebServlet("/updateServlet")
public class UpdateServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UpdateServlet() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		//response.getWriter().append("Served at: ").append(request.getContextPath());
		AdminDao adminDao = new AdminDao();
		WorkerDao workerDao = new WorkerDao();
		UserDao userDao = new UserDao();
		HttpSession session=request.getSession();//获得一个Session对象，用于存放一些提示信息返回到前台
		request.setCharacterEncoding("UTF-8");//设置请求的编码方式
		response.setCharacterEncoding("UTF-8");//设置响应的编码方式
		//通过request对象从前台登录页面表单获得登录名和密码数据
  		String loginname=request.getParameter("loginname");
  		String name=request.getParameter("name");
  		String password=request.getParameter("password");
  		String modifyFace = request.getParameter("modifyFace");
  		Object perObj=session.getAttribute("person");
  		Person person=(Person)perObj;
  		String tel=request.getParameter("tel");
  		Boolean successed=false;
  		int type=person.getType();
  		try{
  			System.out.println(loginname);
  			System.out.println(password);
  			System.err.println("type: "+type);

			String facepath =  getServletContext().getRealPath(getServletContext().getInitParameter("face.location"));
			String face_csv = facepath+"/"+person.getTypeStr()+"-"+person.getLoginname()+".csv";
			if(modifyFace.equalsIgnoreCase("delete")){
				Files.deleteIfExists(Paths.get(face_csv));
			}
	  		//根据登陆方法返回的值判断用户是哪种类型用户，并将学生、教师或者管理员对象转化为User对象并设置对应的登录名、真实姓名、用户类型属性值
	  		if(type==0){
	  			Admin admin=adminDao.findAdminByLoginName(loginname);
	  			if(admin!=null){
	  				admin.setName(name);
		  			admin.setPassword(password);
		  			admin.setTel(tel);
		  			adminDao.updateAdmin(admin);
	  				successed=true;
	  				person.setName(admin.getName());
	  				person.setTel(admin.getTel());
	  			}
	  		}else if(type==1 || type==2) {
	  			Worker worker=workerDao.findWorkerByLoginName(loginname);
	  			if(worker!=null){
	  				worker.setName(name);
	  				worker.setPassword(password);
	  				workerDao.updateWorker(worker);
	  				successed=true;
	  				person.setName(worker.getName());
	  				
	  			}
	  		}else{
	  			User user=userDao.findUserByLoginName(loginname);
	  			if(user!=null){
	  				user.setName(name);
	  				user.setPassword(password);
	  				user.setTel(tel);
	  				userDao.updateUserInformation(user);
	  				successed=true;
	  				person.setName(user.getName());
	  				person.setTel(user.getTel());
	  			}
	  		}
	  		if(successed){
	  			session.setAttribute("message","修改信息成功！！！");
			}else{
				session.setAttribute("message","修改信息失败");
			}
	  		//如果登录成功将获得的User对象存在session对象中（十分重要，程序以后要使用）
			session.setAttribute("person",person);
			//如果登录成功跳转系统主页面index.jsp
			request.getRequestDispatcher("/personDetailServlet").forward(request, response);
  		}catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("exception", e);
			request.getRequestDispatcher("/exception.jsp").forward(request, response);
		}
	}

}
