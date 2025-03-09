package pl.polsl.lab.listeners;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.Persistence;
import jakarta.servlet.ServletContext;
import jakarta.servlet.ServletContextEvent;
import jakarta.servlet.ServletContextListener;
import pl.polsl.lab.model.NumConversion;

/**
 * Web application life cycle listener.
 *
 * @author Wing Cheung Chow
 * @version 1.5
 */
public class ContextServletListener implements ServletContextListener {

    /**
     * Initialize the servlet context with the NumConversion class.
     * 
     * @param sce servlet context event
     */
    @Override
    public void contextInitialized(ServletContextEvent sce) {
        
        String puName = sce.getServletContext().getInitParameter("puName");
        EntityManagerFactory emf = Persistence.createEntityManagerFactory(puName);
        EntityManager em = emf.createEntityManager();   
        NumConversion model = new NumConversion();
        ServletContext context = sce.getServletContext();
        context.setAttribute("model", model);
        context.setAttribute("database", em);
    }

//    @Override
//    public void contextDestroyed(ServletContextEvent sce) { 
//        
//        ServletContext context = sce.getServletContext();
//        EntityManager em = (EntityManager)context.getAttribute("database"); 
//        em.close();
//    }
}
