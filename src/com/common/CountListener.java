package com.common;
import javax.servlet.annotation.WebListener;
import javax.servlet.http.HttpSessionEvent;
import javax.servlet.http.HttpSessionListener;
@WebListener
public class CountListener implements HttpSessionListener{
	private static long linedNumber=0;//初始化在线人数变量
	//监听session创建的方法
	public void sessionCreated(HttpSessionEvent arg0) {
		linedNumber++;
	}
	////监听session销毁的方法
	public void sessionDestroyed(HttpSessionEvent arg0) {
		linedNumber--;
	}
	//返回在线人数
	public static long getLinedNumber(){
		return linedNumber;
	}
}
