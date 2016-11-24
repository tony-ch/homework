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
import com.javabean.entity.Maintain;
import com.javabean.entity.Purchase;
public class MaintainDao{
	//添加维修单方法
	public void addMaintain(Maintain maintain){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="insert into maintain (user,bike,place,time) values(?,?,?,?)"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, maintain.getUser());
			pstmt.setInt(2, maintain.getBike());
			pstmt.setString(3, maintain.getPlace());
			pstmt.setTimestamp(4, maintain.getTime());
			pstmt.executeUpdate();
		}catch(Exception e){
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//删除维修单
	public void delMaintain(int id){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from maintain where id=?"; 
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
	//根据维修单编号查找维修单
		public Maintain findMaintainById(int id){
			Maintain maintain=null;
			Connection conn=null;
			PreparedStatement pstmt=null;
			ResultSet rs=null;
			try{
				conn=ConnectionFactory.getConnection();
				String sql="select * from maintain where id=?"; 
				pstmt=conn.prepareStatement(sql);
				pstmt.setInt(1, id);
				rs=pstmt.executeQuery();
				while(rs.next()){
					maintain=new Maintain();
					maintain.setId(rs.getInt(1));
					maintain.setUser(rs.getInt(2));
					maintain.setBike(rs.getInt(3));
					maintain.setMaintainer(rs.getInt(4));
					maintain.setPlace(rs.getString(5));
					maintain.setTime(rs.getTimestamp(6));
				}
			}catch (SQLException e) {
				e.printStackTrace();
				throw new RTException("数据库操作异常，请稍后重试!");
			}finally{
				ResourceClose.close(rs, pstmt, conn);
			}
			return maintain;
		}
	//根据user查找维修单
	public Maintain findMaintainByUser(int user){
		Maintain maintain=null;
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from maintain where user=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, user);
			rs=pstmt.executeQuery();
			while(rs.next()){
				maintain=new Maintain();
				maintain.setId(rs.getInt(1));
				maintain.setUser(rs.getInt(2));
				maintain.setBike(rs.getInt(3));
				maintain.setMaintainer(rs.getInt(4));
				maintain.setPlace(rs.getString(5));
				maintain.setTime(rs.getTimestamp(6));
			}
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
		return maintain;
	}
	//根据维修工id查找维修单
	public Maintain findMaintainByMainter(int maintainer){
		Maintain maintain=null;
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from maintain where maintainer=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, maintainer);
			rs=pstmt.executeQuery();
			while(rs.next()){
				maintain=new Maintain();
				maintain.setId(rs.getInt(1));
				maintain.setUser(rs.getInt(2));
				maintain.setBike(rs.getInt(3));
				maintain.setMaintainer(rs.getInt(4));
				maintain.setPlace(rs.getString(5));
				maintain.setTime(rs.getTimestamp(6));
			}
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
		return maintain;
	}
	//根据自行车id查找维修单
	public Maintain findMaintainByBIke(int bike){
		Maintain maintain=null;
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from maintain where bike=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, bike);
			rs=pstmt.executeQuery();
			while(rs.next()){
				maintain=new Maintain();
				maintain.setId(rs.getInt(1));
				maintain.setUser(rs.getInt(2));
				maintain.setBike(rs.getInt(3));
				maintain.setMaintainer(rs.getInt(4));
				maintain.setPlace(rs.getString(5));
				maintain.setTime(rs.getTimestamp(6));
			}
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
		return maintain;
	}
	//列表显示所有维修单
	public Map findAllMaintain(int curPage){
		Maintain maintain=null;
		ArrayList list=new ArrayList();
		Connection conn=null;
		Statement pstmt=null;
		ResultSet rs=null;
		ResultSet r=null;
		Map map=null;
		Page pa=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from maintain order by id"; 
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
					maintain=new Maintain();
					maintain.setId(rs.getInt(1));
					maintain.setUser(rs.getInt(2));
					maintain.setBike(rs.getInt(3));
					maintain.setMaintainer(rs.getInt(4));
					maintain.setPlace(rs.getString(5));
					maintain.setTime(rs.getTimestamp(6));
					list.add(maintain);
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
	public Map findAllMaintainByTime(String start_time,String end_time,int curPage){
		Maintain maintain=null;
		ArrayList list=new ArrayList();
		Connection conn=null;
		Statement pstmt=null;
		ResultSet rs=null;
		ResultSet r=null;
		Map map=null;
		Page pa=null;
		//构造多条件查询的SQL语句
		String sql="select * from maintain where 1=1 ";
		//模糊查询
		if(start_time!=null&&!start_time.equals("")){
			sql+=" and time>='"+start_time+"'";
		}
		if(end_time!=null&&!end_time.equals("")){
			sql+=" and time<='"+end_time+"'";
		}
		sql+=" order by id";
		//System.out.println(sql);
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
					maintain=new Maintain();
					maintain.setId(rs.getInt(1));
					maintain.setUser(rs.getInt(2));
					maintain.setBike(rs.getInt(3));
					maintain.setMaintainer(rs.getInt(4));
					maintain.setPlace(rs.getString(5));
					maintain.setTime(rs.getTimestamp(6));
					list.add(maintain);
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
