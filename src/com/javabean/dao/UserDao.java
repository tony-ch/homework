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

import com.common.ConnectionFactory;
import com.common.Page;
import com.common.RTException;
import com.common.ResourceClose;
import com.javabean.entity.Admin;
import com.javabean.entity.User;
public class UserDao{
	//添加用户方法
	public void addUser(User user){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="insert into user (login_name,password,name,tel) values(?,?,?,?)"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setString(1, user.getLoginname());
			pstmt.setString(2, user.getPassword());
			pstmt.setString(3, user.getName());
			pstmt.setString(4, user.getTel());
			pstmt.executeUpdate();
		}catch(Exception e){
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//删除用户方法
	public void delUser(String loginName){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from user where login_name=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setString(1, loginName);
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//修改用户(身份)信息
	public void updateUserInformation(User user){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="update user set name=?,password=?,tel=? where login_name=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setString(1, user.getName());
			pstmt.setString(2, user.getPassword());
			pstmt.setString(3, user.getTel());
			pstmt.setString(4, user.getLoginname());
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	
	//修改用户(账户)信息
	public void updateUserBalance(User user,double balance){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="update user set balance=? where login_name=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setDouble(1, balance);
			pstmt.setString(2, user.getLoginname());
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	
	
	//根据登录名查找用户
	public User findUserByLoginName(String loginName){
		User user=null;
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from user where login_name=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setString(1, loginName);
			rs=pstmt.executeQuery();
			while(rs.next()){
				user=new User();
				user.setId(rs.getInt(1));
				user.setLoginname(rs.getString(2));
				user.setPassword(rs.getString(3));
				user.setBalance(rs.getDouble(4));
				user.setName(rs.getString(5));
				user.setTel(rs.getString(6));
			}
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
		return user;
	}
	//列表显示所有用户列表
	public Map findAllUser(int curPage){
		User user=null;
		ArrayList list=new ArrayList();
		Connection conn=null;
		Statement pstmt=null;
		ResultSet rs=null;
		ResultSet r=null;
		Map map=null;
		Page pa=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from user order by id"; 
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
					user=new User();
					user.setId(rs.getInt(1));
					user.setLoginname(rs.getString(2));
					user.setPassword(rs.getString(3));
					user.setBalance(rs.getDouble(4));
					user.setName(rs.getString(5));
					user.setTel(rs.getString(6));
					list.add(user);
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
	
	public Map findAllUserByMostCon(String id,String loginName,String name,String password,int curPage){
		User user=null;
		ArrayList list=new ArrayList();
		Connection conn=null;
		Statement pstmt=null;
		ResultSet rs=null;
		ResultSet r=null;
		Map map=null;
		Page pa=null;
		//构造多条件查询的SQL语句
		String sql="select * from user where 1=1 ";
		//模糊查询
		if(id!=null&&!id.equals("")){
			sql+=" and id like '%"+id+"%'";
		}
		if(loginName!=null&&!loginName.equals("")){
			sql+=" and login_name like '%"+loginName+"%'";
		}
		if(name!=null&&!name.equals("")){
			sql+=" and name like '%"+name+"%'";
		}
		if(password!=null&&!password.equals("")){
			sql+=" and password like '%"+password+"%'";
		}
		sql+=" order by id";
		try{
			conn=ConnectionFactory.getConnection();
			pstmt=conn.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_READ_ONLY);
			rs=pstmt.executeQuery(sql);
			pa=new Page();//声明分页类对象
			pa.setPageSize(5);
			pa.setPageCount(rs);
			pa.setCurPage(curPage);
			r=pa.setRs(rs);
			r.previous();
			for(int i=0;i<pa.getPageSize();i++){
				if(rs.next()){
					user=new User();
					user.setId(rs.getInt(1));
					user.setLoginname(rs.getString(2));
					user.setPassword(rs.getString(3));
					user.setBalance(rs.getDouble(4));
					user.setName(rs.getString(5));
					user.setTel(rs.getString(6));
					list.add(user);
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
	//管理员登录验证方法
	public User login(String loginName,String password){
		User user=null;
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from user where login_name=? and password=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setString(1, loginName);
			pstmt.setString(2, password);
			rs=pstmt.executeQuery();
			while(rs.next()){
				user=new User();
				user.setId(rs.getInt(1));
				user.setLoginname(rs.getString(2));
				user.setPassword(rs.getString(3));
				user.setBalance(rs.getDouble(4));
				user.setName(rs.getString(5));
				user.setTel(rs.getString(6));
			}
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
		return user;
	}
}
