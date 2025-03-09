package pl.polsl.lab.model;

/**
 * Customize the error message for exception to be thrown.
 * 
 * @author Wing Cheung Chow
 * @version 1.4
 */
public class InputOutputException extends Exception {
    
    /**
     * Initialize the constructor with a string and calls the constructor of superclass.
     * 
     * @param message the error message when exception is thrown
     */
    InputOutputException(String message){
        super(message);
    }    
}
