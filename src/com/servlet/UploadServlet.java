package com.servlet;

import com.common.FaceServer;
import com.javabean.entity.Person;
import org.python.antlr.ast.Str;
import org.python.util.PythonInterpreter;

import javax.servlet.ServletException;
import javax.servlet.annotation.MultipartConfig;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.*;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.security.SecureRandom;
import java.util.Enumeration;

@WebServlet("/uploadServlet")
@MultipartConfig
public class UploadServlet extends HttpServlet {
    private static String uploads;
    private static File logfile;
    private static String py_addface_path;
    private static String facepath;

    @Override
    public void init() throws ServletException {
        uploads = getServletContext().getRealPath(getServletContext().getInitParameter("upload.location"));
        logfile = new File(getServletContext().getRealPath(getServletContext().getInitParameter("log.location")));
        py_addface_path =  getServletContext().getRealPath(getServletContext().getInitParameter("addfacepy.location"));
        facepath =  getServletContext().getRealPath(getServletContext().getInitParameter("face.location"));
        try {
            if(!Files.exists(Paths.get(uploads))){
                Files.createDirectories(Paths.get(uploads));
            }
            File face_dir = new File(facepath);
            if (!Files.exists(Paths.get(facepath))) {
                Files.createDirectories(Paths.get(facepath));
            }
            if(!logfile.getParentFile().exists()){
                logfile.getParentFile().mkdirs();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("SAVING>>>>>>>>>>>>>>>>>>>>");
        HttpSession session=request.getSession();
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        Part webcam = request.getPart("webcam");

        Object perObj=session.getAttribute("person");
        Person person=(Person)perObj;

        String file = FaceServer.generateTempPath(person.getLoginname()+"-",".jpg",uploads);

        System.out.println(webcam.getSize());
        System.out.println(webcam.getContentType());
        try (InputStream input = webcam.getInputStream()){
            //System.out.println(file.getAbsolutePath());
            Files.copy(input,Paths.get(file), StandardCopyOption.REPLACE_EXISTING);
        }
        Process proc;
        try {
            System.out.println(py_addface_path);
            System.out.println(file);
            //System.out.println(facepath+"/admin-amdin.pickle");
            FaceServer.forwardRequest("GET",request,response,file,facepath+"/"+person.getTypeStr()+"-"+person.getLoginname()+".csv");
//            String[] args = new String[] { "python3", py_addface_path, file, facepath+"/"+person.getTypeStr()+"-"+person.getLoginname()+".csv" };
//            proc = Runtime.getRuntime().exec(args);// 执行py文件
//            proc.waitFor();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doGet(request, response);
    }
}
