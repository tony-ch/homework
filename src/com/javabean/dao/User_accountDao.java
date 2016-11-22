package com.javabean.dao;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.commmon.ConnectionFactory;
import com.commmon.RTException;
import com.commmon.Page;
import com.commmon.ResourceClose;
import com.javabean.entity.User_account;
public class User_accountDao{
	//添加记录方法
	public void addUser_account(User_account user_account){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="insert into user_account (user,money,time) values(?,?,?)"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, user_account.getUser());
			pstmt.setDouble(2, user_account.getMoney());
			pstmt.setTimestamp(3, user_account.getTime());
			pstmt.executeUpdate();
		}catch(Exception e){
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//(根据用户id)删除记录方法
	public void delUser_accountByUser(int user){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from user_account where user=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, user);
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	/*
	//修改记录信息
	public void updateUser_account(User_account user_account){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="update user_account set user=?,money=?,time=? where user=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, user_account.getUser());
			pstmt.setDouble(2, user_account.getMoney());
			pstmt.setDate(3, user_account.getTime());
			pstmt.setInt(4, user_account.getUser());
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new DaoException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}*/
	//根据用户id查找记录
	public User_account findUser_accountByUserId(int id){
		User_account user_account=null;
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from user_account where id=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, id);
			rs=pstmt.executeQuery();
			while(rs.next()){
				user_account=new User_account();
				user_account.setId(rs.getInt(1));
				user_account.setUser(rs.getInt(2));
				user_account.setMoney(rs.getDouble(3));
				user_account.setTime(rs.getTimestamp(4));
			}
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
		return user_account;
	}
	//列表显示所有记录列表
	public Map findUser_account(int curPage){
		User_account user_account=null;
		ArrayList list=new ArrayList();
		Connection conn=null;
		Statement pstmt=null;
		ResultSet rs=null;
		ResultSet r=null;
		Map map=null;
		Page pa=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from user_account order by id"; 
			pstmt=conn.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_READ_ONLY);
			rs=pstmt.executeQuery(sql);
			pa=new Page();//声明分页类对象
			pa.setPageSize(5);
			pa.setPageCount(rs);
			pa.setCurPage(curPage);
			r=pa.setRs(rs);
			r.previous();
			for(int i=0;i<pa.getPageSize();i++){
				if(r.next()){
					user_account=new User_account();
					user_account.setId(rs.getInt(1));
					user_account.setUser(rs.getInt(2));
					user_account.setMoney(rs.getDouble(3));
					user_account.setTime(rs.getTimestamp(4));
					list.add(user_account);
				}else{
					break;
				}
			}
			map=new HashMap();
			map.put("list",list);
			map.put("pa",pa);
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
			ResourceClose.close(r, null, null);
		}
		return map;
	}
}
