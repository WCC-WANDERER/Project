package pl.polsl.lab.servlets;

import jakarta.persistence.EntityManager;
import jakarta.persistence.PersistenceException;
import java.io.IOException;
import java.io.PrintWriter;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import pl.polsl.lab.entities.DataConversion;
import pl.polsl.lab.entities.DataConversionHistory;
import pl.polsl.lab.model.*;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Servlet responsible for conversion of input number to output number, saving the conversion history inside the model and to the database.
 * 
 * @author Wing Cheung Chow
 * @version 1.5
 */
@WebServlet(name = "ConversionServlet", urlPatterns = {"/Conversion"})
public class ConversionServlet extends HttpServlet {
    
    /**
     * Logger to log the persistence exception details.
     */
    private static final Logger LOGGER = Logger.getLogger(ConversionServlet.class.getName());
    
    /**
     * Counter for the number of successful conversion.
     */
    private int successCounter = 0;

    /**
     * Counter for the number of input error and IO exception error occurred.
     */
    private int errorCounter = 0;
    
    /**
     * Processes requests for both HTTP <code>GET</code> and <code>POST</code> methods.
     * Convert the input number to output number and display it to the user. 
     * Related cookies and operations are saved in cookies access and database respectively.
     * In case of input error, IO exception or database exception, error message is send to the user.
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
        out.println("<title>Conversion Result</title>");
        out.println("<style>");
        out.println("button {");
        out.println("background-color: #a0a0a0;");
        out.println("color: white;");
        out.println("padding: 15px 32px;");
        out.println("font-size: 16px;");
        out.println("border: none;");
        out.println("cursor: pointer; }");
        out.println("button:hover {");
        out.println("background-color: #dcdcdc; }");
        out.println("</style>");
        out.println("</head>");
        out.println("<body>");
        out.println("<h1>Conversion Result</h1>");
        
        // Get the model object from servlet context 
        NumConversion conversion = (NumConversion)getServletContext().getAttribute("model");
 
        // Get parameter values - input number
        String inputNum = request.getParameter("inputnumber");        
        
        // Input parameter was not given - send error message
        if (inputNum == null || inputNum.trim().isEmpty()) {
            // Count the number of error and add the related cookie for record
            errorCounter += 1;
            String key = "Error"+ Integer.toString(errorCounter);
            Cookie errorCookie = new Cookie(key, "Empty");
            response.addCookie(errorCookie);
            response.sendError(HttpServletResponse.SC_BAD_REQUEST, "You should give an input parameter!");
        }
        else {
            
            // IO exception handling
            try {               
                conversion.convertNum(inputNum);               
            } 
            catch (InputOutputException ex) {
                // Count the number of error and add the related cookie for record
                errorCounter += 1;
                String key = "Error"+ Integer.toString(errorCounter);
                Cookie errorCookie = new Cookie(key, "Exception");
                response.addCookie(errorCookie);
                response.sendError(HttpServletResponse.SC_INTERNAL_SERVER_ERROR, ex.getMessage());
                return;
            }
            
            // Get and return the output number
            String outputNum = conversion.getOutputNumber();       
            
            // Add to databaseEntry history
            DataHistory data = new DataHistory(inputNum, outputNum);   // record class
            conversion.addHistory(data); 
            
            // Count the number of successful conversion and add the related cookie for record
            successCounter += 1;
            String key = "Convert"+ Integer.toString(successCounter);
            Cookie successConversionCookie = new Cookie(key, outputNum);
            response.addCookie(successConversionCookie);
                       
            // Write data to the database
            DataConversion databaseEntry = new DataConversion();
            databaseEntry.setInputNumber(inputNum);
            databaseEntry.setOutputNumber(outputNum);
            
            DataConversionHistory databaseEntryRecord = new DataConversionHistory();           
            databaseEntryRecord.setInputNumber(inputNum);
            databaseEntryRecord.setOutputNumber(outputNum);
            String time = new java.util.Date().toString().replace(' ','_');
            databaseEntryRecord.setDateAndTime(time);
            databaseEntryRecord.setOperation("Success");
            
            // Link the entities 
            databaseEntry.setDataConversionHistory(databaseEntryRecord);
            databaseEntryRecord.setDataConversion(databaseEntry);
                       
            EntityManager em = (EntityManager)getServletContext().getAttribute("database");           
            em.getTransaction().begin();
            try {
                // Persist operations on the model to the database
                em.persist(databaseEntry);
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
            
            // Print results in web page           
            out.println("<br/><h3>The input number is " + inputNum + ".</h3>");  
            out.println("<h3>The output number is " + outputNum + ".</h3><br/>");
            out.println("<p>Enjoy! You can go back to home page to start a new conversion.</p><br/><br/>");
                              
            // Return to home page button 
            out.println("<form action='Conversion' method='POST'>");
            out.println("<button type='submit'>Back to home page</button>");
            out.println("</form>");
            
            out.println("</body>");
            out.println("</html>");
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
        return "Conversion of Arabic-Roman number";
    }// </editor-fold>

}
