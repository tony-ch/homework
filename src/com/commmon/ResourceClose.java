package com.commmon;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;

public class ResourceClose {
	public static void close(ResultSet rs,Statement stmt,Connection conn){
		try{
			if(rs!=null)rs.close();
			if(stmt!=null)stmt.close();
			if(conn!=null)conn.close();			
		}catch(Exception e){
			e.printStackTrace();
		}		
	}
	public static void close(ResultSet rs,Statement stmt){
		close(rs,stmt,null);
	}
	public static void close(Statement stmt,Connection conn){
		close(null,stmt,conn);
	}
	public static void close(Connection conn){
		close(null,null,conn);
	}
	public static void close(ResultSet rs){
		close(rs,null,null);
	}
	public static void close(Statement stmt){
		close(null,stmt,null);
	}
}
