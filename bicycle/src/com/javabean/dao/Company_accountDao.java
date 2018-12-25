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
import com.javabean.entity.Company_account;
import com.javabean.entity.Order;
public class Company_accountDao{
	//添加公司账户记录
	public void addCompany_account(int adminId,int bikenum,int purchaseId,Boolean ds){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="call HANDLE_PURCHASE(?,?,?,?)"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1,adminId);
			pstmt.setInt(2,bikenum );
			pstmt.setInt(3,purchaseId);
			pstmt.setBoolean(4,ds);
			pstmt.executeUpdate();
		}catch(Exception e){
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//通过记录编号删除
	public void delCompany_accountById(int id){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from company_account where id=?"; 
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
	//通过admin ID删除
	public void delCompany_accountByAdminID(int adminId){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from company_account where admin=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, adminId);
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//通过worker ID删除
	public void delCompany_accountByWorkerID(int workerId){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from company_account where worker=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, workerId);
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	
	//列表显示所有管理员列表
	public Map findAllRecord(int curPage){
		Company_account company_account=null;
		ArrayList list=new ArrayList();
		Connection conn=null;
		Statement pstmt=null;
		ResultSet rs=null;
		ResultSet r=null;
		Map map=null;
		Page pa=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from company_account order by id"; 
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
					company_account=new Company_account();
					company_account.setId(rs.getInt(1));
					company_account.setMoney(rs.getInt(2));
					company_account.setTime(rs.getTimestamp(3)); 
					company_account.setAdmin(rs.getInt(4));
					company_account.setWorker(rs.getInt(5));
					company_account.setType(rs.getString(6));
					list.add(company_account);
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
	public Map findAllRecordByTime(String start_time,String end_time,int curPage){
		Company_account company_account=null;
		ArrayList list=new ArrayList();
		Connection conn=null;
		Statement pstmt=null;
		ResultSet rs=null;
		ResultSet r=null;
		Map map=null;
		Page pa=null;
		//构造多条件查询的SQL语句
		String sql="select * from company_account where 1=1 ";
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
					company_account=new Company_account();
					company_account.setId(rs.getInt(1));
					company_account.setMoney(rs.getInt(2));
					company_account.setTime(rs.getTimestamp(3)); 
					company_account.setAdmin(rs.getInt(4));
					company_account.setWorker(rs.getInt(5));
					company_account.setType(rs.getString(6));
					list.add(company_account);
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
