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
@WebFilter("/*")
public class EncodeFilter implements Filter {
	public void destroy() {
		/*
		 * 包含初始化Filter时需要执行的代码，该代码执行一次
		 * */
	}
	public void doFilter(ServletRequest request, ServletResponse response,
			FilterChain chain) throws IOException, ServletException {
		HttpServletRequest req = (HttpServletRequest) request;//将request强制转换为HttpServletRequest类型
		HttpServletResponse res = (HttpServletResponse) response;//将response强制转换为HttpServletResponse类型
		//设置请求的编码方式为Java通用编码方式UTF-8
		request.setCharacterEncoding("UTF-8");
		req.setCharacterEncoding("UTF-8");
		//继续执行用户请求的操作
		chain.doFilter(request,response);
		//设置响应的编码方式为Java通用编码方式UTF-8
		res.setCharacterEncoding("UTF-8");
		response.setCharacterEncoding("UTF-8");
	}

	public void init(FilterConfig config) throws ServletException {
		/*
		 * 包含资源释放的代码，通常对init()中的初始化的资源执行收尾工作；
		 * */
	}
}
