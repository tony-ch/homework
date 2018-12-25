package com.filter;

import java.io.IOException;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.annotation.WebFilter;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.javabean.entity.Person;

/**
 * Servlet Filter implementation class MaintainerFilter
 */
@WebFilter({"/ma/*","/maintain.jsp"})
public class MaintainerFilter implements Filter {

    /**
     * Default constructor. 
     */
    public MaintainerFilter() {
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see Filter#destroy()
	 */
	public void destroy() {
		// TODO Auto-generated method stub
	}

	/**
	 * @see Filter#doFilter(ServletRequest, ServletResponse, FilterChain)
	 */
	public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
		// TODO Auto-generated method stub
		HttpServletRequest req = (HttpServletRequest) request;//将request强制转换为HttpServletRequest类型
		HttpServletResponse res = (HttpServletResponse) response;//将response强制转换为HttpServletResponse类型
		HttpSession session=req.getSession();//获得一个Session对象，用于存放一些提示信息返回到前台或者获取Session对象中的信息
		//从session中取出user对象（该对象在登录成功后放入session）
		Person person=(Person)session.getAttribute("person");
		//如果用户登录成功并且登录用户为维修工类型，将继续执行用户请求操作，否则返回越权操作或者未登录提示信息并跳转到系统首页或者登录页面
		if (person!=null&&person.getType()==2){
			chain.doFilter(req, res);//继续执行用户请求的操作		
		}else if(person!=null&&(person.getType()==1||person.getType()==0||person.getType()==3)){
			//登录成功但是登录用户不是管理员类型将返回越权操作提示信息并跳转到系统首页
			session.setAttribute("message","对不起，该功能仅对维修人员开放！");
			res.sendRedirect(req.getContextPath() + "/index.jsp");//跳转到系统首页
		}else{
			//未登录成功将返回用户没有登录提示信息并跳转到系统登录页面
			session.setAttribute("message","对不起，只有登录后才能访问系统！");
			res.sendRedirect(req.getContextPath() + "/login.jsp");
		}
	}

	/**
	 * @see Filter#init(FilterConfig)
	 */
	public void init(FilterConfig fConfig) throws ServletException {
		// TODO Auto-generated method stub
	}

}
