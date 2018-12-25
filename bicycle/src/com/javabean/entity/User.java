package com.javabean.entity;

public class User {
	private int id;//用户编号
	private String name;//用户真实姓名
	private String loginname;//用户登录名
	private String password;//用户登录密码
	private String tel;//联系方式
	private double balance;//账户余额

	//无参构造方法
	public User() {
	}
	//有参构造方法
	public User(int id,String loginname , String password,double balance,String name,String tel) {
		this.id = id;
		this.loginname = loginname;
		this.name = name;
		this.password = password;
		this.tel = tel;
		this.balance = balance;
	}
	public boolean depositMoney(double money){		//存钱
		if(money<=0)
			return false;
		this.balance+=money;
		return true;
	}
	public boolean drawMoney(double money){		//取钱
		if(money<=0)
			return false;
		if(this.balance<money){			//金额不足仍扣款，记账户余额为负数
			this.balance-=money;
			return false;
		}
		this.balance-=money;
		return true;
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
	public double getBalance() {
		return balance;
	}
	public void setBalance(double balance) {
		this.balance = balance;
	}
	
	
	
}
