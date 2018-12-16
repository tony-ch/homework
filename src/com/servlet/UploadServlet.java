package com.servlet;

import com.javabean.entity.Person;
import org.python.util.PythonInterpreter;

import javax.servlet.ServletException;
import javax.servlet.annotation.MultipartConfig;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.*;
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;

@WebServlet("/uploadServlet")
@MultipartConfig
public class UploadServlet extends HttpServlet {
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("SAVING>>>>>>>>>>>>>>>>>>>>");
        HttpSession session=request.getSession();
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        Part webcam = request.getPart("webcam");
        File uploads = new File(getServletContext().getRealPath(getServletContext().getInitParameter("upload.location")));
        File logfile = new File(getServletContext().getRealPath(getServletContext().getInitParameter("log.location")));
        String py_addface_path =  getServletContext().getRealPath(getServletContext().getInitParameter("addfacepy.location"));
        String facepath =  getServletContext().getRealPath(getServletContext().getInitParameter("face.location"));
        Object perObj=session.getAttribute("person");
        Person person=(Person)perObj;
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
//        PythonInterpreter interpreter = new PythonInterpreter();
//        interpreter.exec("a=[1,2,3,4,2]");
//        interpreter.exec("print(sorted(a))");
//        interpreter.exec("a=[1,2,3,4]");
        File file = File.createTempFile(person.getLoginname()+"-",".jpg",uploads);
        //String fileName = Paths.get(webcam.getSubmittedFileName()).getFileName().toString();
        //System.out.println(webcam.getName());
        //System.out.println(fileName);
        System.out.println(webcam.getSize());
        System.out.println(webcam.getContentType());
        try (InputStream input = webcam.getInputStream()){
            //System.out.println(file.getAbsolutePath());
            Files.copy(input,file.toPath(), StandardCopyOption.REPLACE_EXISTING);
        }
        Process proc;
        try {
            System.out.println(py_addface_path);
            System.out.println(file.getAbsolutePath());
            //System.out.println(facepath+"/admin-amdin.pickle");
            String[] args = new String[] { "python", py_addface_path, file.getAbsolutePath(), facepath+"/"+person.getTypeStr()+"-"+person.getLoginname()+".csv" };
            proc = Runtime.getRuntime().exec(args);// 执行py文件
            proc.waitFor();
//            double rowList[] = new double[128];
//            try (BufferedReader br = new BufferedReader(new FileReader(facepath+"/admin-amdin.csv"))) {
//                String line;
//                for(int i=0;i<rowList.length;i++){
//                    line = br.readLine();
//                    String[] lineItems = line.split(",");
//                    double item = Double.parseDouble(lineItems[0]);
//                    rowList[i]=item;
//                    System.out.println(item);
//                }
//            }
//            catch(Exception e){
//                // Handle any I/O problems
//            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doGet(request, response);
    }
}
