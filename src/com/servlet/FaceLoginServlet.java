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
import java.nio.file.Files;
import java.nio.file.Paths;

@WebServlet("/faceLoginServlet")
public class FaceLoginServlet extends HttpServlet {
    private final static int VECLEN=128;
    private final static double TOLERANCE = 0.45;
    private File uploads;
    private File logfile;
    private String py_addface_path;
    private String facepath;

    @Override
    public void init() throws ServletException {
        uploads = new File(getServletContext().getRealPath(getServletContext().getInitParameter("upload.location")));
        logfile = new File(getServletContext().getRealPath(getServletContext().getInitParameter("log.location")));
        py_addface_path =  getServletContext().getRealPath(getServletContext().getInitParameter("addfacepy.location"));
        facepath =  getServletContext().getRealPath(getServletContext().getInitParameter("face.location"));
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
    }

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("begin: " + System.currentTimeMillis());
        AdminDao adminDao = new AdminDao();
        WorkerDao workerDao = new WorkerDao();
        UserDao userDao = new UserDao();
        HttpSession session=request.getSession();//获得一个Session对象，用于存放一些提示信息返回到前台
        request.setCharacterEncoding("UTF-8");//设置请求的编码方式
        response.setCharacterEncoding("UTF-8");//设置响应的编码方式

        //通过request对象从前台登录页面表单获得登录名和密码数据
        String loginname=request.getParameter("loginname");
        String imageString = request.getParameter("imgurl");
        String type=request.getParameter("type");
        Boolean successed=false;
        Person person=null;
        try{
            System.out.println("login:"+ loginname+",type: "+type);
            System.out.println();
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
            if(!successed){
                System.err.println("res: 用户不存在");
                session.setAttribute("message","用户不存在！！！");
                request.getRequestDispatcher("/face_login.jsp").forward(request, response);
                return;
            }
        }catch (Exception e) {
            e.printStackTrace();
        }
        successed = false;

        File img_file = File.createTempFile("-temp-",".jpg",uploads);
        String  known_csv = facepath+"/"+type+"-"+loginname+".csv";
        File check_csvfile = File.createTempFile("-temp-",".csv",new File(facepath));

        if(!Files.exists(Paths.get(known_csv))){
            System.err.println("res: 未录入人脸");
            session.setAttribute("message","未录入人脸！！！");
            request.getRequestDispatcher("/face_login.jsp").forward(request, response);
            return;
        }
        Process proc;
        try {
            byte[] imageByte;
            String base64Image = imageString.split(",")[1];

            BASE64Decoder decoder = new BASE64Decoder();
            imageByte = decoder.decodeBuffer(base64Image);
            OutputStream outputStream = new BufferedOutputStream(new FileOutputStream(img_file));
            outputStream.write(imageByte);
            System.out.println(img_file.getAbsolutePath());
            System.out.println(check_csvfile.getAbsolutePath());
            String[] args = new String[]{"python3", py_addface_path, img_file.getAbsolutePath(), check_csvfile.getAbsolutePath()};
            proc = Runtime.getRuntime().exec(args);// 执行py文件
            proc.waitFor();
        }catch (Exception e) {
            e.printStackTrace();
        }
        try {
            double dis=0;
            BufferedReader br_known = new BufferedReader(new FileReader(known_csv));
            BufferedReader br_check = new BufferedReader(new FileReader(check_csvfile.getAbsolutePath()));
            for(int i=0;i<VECLEN;i++){
                double item_known = Double.parseDouble(br_known.readLine().split(",")[0]);
                double item_check = Double.parseDouble(br_check.readLine().split(",")[0]);
                dis +=(item_check-item_known)*(item_check-item_known);
                if(dis>=TOLERANCE*TOLERANCE)
                    break;
                //System.out.println(item);
            }
            System.out.println("distance:"+ dis);
            if(dis<TOLERANCE*TOLERANCE)
                successed=true;
            br_check.close();
            br_known.close();
            System.out.println("end:"+System.currentTimeMillis());
            if(!successed){
                System.err.println("res: 人脸信息不匹配");
                session.setAttribute("message","人脸信息不匹配！！！");
                request.getRequestDispatcher("/face_login.jsp").forward(request, response);
                return;
            }
        } catch (Exception e) {
            System.err.println(e.getMessage());
            System.err.println("res: 未检测到人脸");
            session.setAttribute("message", "未检测到人脸！！！");
            request.getRequestDispatcher("/face_login.jsp").forward(request, response);
            return;
        }

        System.err.println("res: 成功");
        session.setAttribute("message", "登录成功");
        session.setAttribute("person",person);
        request.getRequestDispatcher("/index.jsp").forward(request, response);
    }
}
