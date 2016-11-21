package com.commmon;

import java.sql.ResultSet;
import java.sql.SQLException;

/**
 *@author: Lilx
 *@date: Feb 20, 2011
 *@company: cstd
 *@Email:llxhappy@126.com
 */
public class Page {
	private int pageSize;//每页显示的记录数
	private int pageCount;//总页数
	private int curPage;//当前页
	public void setPageSize(int size){
	    pageSize=size;//设置每页显示几条记录
	}
	public int getPageSize(){
	    return pageSize;//得到每页显示的记录数
	}
	//计算总页数
	public void setPageCount(ResultSet rs){
		int lastrow;
		try{
			rs.last();
			lastrow=rs.getRow();
		    if(lastrow % pageSize==0){
		    	pageCount=lastrow/pageSize;
		    }else{
		    	pageCount=lastrow/pageSize+1;
		    }
		}catch(SQLException e) {
			e.printStackTrace();
		}
	}
	//取得总页数
	public int getPageCount(){
	  return pageCount;
	}
	//根据页数设置结果集
	public ResultSet setRs(ResultSet rs){
		try{
			rs.absolute((curPage-1)*pageSize+1);
		}catch(SQLException e){
			e.printStackTrace();
		}
		return rs;
	}
	//设置要显示的页
	public void setCurPage(int row){
		if (row<=1)
			curPage=1;
		else if(row>=pageCount){
			curPage=pageCount;
		}else{
			curPage=row;
		}
	}
	//取得要显示的页
	public int getCurPage(){
		return curPage;
	}
}


