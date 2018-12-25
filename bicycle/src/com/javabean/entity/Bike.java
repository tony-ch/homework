package com.javabean.entity;

public class Bike {
	private int id;
	private String key;
	private String state;
	public Bike(){
	}
	
	public Bike(int id,String key,String state){
		this.id=id;
		this.key=key;
		this.state=state;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getKey() {
		return key;
	}

	public void setKey(String key) {
		this.key = key;
	}

	public String getState() {
		return state;
	}

	public void setState(String state) {
		this.state = state;
	}
	
}
