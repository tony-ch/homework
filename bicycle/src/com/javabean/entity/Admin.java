package com.javabean.entity;

public class Admin {
	private int id;//管理员编号
	private String name;//管理员真实姓名	
	private String loginname;//登录名
	private String password;//密码
	private String tel;//联系方式
	
	public Admin(){//空构造方法
	}
	
	public Admin(int id,String name,String loginname,String password,String tel){
		this.id = id;
		this.name = name;
		this.loginname = loginname;
		this.password = password;
		this.tel = tel;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getLoginname() {
		return loginname;
	}

	public void setLoginname(String loginname) {
		this.loginname = loginname;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getTel() {
		return tel;
	}

	public void setTel(String tel) {
		this.tel = tel;
	}
	
}
