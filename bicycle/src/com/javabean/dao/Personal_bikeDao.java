package com.javabean.dao;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import com.common.ConnectionFactory;
import com.common.Page;
import com.common.RTException;
import com.common.ResourceClose;
import com.javabean.entity.Personal_bike;
public class Personal_bikeDao{
	//添加个人自行车信息
	public void addPersonal_bike(Personal_bike personal_bike){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="insert into personal_bike (user,tel,start_time,end_time,rent,descp) values(?,?,?,?,?,?)"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, personal_bike.getUser());
			pstmt.setString(2, personal_bike.getTel());
			pstmt.setDate(3, personal_bike.getStart_time());
			pstmt.setDate(4, personal_bike.getEnd_time());
			pstmt.setInt(5, personal_bike.getRent());
			pstmt.setString(6, personal_bike.getDesc());
			pstmt.executeUpdate();
		}catch(Exception e){
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//删除个人自行车信息(根据personal_bike编号)
	public void delPersonal_bike(int id){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from personal_bike where id=?"; 
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
	//修改个人自行车信息
	public void updatePersonal_bike(Personal_bike personal_bike){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="update personal_bike set tel=?,start_time=?,end_time=? ,rent=?,descp=? where id=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setString(1, personal_bike.getTel());
			pstmt.setDate(2, personal_bike.getStart_time());
			pstmt.setDate(3, personal_bike.getEnd_time());
			pstmt.setInt(4, personal_bike.getRent());
			pstmt.setString(5, personal_bike.getDesc());
			pstmt.setInt(6, personal_bike.getId());
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//根据user查找个人自行车信息
	public Personal_bike findPersonal_bikeByUser(int user){
		Personal_bike personal_bike = null;
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from personal_bike where user=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, user);
			rs=pstmt.executeQuery();
			while(rs.next()){
				personal_bike=new Personal_bike();
				personal_bike.setId(rs.getInt(1));
				personal_bike.setUser(rs.getInt(2));
				personal_bike.setTel(rs.getString(3));
				personal_bike.setStart_time(rs.getDate(4));
				personal_bike.setEnd_time(rs.getDate(5));
				personal_bike.setRent(rs.getInt(6));
				personal_bike.setDesc(rs.getString(7));
			}
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
		return personal_bike;
	}
	//根据user查找个人自行车信息
		public Personal_bike findPersonal_bikeById(int id){
			Personal_bike personal_bike = null;
			Connection conn=null;
			PreparedStatement pstmt=null;
			ResultSet rs=null;
			try{
				conn=ConnectionFactory.getConnection();
				String sql="select * from personal_bike where id=?"; 
				pstmt=conn.prepareStatement(sql);
				pstmt.setInt(1, id);
				rs=pstmt.executeQuery();
				while(rs.next()){
					personal_bike=new Personal_bike();
					personal_bike.setId(rs.getInt(1));
					personal_bike.setUser(rs.getInt(2));
					personal_bike.setTel(rs.getString(3));
					personal_bike.setStart_time(rs.getDate(4));
					personal_bike.setEnd_time(rs.getDate(5));
					personal_bike.setRent(rs.getInt(6));
					personal_bike.setDesc(rs.getString(7));
				}
			}catch (SQLException e) {
				e.printStackTrace();
				throw new RTException("数据库操作异常，请稍后重试!");
			}finally{
				ResourceClose.close(rs, pstmt, conn);
			}
			return personal_bike;
		}
	//列表显示所有个人自行车信息列表
	public Map findAllPersonal_bike(int curPage){
		Personal_bike personal_bike=null;
		ArrayList list=new ArrayList();
		Connection conn=null;
		Statement pstmt=null;
		ResultSet rs=null;
		ResultSet r=null;
		Map map=null;
		Page pa=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from personal_bike order by id"; 
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
					personal_bike=new Personal_bike();
					personal_bike.setId(rs.getInt(1));
					personal_bike.setUser(rs.getInt(2));
					personal_bike.setTel(rs.getString(3));
					personal_bike.setStart_time(rs.getDate(4));
					personal_bike.setEnd_time(rs.getDate(5));
					personal_bike.setRent(rs.getInt(6));
					personal_bike.setDesc(rs.getString(7));
					list.add(personal_bike);
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
