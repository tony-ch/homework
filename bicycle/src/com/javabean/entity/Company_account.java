package com.javabean.entity;

import java.sql.Timestamp;

public class Company_account {
	private int id;
	private int money;
	private Timestamp time;
	private int admin;
	private int worker;
	private String type;		//"B","S",采购和工资
	
	public Company_account(){
	}

	public Company_account(int id,int money,Timestamp time,int admin,int worker,String type){
		this.id=id;
		this.money=money;
		this.time=time;
		this.admin=admin;
		this.worker=worker;
		this.type=type;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public int getMoney() {
		return money;
	}

	public void setMoney(int money) {
		this.money = money;
	}

	public Timestamp getTime() {
		return time;
	}

	public void setTime(Timestamp time) {
		this.time = time;
	}

	public int getAdmin() {
		return admin;
	}

	public void setAdmin(int admin) {
		this.admin = admin;
	}

	public int getWorker() {
		return worker;
	}

	public void setWorker(int worker) {
		this.worker = worker;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}
	
	
	
}
