package com.javabean.entity;

import java.sql.Timestamp;

public class Purchase {
	private int id;
	private int buyer;
	private int bikenum;
	private int money;
	private Timestamp time;
	
	public Purchase(){
	}
	
	public Purchase(int id,int buyer,int bikenum,int money,Timestamp time){
		this.id=id;
		this.buyer=buyer;
		this.bikenum=bikenum;
		this.money=money;
		this.time=time;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public int getBuyer() {
		return buyer;
	}

	public void setBuyer(int buyer) {
		this.buyer = buyer;
	}

	public int getBikenum() {
		return bikenum;
	}

	public void setBikenum(int bikenum) {
		this.bikenum = bikenum;
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
	
}