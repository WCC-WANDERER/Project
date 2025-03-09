package pl.polsl.lab.servlets;

import java.io.IOException;
import java.io.PrintWriter;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.util.stream.Stream;
import pl.polsl.lab.model.*;

/**
 * Servlet displaying the conversion history to the user.
 * 
 * @author Wing Cheung Chow
 * @version 1.5
 */
@WebServlet(name = "HistoryServlet", urlPatterns = {"/History"})
public class HistoryServlet extends HttpServlet {

    /**
     * Processes requests for both HTTP <code>GET</code> and <code>POST</code> methods.
     * Display the conversion history saved in the model to the user.
     * Clear conversion history button is available to the user.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        
        // Page layout and format
        response.setContentType("text/html; charset=UTF-8");
        PrintWriter out = response.getWriter();
        out.println("<!DOCTYPE html>");
        out.println("<html lang=\"en\">");
        out.println("<head>");
        out.println("<title>Conversion History</title>");
        out.println("<style>");
        out.println("button {");
        out.println("color: white;");
        out.println("padding: 15px 32px;");
        out.println("font-size: 16px;");
        out.println("border: none;");
        out.println("cursor: pointer; }");
        out.println(".btn-green {");
        out.println("background-color: #4CAF50; }");
        out.println(".btn-red {");
        out.println("background-color: red; }");
        out.println("button:hover {");
        out.println("opacity: 0.8; }");
        out.println("</style>");
        out.println("</head>");
        out.println("<body>");
        out.println("<h1>Conversion History</h1><br/>");
        
        // Get the model object from servlet context 
        NumConversion conversion = (NumConversion)getServletContext().getAttribute("model");
                
        // Use of stream for collection and for-each loop to get the saved conversion history
        Stream<DataHistory> stream;
        stream = conversion.getHistory().stream();
        stream.forEach(history -> {
        out.println("<pre><h2>Input: " + history.input() + "   " + 
                        "Output: " + history.output() +"</h2></pre>");  
        });
         
        // Clear history button
        out.println("<br/><br/><form action='History' method='POST'>");
        out.println("<button class='btn-red' type='submit' name='clearHistory' value='clear'>Clear history</button>");
        out.println("</form>");
        
        // Return to home page button
        out.println("<br/><br/><form action='History' method='POST'>");
        out.println("<button class='btn-green' type='submit' name='homePage' value='back'>Back to home page</button>");
        out.println("</form>");
        
        out.println("</body>");
        out.println("</html>");
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
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {      
        processRequest(request, response);                    
    }

    /**
     * Handles the HTTP <code>POST</code> method.
     * Clear history and back to home page buttons are handled respectively.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        
        String clear = request.getParameter("clearHistory"); 
        String back = request.getParameter("homePage");
        
        // Check which button is clicked by the user
        if("clear".equals(clear)) {
            // Clear conversion history saved in the model
            NumConversion conversion = (NumConversion)getServletContext().getAttribute("model");
            conversion.clearHistory();
            response.sendRedirect(request.getRequestURI());
        }
        else if("back".equals(back)) {
            // Return to home page
            response.sendRedirect("index.html");
        }       
    }

    /**
     * Returns a short description of the servlet.
     *
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "Conversion History";
    }// </editor-fold>
    
}
