package com.javabean.entity;

import java.sql.Timestamp;

public class User_account {
	private int id;
	private int user;
	private double money;
	private Timestamp time;
	
	public User_account(){
	}
	
	public User_account(int id,int user,double money,Timestamp time){
		this.id=id;
		this.user=user;
		this.money=money;
		this.time=time;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public int getUser() {
		return user;
	}

	public void setUser(int user) {
		this.user = user;
	}

	public double getMoney() {
		return money;
	}

	public void setMoney(double money) {
		this.money = money;
	}

	public Timestamp getTime() {
		return time;
	}

	public void setTime(Timestamp time) {
		this.time = time;
	}
	
}