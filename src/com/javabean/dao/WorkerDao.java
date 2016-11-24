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
import com.javabean.entity.Worker;
public class WorkerDao {
	//添加员工方法
		public void addWorker(Worker worker){
			Connection conn=null;
			PreparedStatement pstmt=null;
			ResultSet rs=null;
			try{
				conn=ConnectionFactory.getConnection();
				String sql="insert into worker (login_name,password,department,name,salaryM) values(?,?,?,?,?)"; 
				pstmt=conn.prepareStatement(sql);
				pstmt.setString(1, worker.getLoginname());
				pstmt.setString(2, worker.getPassword());
				pstmt.setString(3, worker.getDepartment());
				pstmt.setString(4, worker.getName());
				pstmt.setDouble(5, worker.getSalary());
				pstmt.executeUpdate();
			}catch(Exception e){
				e.printStackTrace();
				throw new RTException("数据库操作异常，请稍后重试!");
			}finally{
				ResourceClose.close(rs, pstmt, conn);
			}
		}
		//删除员工方法
		public void delWorker(String loginName){
			Connection conn=null;
			PreparedStatement pstmt=null;
			ResultSet rs=null;
			try{
				conn=ConnectionFactory.getConnection();
				String sql="delete from worker where login_name=?"; 
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
		//修改员工信息
		public void updateWorker(Worker worker){
			Connection conn=null;
			PreparedStatement pstmt=null;
			ResultSet rs=null;
			try{
				conn=ConnectionFactory.getConnection();
				String sql="update worker set name=?,password=?,salaryM=? where login_name=?"; 
				pstmt=conn.prepareStatement(sql);
				pstmt.setString(1, worker.getName());
				pstmt.setString(2, worker.getPassword());
				pstmt.setInt(3, worker.getSalary());
				pstmt.setString(4, worker.getLoginname());
				System.out.println("after update:"+pstmt.executeUpdate());
			}catch (SQLException e) {
				e.printStackTrace();
				throw new RTException("数据库操作异常，请稍后重试!");
			}finally{
				ResourceClose.close(rs, pstmt, conn);
			}
		}
		//根据登录名查找员工
		public Worker findWorkerByLoginName(String loginName){
			Worker worker=null;
			Connection conn=null;
			PreparedStatement pstmt=null;
			ResultSet rs=null;
			try{
				conn=ConnectionFactory.getConnection();
				String sql="select * from worker where login_name=?"; 
				pstmt=conn.prepareStatement(sql);
				pstmt.setString(1, loginName);
				rs=pstmt.executeQuery();
				while(rs.next()){
					worker=new Worker();
					worker.setId(rs.getInt(1));
					worker.setLoginname(rs.getString(2));
					worker.setPassword(rs.getString(3));
					worker.setDepartment(rs.getString(4));
					worker.setName(rs.getString(5));
					worker.setSalary(rs.getInt(6));
					worker.setCnt(rs.getInt(7));
					worker.setBalance(rs.getInt(8));
				}
			}catch (SQLException e) {
				e.printStackTrace();
				throw new RTException("数据库操作异常，请稍后重试!");
			}finally{
				ResourceClose.close(rs, pstmt, conn);
			}
			return worker;
		}
		//列表显示所有管理员列表
		public Map findAllWorker(int curPage){
			Worker worker=null;
			ArrayList list=new ArrayList();
			Connection conn=null;
			Statement pstmt=null;
			ResultSet rs=null;
			ResultSet r=null;
			Map map=null;
			Page pa=null;
			try{
				conn=ConnectionFactory.getConnection();
				String sql="select * from worker order by id"; 
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
						worker=new Worker();
						worker.setId(rs.getInt(1));
						worker.setLoginname(rs.getString(2));
						worker.setPassword(rs.getString(3));
						worker.setDepartment(rs.getString(4));
						worker.setName(rs.getString(5));
						worker.setSalary(rs.getInt(6));
						worker.setCnt(rs.getInt(7));
						worker.setBalance(rs.getInt(8));
						list.add(worker);
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
		/*//多条件查询员工
		public List findAllWorkerByMostCon(String loginName,String name,String password){
			Worker worker=null;
			ArrayList list=new ArrayList();
			Connection conn=null;
			PreparedStatement pstmt=null;
			ResultSet rs=null;
			//构造多条件查询的SQL语句
			String sql="select * from worker where 1=1 ";
			//模糊查询
			if(loginName!=null&&!loginName.equals("")){
				sql+=" and login_name like '%"+loginName+"%'";
			}
			if(name!=null&&!name.equals("")){
				sql+=" and name like '%"+name+"%'";
			}
			if(password!=null&&!password.equals("")){
				sql+=" and password like '%"+password+"%'";
			}
			sql+=" order by login_name";
			try{
				conn=ConnectionFactory.getConnection();
				pstmt=conn.prepareStatement(sql);
				rs=pstmt.executeQuery();
				while(rs.next()){
					worker=new Worker();
					worker.setId(rs.getInt(1));
					worker.setLoginname(rs.getString(2));
					worker.setPassword(rs.getString(3));
					worker.setDepartment(rs.getString(4));
					worker.setName(rs.getString(5));
					worker.setSalary(rs.getDouble(6));
					list.add(worker);
				}
			}catch (SQLException e) {
				e.printStackTrace();
				throw new DaoException("数据库操作异常，请稍后重试!");
			}finally{
				ResourceClose.close(rs, pstmt, conn);
			}
			return list;
		}
		//多条件查询员工
		public Map findAllWorkerByMostCon(String loginName,String name,String password,int curPage){
			Worker worker=null;
			ArrayList list=new ArrayList();
			Connection conn=null;
			Statement pstmt=null;
			ResultSet rs=null;
			ResultSet r=null;
			Map map=null;
			Page pa=null;
			//构造多条件查询的SQL语句
			String sql="select * from worker where 1=1 ";
			//模糊查询
			if(loginName!=null&&!loginName.equals("")){
				sql+=" and login_name like '%"+loginName+"%'";
			}
			if(name!=null&&!name.equals("")){
				sql+=" and name like '%"+name+"%'";
			}
			if(password!=null&&!password.equals("")){
				sql+=" and password like '%"+password+"%'";
			}
			sql+=" order by login_name";
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
						worker=new Worker();
						worker.setId(rs.getInt(1));
						worker.setLoginname(rs.getString(2));
						worker.setPassword(rs.getString(3));
						worker.setDepartment(rs.getString(4));
						worker.setName(rs.getString(5));
						worker.setSalary(rs.getDouble(6));
						list.add(worker);
					}else{
						break;
					}
				}
				map=new HashMap();
				map.put("list",list);
				map.put("pa",pa);
			}catch (SQLException e) {
				e.printStackTrace();
				throw new DaoException("数据库操作异常，请稍后重试!");
			}finally{
				ResourceClose.close(rs, pstmt, conn);
				ResourceClose.close(r, null, null);
			}
			return map;
		}*/
		//员工登录验证方法
		public Worker login(String loginName,String password){
			Worker worker=null;
			Connection conn=null;
			PreparedStatement pstmt=null;
			ResultSet rs=null;
			try{
				conn=ConnectionFactory.getConnection();
				String sql="select * from worker where login_name=? and password=?"; 
				pstmt=conn.prepareStatement(sql);
				pstmt.setString(1, loginName);
				pstmt.setString(2, password);
				rs=pstmt.executeQuery();
				while(rs.next()){
					worker=new Worker();
					worker.setId(rs.getInt(1));
					worker.setLoginname(rs.getString(2));
					worker.setPassword(rs.getString(3));
					worker.setDepartment(rs.getString(4));
					worker.setName(rs.getString(5));
					worker.setSalary(rs.getInt(6));
					worker.setCnt(rs.getInt(7));
					worker.setBalance(rs.getInt(8));
				}
			}catch (SQLException e) {
				e.printStackTrace();
				throw new RTException("数据库操作异常，请稍后重试!");
			}finally{
				ResourceClose.close(rs, pstmt, conn);
			}
			return worker;
		}
}
