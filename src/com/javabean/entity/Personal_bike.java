package com.javabean.entity;

import java.sql.Date;
import java.sql.Timestamp;

public class Personal_bike {
	private int id;
	private int user;
	private String tel;
	private Date start_time;
	private Date end_time;
	private int rent;
	private String desc;
	
	public Personal_bike(){
	}
	
	public Personal_bike(int id,int user,String tel,Date start_time,Date end_time,int rent,String desc){
		this.id=id;
		this.user=user;
		this.tel=tel;
		this.start_time=start_time;
		this.end_time=end_time;
		this.rent=rent;
		this.desc=desc;
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

	public String getTel() {
		return tel;
	}

	public void setTel(String tel) {
		this.tel = tel;
	}

	public Date getStart_time() {
		return start_time;
	}

	public void setStart_time(Date start_time) {
		this.start_time = start_time;
	}

	public Date getEnd_time() {
		return end_time;
	}

	public void setEnd_time(Date end_time) {
		this.end_time = end_time;
	}

	public int getRent() {
		return rent;
	}

	public void setRent(int rent) {
		this.rent = rent;
	}

	public String getDesc() {
		return desc;
	}

	public void setDesc(String desc) {
		this.desc = desc;
	}

}