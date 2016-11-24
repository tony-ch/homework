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
import com.javabean.entity.Maintain;
import com.javabean.entity.Purchase;
public class PurchaseDao{
	//添加采购单方法
	public void addPurchase(Purchase purchase){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="insert into purchase (buyer,bikenum,money,time) values(?,?,?,?)"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, purchase.getBuyer());
			pstmt.setInt(2, purchase.getBikenum());
			pstmt.setInt(3, purchase.getMoney());
			pstmt.setTimestamp(4, purchase.getTime());
			pstmt.executeUpdate();
		}catch(Exception e){
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	public Purchase findPurchaseById(int id){	//根据采购单编号查找
		Purchase purchase=null;
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from purchase where id=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, id);
			rs=pstmt.executeQuery();
			while(rs.next()){
				purchase=new Purchase();
				purchase.setId(rs.getInt(1));
				purchase.setBuyer(rs.getInt(2));
				purchase.setBikenum(rs.getInt(3));
				purchase.setMoney(rs.getInt(4));
				purchase.setTime(rs.getTimestamp(5));
			}
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
		return purchase;
	}
	//删除采购单（根据采购员id）方法
	public void delPurchaseByBuyerId(int buyer){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from purchase where buyer=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, buyer);
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//删除采购单（根据采购单id）
	public void delPurchaseById(int id){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="delete from purchase where id=?"; 
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
	//修改采购单信息（根据buyerID）
	public void updatePurchase(Purchase purchase){
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="update purchase set bikenum=?,money=?,time=? where buyer=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, purchase.getBikenum());
			pstmt.setInt(2, purchase.getMoney());
			pstmt.setTimestamp(3, purchase.getTime());
			pstmt.setInt(4, purchase.getBuyer());
			pstmt.executeUpdate();
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
	}
	//根据buyerID查找采购单
	public Purchase findPurchaseByBuyer(int buyer){
		Purchase purchase=null;
		Connection conn=null;
		PreparedStatement pstmt=null;
		ResultSet rs=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from purchase where buyer=?"; 
			pstmt=conn.prepareStatement(sql);
			pstmt.setInt(1, buyer);
			rs=pstmt.executeQuery();
			while(rs.next()){
				purchase=new Purchase();
				purchase.setId(rs.getInt(1));
				purchase.setBuyer(rs.getInt(2));
				purchase.setBikenum(rs.getInt(3));
				purchase.setMoney(rs.getInt(4));
				purchase.setTime(rs.getTimestamp(5));
			}
		}catch (SQLException e) {
			e.printStackTrace();
			throw new RTException("数据库操作异常，请稍后重试!");
		}finally{
			ResourceClose.close(rs, pstmt, conn);
		}
		return purchase;
	}
	//列表显示所有采购单
	public Map findAllPurchase(int curPage){
		Purchase purchase=null;
		ArrayList list=new ArrayList();
		Connection conn=null;
		Statement pstmt=null;
		ResultSet rs=null;
		ResultSet r=null;
		Map map=null;
		Page pa=null;
		try{
			conn=ConnectionFactory.getConnection();
			String sql="select * from purchase order by id"; 
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
					purchase=new Purchase();
					purchase.setId(rs.getInt(1));
					purchase.setBuyer(rs.getInt(2));
					purchase.setBikenum(rs.getInt(3));
					purchase.setMoney(rs.getInt(4));
					purchase.setTime(rs.getTimestamp(5));
					list.add(purchase);
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
