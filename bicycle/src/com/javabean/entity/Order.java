package com.javabean.entity;

import java.sql.Timestamp;

public class Order {
	private int id;
	private int user;
	private int bike;
	private Timestamp start_time;
	private Timestamp end_time;
	
	public Order(){
	}
	
	public Order(int id,int user,int bike,Timestamp start_time,Timestamp end_time){
		this.id=id;
		this.user=user;
		this.bike=bike;
		this.start_time=start_time;
		this.end_time=end_time;
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

	public int getBike() {
		return bike;
	}

	public void setBike(int bike) {
		this.bike = bike;
	}

	public Timestamp getStart_time() {
		return start_time;
	}

	public void setStart_time(Timestamp start_time) {
		this.start_time = start_time;
	}

	public Timestamp getEnd_time() {
		return end_time;
	}

	public void setEnd_time(Timestamp end_time) {
		this.end_time = end_time;
	}
	
}
