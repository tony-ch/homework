package com.javabean.entity;

public class Worker {
	protected int id;//员工编号
	protected String loginname;//登录名
	protected String password;//密码
	protected String department;//部门 ，B采购员，M维修工
	protected String name;//员工真实姓名	
	protected int salary;//工资
	protected int cnt;	//维修次数
	protected int balance;//员工账户余额（体现发工资动作）
	
	public Worker(){//无参构造方法
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
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

	public String getDepartment() {
		return department;
	}

	public void setDepartment(String department) {
		this.department = department;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getSalary() {
		return salary;
	}

	public void setSalary(int salary) {
		this.salary = salary;
	}

	public int getCnt() {
		return cnt;
	}

	public void setCnt(int cnt) {
		this.cnt = cnt;
	}

	public int getBalance() {
		return balance;
	}

	public void setBalance(int balance) {
		this.balance = balance;
	}
	
}
