package com.javabean.dao;
import java.sql.Connection;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import com.commmon.ConnectionFactory;
import com.commmon.RTException;
import com.commmon.ResourceClose;
import com.javabean.entity.Order;
public class OrderDao{
	//添加订单方法
	public void addOrder(Order order){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="insert into order (user,bike) values(?,?)"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, order.getUser());
			pstmt.setInt(2,order.getBike() );
			/*pstmt.setTimestamp(3, order.getStart_time());
			pstmt.setTimestamp(4,order.getEnd_time() );*/
			pstmt.executeUpdate();
		}catch(Exception e){
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//根据用户id删除订单方法（不允许一个人同时租多辆车，所以根据用户id删除）
	public void delOrderByUser(int user){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from order where user=?"; 
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
	//根据自行车id删除订单方法
	public void delOrderByBike(int bike){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from order where bike=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, bike);
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//根据订单id删除订单方法
	public void delOrderByOrderId(int id){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from order where id=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, id);
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//根据用户id、车的id、起始时间、终止时间删除订单方法（用户自行删除订单记录时候用）
		public void delOrderByUserAndBike(int user,int bike,Date start_time,Date end_time){
			Connection conn=null;
			PreparedStatement pstmt=null;
			ResultSet rs=null;
			try{
				conn=ConnectionFactory.getConnection();
				String sql="delete from order where user=? and bike=? and start_time=? and end_time=?"; 
				pstmt=conn.prepareStatement(sql);
				pstmt.setInt(1, user);
				pstmt.setInt(2, bike);
				pstmt.setDate(3, start_time);
				pstmt.setDate(4, end_time);
				pstmt.executeUpdate();
			}catch (SQLException e) {
				e.printStackTrace();
				throw new RTException("数据库操作异常，请稍后重试!");
			}finally{
				ResourceClose.close(rs, pstmt, conn);
			}
		}
	//修改订单信息（根据用户id，车id，起始时间）
	public void updateOrder(Order order){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="update order set end_time=? where user=? and bike=? and start_time=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setTimestamp(1, order.getEnd_time());
			pstmt.setInt(2, order.getUser());
			pstmt.setInt(3, order.getBike());
			pstmt.setTimestamp(4, order.getStart_time());
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//查找订单（根据订单id）
	public Order findOrderByOrderId(int id){
		Order order=null;
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from order where id=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, id);;
			rs=pstmt.executeQuery();
			while(rs.next()){
				order=new Order();
				order.setId(rs.getInt(1));
				order.setUser(rs.getInt(2));
				order.setBike(rs.getInt(3));
				order.setStart_time(rs.getTimestamp(4));
				order.setEnd_time(rs.getTimestamp(5));
			}
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
		return order;
	}
	//查找订单（根据用户id）
	public Order findOrderByUserId(int user){
		Order order=null;
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from order where user=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, user);;
			rs=pstmt.executeQuery();
			while(rs.next()){
				order=new Order();
				order.setId(rs.getInt(1));
				order.setUser(rs.getInt(2));
				order.setBike(rs.getInt(3));
				order.setStart_time(rs.getTimestamp(4));
				order.setEnd_time(rs.getTimestamp(5));
			}
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
		return order;
	}
}
