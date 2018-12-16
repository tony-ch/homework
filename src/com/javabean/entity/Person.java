package com.javabean.entity;

public class Person {
	private String loginname;//用户登录名
	private String name;//用户真实姓名
	private String tel;
	private int type;//用户类别，管理员类别为0，采购为1,维修为2,用户为3
	//无参构造方法
	public Person() {
		super();
	}
	//有参构造方法
	public Person(String loginname, String name, int type,String tel) {
		super();
		this.loginname = loginname;
		this.name = name;
		this.type = type;
		this.tel=tel;
	}
	//getter、setter方法
	public String getLoginname() {
		return loginname;
	}
	public void setLoginname(String loginname) {
		this.loginname = loginname;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getType() {
		return type;
	}
	public String getTypeStr(){
		if(this.type==0) return "admin";
		if(this.type==1 || this.type==2 ) return "worker";
		if(this.type==3) return "user";
		return "";
	}
	public void setType(int type) {
		this.type = type;
	}	
	
	public String getTel() {
		return tel;
	}
	public void setTel(String tel) {
		this.tel = tel;
	}	
}
