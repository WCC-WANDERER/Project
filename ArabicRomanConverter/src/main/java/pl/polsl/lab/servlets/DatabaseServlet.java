package pl.polsl.lab.servlets;

import jakarta.persistence.EntityManager;
import jakarta.persistence.PersistenceException;
import jakarta.persistence.Query;
import java.io.IOException;
import java.io.PrintWriter;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.util.List;
import pl.polsl.lab.entities.DataConversion;
import pl.polsl.lab.entities.DataConversionHistory;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Servlet displaying the content of database.
 * 
 * @author Wing Cheung Chow
 * @version 1.5
 */
@WebServlet(name = "DatabaseServlet", urlPatterns = {"/Database"})
public class DatabaseServlet extends HttpServlet {

    /**
     * Logger to log the persistence exception details.
     */
    private static final Logger LOGGER = Logger.getLogger(DatabaseServlet.class.getName());
    
    /**
     * Processes requests for both HTTP <code>GET</code> and <code>POST</code> methods.
     * Display the content of database to the user.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        
        response.setContentType("text/html; charset=UTF-8");
        PrintWriter out = response.getWriter();
        out.println("<!DOCTYPE html>");
        out.println("<html lang=\"en\">");
        out.println("<head>");
        out.println("<title>Database Content</title>");
        out.println("<style>");
        
        // Add styling for buttons
        out.println("button {");
        out.println("background-color: #a0a0a0;");
        out.println("color: white;");
        out.println("padding: 15px 32px;");
        out.println("font-size: 16px;");
        out.println("border: none;");
        out.println("cursor: pointer;");
        out.println("}");
        out.println("button:hover {");
        out.println("background-color: #dcdcdc;");
        out.println("}");

        // Styling for table
        out.println("table {");
        out.println("border-collapse: collapse;"); // Prevents borders from overlapping
        out.println("width: 80%;");
        out.println("margin-bottom: 20px;"); // Space after table
        out.println("}");

        // Styling for table cells
        out.println("th, td {");
        out.println("border: 1px solid black;");
        out.println("padding: 12px 20px;"); // More padding for better readability
        out.println("text-align: left;");
        out.println("}");

        // Styling for header row
        out.println("th {");
        out.println("background-color: #f2f2f2;");
        out.println("font-weight: bold;");
        out.println("}");

        // Table spacing between columns
        out.println("table {");
        out.println("border-spacing: 5px;");
        out.println("}");
        out.println("</style>");
        out.println("</head>");
        out.println("<body>");             
        
        
        // First table with conversion results
        List<DataConversion> conversionList = null;  
        List<DataConversionHistory> conversionHistoryList = null;  
        EntityManager em = (EntityManager)getServletContext().getAttribute("database"); 
        em.getTransaction().begin();
        
        try {
            Query query = em.createQuery("SELECT d FROM DataConversion d");
            conversionList = query.getResultList();
            Query query2 = em.createQuery("SELECT h FROM DataConversionHistory h");
            conversionHistoryList = query2.getResultList();
            
            // First table for conversion results
            out.println("<h2>Conversion Results</h2>");
            out.println("<table>");
            out.println("<tr><th>ID no.</th><th>Input Number</th><th>Output Number</th></tr>");
            
            // Read data from database for first table
            for (DataConversion dataFromDB : conversionList) {
            out.println("<tr>");
            out.println("<td>" + dataFromDB.getId() + "</td>");
            out.println("<td>" + dataFromDB.getInputNumber() + "</td>");
            out.println("<td>" + dataFromDB.getOutputNumber() + "</td>");
            out.println("</tr>");
            }
            out.println("</table><br/>");           
               
            // Second table for conversion history
            out.println("<h2>Conversion History</h2>");
            out.println("<table>");
            out.println("<tr><th>ID no.</th><th>Data Conversion ID no.</th><th>Input Number</th><th>Output Number</th><th>Date and Time</th><th>Operation</th></tr>");

            // Read data from database for second table
            for (DataConversionHistory dataHistoryFromDB : conversionHistoryList) {
                out.println("<tr>");
                out.println("<td>" + dataHistoryFromDB.getId() + "</td>");
                out.println("<td>" + dataHistoryFromDB.getDataConversionId() + "</td>");
                out.println("<td>" + dataHistoryFromDB.getInputNumber() + "</td>");
                out.println("<td>" + dataHistoryFromDB.getOutputNumber() + "</td>");
                out.println("<td>" + dataHistoryFromDB.getDateAndTime() + "</td>");
                out.println("<td>" + dataHistoryFromDB.getOperation() + "</td>");
                out.println("</tr>");
            }
            out.println("</table>");
           
            // Commit to finish transaction
            em.getTransaction().commit();
        
        } catch (PersistenceException e) {
            
            // Log the exception with details
            LOGGER.log(Level.SEVERE, "Database operation failed: " + e.getMessage(), e);
    
            // Rollback the transaction to discard any partial changes 
            em.getTransaction().rollback();
                
            // Send a user-friendly error message to the client
            response.sendError(HttpServletResponse.SC_INTERNAL_SERVER_ERROR, 
                        "An unexpected error occurred. Please try again later.");             
        }
             
        // Return to home page button      
        out.println("<br/><form action='Database' method='POST'>");
        out.println("<button type='submit'>Back to home page</button>");
        out.println("</form>");
            
        out.println("</body>");
        out.println("</html>");        
    }

    // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP <code>GET</code> method.
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
        return "Database Content";
    }// </editor-fold>

}
