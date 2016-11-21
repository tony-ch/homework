package com.commmon;

import java.sql.Connection;
import java.sql.DriverManager;

public class ConnectionFactory {
	/*
	 * 数据库连接需要的四大参数:driver代表操作Oracle数据库的驱动名称;url代表数据库的URL;
	 * userName代表连接数据库的用户名;password代表连接数据库的用户密码。
	 * 读者一定要注意参数值的拼写。
	 */	
	private static String driver="com.mysql.jdbc.Driver";
	private static String url="jdbc:mysql://localhost:3306/bicycle";
	//url格式:协议名+@+数据库所在主机IP地址+数据库端口号+数据库名字;中间使用":"隔开。
	private static String userName="root";
	private static String password="root";
	/*
	 *该方法用来获得数据库连接，其返回类型为Connection引用类型;
	 *定义为static方法可以不用实例化类（ConnectionFactory）对象直接通过类名使用该方法。	 
	 */
	synchronized public static Connection getConnection(){
		try{
			Class.forName(driver);//注册数据库驱动
			return DriverManager.getConnection(url,userName,password);
			//使用DriverManager的getConnection方法来获得连接，作为方法的返回值。
		}catch(Exception e){
			//如果程序出现异常，打印异常的栈信息，方法的返回值为null。
			e.printStackTrace();
			return null;
		}
	}
}
