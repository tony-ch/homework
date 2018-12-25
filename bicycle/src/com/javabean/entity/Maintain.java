package com.javabean.entity;

import java.sql.Timestamp;

public class Maintain {
	private int id;
	private int user;
	private int bike;
	private int maintainer;
	private String place;
	private Timestamp time;
	
	public Maintain(){
	}
	
	public Maintain(int id,int user,int bike,int maintainer,String place,Timestamp time){
		this.id=id;
		this.user=user;
		this.bike=bike;
		this.maintainer=maintainer;
		this.place=place;
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

	public int getBike() {
		return bike;
	}

	public void setBike(int bike) {
		this.bike = bike;
	}

	public int getMaintainer() {
		return maintainer;
	}

	public void setMaintainer(int maintainer) {
		this.maintainer = maintainer;
	}

	public String getPlace() {
		return place;
	}

	public void setPlace(String place) {
		this.place = place;
	}

	public Timestamp getTime() {
		return time;
	}

	public void setTime(Timestamp time) {
		this.time = time;
	}
	

}