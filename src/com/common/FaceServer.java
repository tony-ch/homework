package com.common;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.File;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.SecureRandom;
import java.util.Enumeration;

public class FaceServer {
    private static SecureRandom random =  new SecureRandom();

    public static String generateTempPath(String pre,String suf, String dir){
        long rnd = random.nextLong();
        if (rnd == -9223372036854775808L) {
            rnd = 0L;
        } else {
            rnd = Math.abs(rnd);
        }
        return new File(dir,pre+rnd+suf).getAbsolutePath();
    }

    public static void forwardRequest(String method, HttpServletRequest req, HttpServletResponse resp, String im, String res) {
        final boolean hasoutbody = (method.equals("POST"));

        try {
            final URL url = new URL("http://localhost:5000?im="+im+"&res="+res);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod(method);

            final Enumeration<String> headers = req.getHeaderNames();
            while (headers.hasMoreElements()) {
                final String header = headers.nextElement();
                final Enumeration<String> values = req.getHeaders(header);
                while (values.hasMoreElements()) {
                    final String value = values.nextElement();
                    conn.addRequestProperty(header, value);
                }
            }

            //conn.setFollowRedirects(false);  // throws AccessDenied exception
            conn.setUseCaches(false);
            conn.setRequestProperty("Connection", "close");
            conn.setDoInput(true);
            conn.setDoOutput(hasoutbody);
            conn.connect();

            final byte[] buffer = new byte[16384];
//            while (hasoutbody) {
//                final int read = req.getInputStream().read(buffer);
//                if (read <= 0) break;
//                conn.getOutputStream().write(buffer, 0, read);
//            }

            conn.getResponseCode();
//            for (int i = 0; ; ++i) {
//                final String header = conn.getHeaderFieldKey(i);
//                if (header == null) break;
//                final String value = conn.getHeaderField(i);
//                resp.setHeader(header, value);
//            }
//
//            while (true) {
//                final int read = conn.getInputStream().read(buffer);
//                if (read <= 0) break;
//                resp.getOutputStream().write(buffer, 0, read);
//            }
            conn.disconnect();
        } catch (Exception e) {
            e.printStackTrace();
            // pass
        }
    }

}
