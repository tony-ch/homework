package com.servlet;

import com.javabean.dao.AdminDao;
import com.javabean.dao.UserDao;
import com.javabean.dao.WorkerDao;
import com.javabean.entity.Admin;
import com.javabean.entity.Person;
import com.javabean.entity.User;
import com.javabean.entity.Worker;
import sun.misc.BASE64Decoder;

import javax.imageio.ImageIO;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.awt.image.BufferedImage;
import java.io.*;

@WebServlet("/faceLoginServlet")
public class FaceLoginServlet extends HttpServlet {
    private final static int VECLEN=128;
    private final static double TORELANCE = 0.6;

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("begin: " + System.currentTimeMillis());
        AdminDao adminDao = new AdminDao();
        WorkerDao workerDao = new WorkerDao();
        UserDao userDao = new UserDao();
        HttpSession session=request.getSession();//获得一个Session对象，用于存放一些提示信息返回到前台
        request.setCharacterEncoding("UTF-8");//设置请求的编码方式
        response.setCharacterEncoding("UTF-8");//设置响应的编码方式
        File uploads = new File(getServletContext().getRealPath(getServletContext().getInitParameter("upload.location")));
        File logfile = new File(getServletContext().getRealPath(getServletContext().getInitParameter("log.location")));
        String py_addface_path =  getServletContext().getRealPath(getServletContext().getInitParameter("addfacepy.location"));
        String facepath =  getServletContext().getRealPath(getServletContext().getInitParameter("face.location"));
        //通过request对象从前台登录页面表单获得登录名和密码数据
        String loginname=request.getParameter("loginname");
        String imageString = request.getParameter("imgurl");
        String type=request.getParameter("type");
        Boolean successed=false;
        if(!uploads.exists()){
            uploads.mkdirs();
        }
        File face_dir = new File(facepath);
        if (!face_dir.exists()) {
            face_dir.mkdirs();
        }
        if(!logfile.getParentFile().exists()){
            logfile.getParentFile().mkdirs();
        }
        File file = File.createTempFile("-temp-",".jpg",uploads);

        File csvfile = File.createTempFile("-temp-",".csv",new File(facepath));
        System.out.println("init: "+ System.currentTimeMillis());
        Process proc;
        try {
            BufferedImage image = null;
            byte[] imageByte;
            String base64Image = imageString.split(",")[1];

            BASE64Decoder decoder = new BASE64Decoder();
            imageByte = decoder.decodeBuffer(base64Image);
//            ByteArrayInputStream bis = new ByteArrayInputStream(imageByte);
//            image = ImageIO.read(bis);
//            ImageIO.write(image,".jpg",file);
//            bis.close();
            OutputStream outputStream = new BufferedOutputStream(new FileOutputStream(file));
            outputStream.write(imageByte);
            System.out.println("read img: "+ System.currentTimeMillis());
            System.out.println(py_addface_path);
            System.out.println(file.getAbsolutePath());
            System.out.println(csvfile.getAbsolutePath());
            //System.out.println(facepath+"/admin-amdin.pickle");
            String[] args = new String[] { "python3", py_addface_path, file.getAbsolutePath(), csvfile.getAbsolutePath() };
            proc = Runtime.getRuntime().exec(args);// 执行py文件
            proc.waitFor();
            double dis=0;
            System.out.println("cal dist: "+ System.currentTimeMillis());
            BufferedReader br_known = new BufferedReader(new FileReader(facepath+"/"+type+"-"+loginname+".csv"));
            BufferedReader br_check = new BufferedReader(new FileReader(csvfile.getAbsolutePath()));
            for(int i=0;i<VECLEN;i++){
                double item_known = Double.parseDouble(br_known.readLine().split(",")[0]);
                double item_check = Double.parseDouble(br_check.readLine().split(",")[0]);
                dis +=(item_check-item_known)*(item_check-item_known);
                if(dis>=TORELANCE*TORELANCE)
                    break;
                //System.out.println(item);
            }
            System.out.println("distance:"+ dis);
            if(dis<TORELANCE*TORELANCE)
                successed=true;
            br_check.close();
            br_known.close();
            System.out.println("end:"+System.currentTimeMillis());
        } catch (Exception e) {
            e.printStackTrace();
        }

        if(!successed){
            //如果登录失败将登录失败提示信息放入session对象
            session.setAttribute("message","登录信息有误，请重新登录！！！");
            //如果登录失败跳转系统登录页面login.jsp，要求用户重新登录
            request.getRequestDispatcher("/face_login.jsp").forward(request, response);
            return;
        }

        try{
            System.out.println(loginname);
            System.err.println("type: "+type);
            Person person=null;
            //根据登陆方法返回的值判断用户是哪种类型用户，并将学生、教师或者管理员对象转化为User对象并设置对应的登录名、真实姓名、用户类型属性值
            if(type.equalsIgnoreCase("admin")){
                Admin admin=adminDao.login_face(loginname);
                if(admin!=null){
                    successed=true;
                    person=new Person();
                    person.setLoginname(admin.getLoginname());
                    person.setName(admin.getName());
                    person.setTel(admin.getTel());
                    person.setType(0);
                }
            }else if(type.equalsIgnoreCase("worker")) {
                Worker worker=workerDao.login_face(loginname);
                if(worker!=null){
                    successed=true;
                    person=new Person();
                    person.setLoginname(worker.getLoginname());
                    person.setName(worker.getName());
                    person.setTel("无");
                    if(worker.getDepartment().equalsIgnoreCase("B")){
                        person.setType(1);
                    }else{
                        person.setType(2);
                    }

                }
            }else{
                User user=userDao.login_face(loginname);
                if(user!=null){
                    successed=true;
                    person=new Person();
                    person.setLoginname(user.getLoginname());
                    person.setName(user.getName());
                    person.setTel(user.getTel());
                    person.setType(3);
                }
            }
            if(successed){
                //如果登录成功将获得的User对象存在session对象中（十分重要，程序以后要使用）
                session.setAttribute("person",person);
                //如果登录成功跳转系统主页面index.jsp
                request.getRequestDispatcher("/index.jsp").forward(request, response);
            }else{
                //如果登录失败将登录失败提示信息放入session对象
                session.setAttribute("message","登录信息有误，请重新登录！！！");
                //如果登录失败跳转系统登录页面login.jsp，要求用户重新登录
                request.getRequestDispatcher("/face_login.jsp").forward(request, response);
            }
        }catch (Exception e) {
            e.printStackTrace();
            request.setAttribute("exception", e);
            request.getRequestDispatcher("/exception.jsp").forward(request, response);
        }
    }
}
