package com.common;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

import com.common.ConnectionFactory;
import com.common.RTException;
import com.common.ResourceClose;

public class Counter {
	
	public static int countAdmin(){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		int count =0 ;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="SELECT COUNT(*) as total FROM admin "; 
			pstmt=conn.prepareStatement(sql);
			rs=pstmt.executeQuery();
			if(rs.next()){
				count = rs.getInt("total");
			}
			return count;
		}catch(Exception e){
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	
	
	public static int countUser(){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		int count =0 ;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="SELECT COUNT(*) as total FROM user "; 
			pstmt=conn.prepareStatement(sql);
			rs=pstmt.executeQuery();
			if(rs.next()){
				count = rs.getInt("total");
			}
			return count;
		}catch(Exception e){
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	
	public static int countWorker(){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		int count =0 ;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="SELECT COUNT(*) as total FROM worker "; 
			pstmt=conn.prepareStatement(sql);
			rs=pstmt.executeQuery();
			if(rs.next()){
				count = rs.getInt("total");
			}
			return count;
		}catch(Exception e){
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
}
