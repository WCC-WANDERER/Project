package pl.polsl.lab.servlets;

import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.*;
import jakarta.servlet.*;
import java.io.*;

/**
 * Servlet presenting the use of cookies.
 *
 * @author Wing Cheung Chow
 * @version 1.5
 */
@WebServlet(name = "CookieAccessServlet", urlPatterns = {"/CookieAccess"})
public class CookieAccessServlet extends HttpServlet {
   
    /**
     * Processes requests for both HTTP <code>GET</code> and <code>POST</code> methods.
     * Display the statistics of cookies to the user.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    public void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        response.setContentType("text/html; charset=UTF-8");
        try (PrintWriter out = response.getWriter()) {
            
            // Page layout and format
            out.println("<!DOCTYPE html>");
            out.println("<html>");
            out.println("<head>");
            out.println("<title>Cookies Access Record</title>");
            out.println("<style>");
            out.println("button {");
            out.println("background-color: #ffff33;");
            out.println("color: black;");
            out.println("padding: 15px 32px;");
            out.println("font-size: 16px;");
            out.println("border: none;");
            out.println("cursor: pointer; }");
            out.println("button:hover {");
            out.println("background-color: #ffffcc; }");
            out.println("</style>");
            out.println("</head>");
            out.println("<body>");
            out.println("<h1>Cookies Access Record</h1>");
            
            // Checking cookies content and print the results
            int visitCounter = 0;
            Cookie[] cookies = request.getCookies();        
            if (cookies != null) {

                int successCounter = 0;
                int errorCounter = 0;
                String lastVisit = "never";
                
                for (Cookie cookie : cookies) {
                    
                    // Check the type of information inside the cookies and count it
                    if (cookie.getName().startsWith("Visit")) {
                        visitCounter += 1;
                        lastVisit = cookie.getValue();
                    }
                    else if (cookie.getName().startsWith("Convert")) {
                        successCounter += 1;                       
                    }
                    else if (cookie.getName().startsWith("Error")) {
                        errorCounter += 1;                       
                    }
                }
                if (visitCounter == 0) {                    
                    out.println("<br/><p>You have not visited this web page before.</p><br/>");
                }
                else {
                    // Display to the user for number of visit and the time of last visit
                    out.println("<br/><p>You have visited this web page for " + visitCounter + " time(s).</p>");
                    out.println("<p>Your last visit was " + lastVisit + ".</p><br/>");
                }
                
                // Display to the user for number of successful conversion and error occured as well as the length of cookies
                out.println("<hr><br/><p>Number of successful conversion: " + successCounter + "</p><br/>");
                out.println("<hr><br/><p>Number of error occured: " + errorCounter + "</p><br/>");
                out.println("<hr><br/><p>Total length of cookies: "+ Integer.toString(cookies.length)+ "</p><br/>");
            }
            else {
                out.println("<br/><p>You have not visited this web page before.</p><br/>");
            }
            
            // Return to home page button  
            out.println("<br/><form action='CookieAccess' method='POST'>");
            out.println("<button type='submit'>Back to home page</button>");
            out.println("</form>");
            
            out.println("</body>");
            out.println("</html>");
            
            // Count the number of visit and add the related cookie for record
            String key = "Visit"+ Integer.toString(visitCounter + 1);
            Cookie cookie = new Cookie(key, new java.util.Date().toString().replace(' ','_'));
            response.addCookie(cookie);           
        }
    }

    // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP <code>GET</code> method.
     * Documentation included in the processRequest method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Handles the HTTP <code>POST</code> method.
     * Redirect to home page after clicking the button.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        response.sendRedirect("index.html");
    }

    /**
     * Returns a short description of the servlet.
     *
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "Cookie Access Record";
    }// </editor-fold>

}
