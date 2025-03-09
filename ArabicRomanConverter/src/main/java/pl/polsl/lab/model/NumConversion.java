package pl.polsl.lab.model;

// Import for collections
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap; 
import java.util.List;
import java.util.Vector;
import java.util.stream.Stream;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * Identify the type of input number and converts to output notation (Arabic to Roman or vice versa).
 * Arabic numerals range from 1 to 3999 since the overline in string cannot be detected from input or shown to the standard output.
 * 
 * @author Wing Cheung Chow
 * @version 1.5
 */
@EqualsAndHashCode
@ToString
public class NumConversion {
    
//    private NumConversion instance;
//
//    // Private constructor prevents instantiation
//    private NumConversion() {
//    }
//
//    public NumConversion getInstance() {
//        if (instance == null) {
//            instance = new NumConversion();
//        }
//        return instance;
//    }

    /**
     * Store the output number after conversion.
     */
     @Getter @Setter private String outputNumber;
     
     /**
     * Store the data conversion history.
     */
     @Getter private List<DataHistory> history = new ArrayList<DataHistory>();

    /**
     * Check whether the input string is an integer.
     * 
     * @param input the string from command line arguments
     * @return   <code>true</code> if the string is an integer.
     *           <code>false</code> otherwise.
     */
    public boolean isStringInteger(String input) {

        try {  
            Integer.parseInt(input);
            return true;
        }
        catch (NumberFormatException e) {   
            return false;
        }   
    }
    
    /**
     * Convert the input number based on the notation. If it is an integer within the desired range, convert it to Roman notation.
     * If it is a string that matches with the rules of Roman numerals, convert it to Arabic notation.
     * Input validation together with custom exception class for exception handling.
     * 
     * @param inputNum the string from command line arguments
     * @throws InputOutputException if the input parameter is outside the range of 1 to 3999 
     *                              or does not match with the style of Roman notation.
     */
    public void convertNum (String inputNum) throws InputOutputException {
               
        // Check if the input string is an integer
        if (isStringInteger(inputNum)) {
                          
            int integerInput = Integer.parseInt(inputNum);
            
            // Input validation and exception handling
            if (integerInput < 1 || integerInput > 3999) 
                throw new InputOutputException("Invalid input parameter. Please try again.");
            else 
                arabicToRoman(integerInput);   // conversion
        }
        else {
            
            // Input validation and exception handling
            if (!inputNum.matches("^(?=.)M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$"))
                throw new InputOutputException("Invalid input parameter. Please try again.");
            else
                romanToArabic(inputNum);    // conversion
        }                      
    }
    
    /**
     * Convert the input integer to Roman numeral.
     * 
     * @param inputNum the integer converted from input parameter
     * @throws InputOutputException if the converted number does not match with the rules of Roman numerals
     */
    public void arabicToRoman (int inputNum) throws InputOutputException {
        
        if (inputNum < 1 || inputNum > 3999) 
            throw new InputOutputException("An unexpected error occurred. Please try again."); 

        else {            
            // Strings storing Roman numerals in different digits from 0 to 9        
            List<String> romanThousands=new ArrayList<>(Arrays.asList("", "M", "MM", "MMM", "MMMM", "MMMMM", "MMMMMM", "MMMMMMM", "MMMMMMMM", "MMMMMMMMM"));
            List<String> romanHundreds=new ArrayList<>(Arrays.asList("",  "C",  "CC",  "CCC",  "CD",
                       "D", "DC", "DCC", "DCCC", "CM"));
            List<String> romanTens=new ArrayList<>(Arrays.asList("",  "X",  "XX",  "XXX",  "XL",
                       "L", "LX", "LXX", "LXXX", "XC"));
            List<String> romanOnes=new ArrayList<>(Arrays.asList("",  "I",  "II",  "III",  "IV",
                       "V", "VI", "VII", "VIII", "IX"));

            // Convert input integer to Roman notation                    
            String result = romanThousands.get(inputNum / 1000) + 
                            romanHundreds.get((inputNum % 1000) / 100) + 
                            romanTens.get((inputNum % 100) / 10) + 
                            romanOnes.get(inputNum % 10);
        
            // Output validation and exception handling
            if (!result.matches("^(?=.)M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$") || result.isEmpty())
                throw new InputOutputException("An unexpected error occurred. Please try again.");        
            else
                outputNumber = result;  // Result saved in private field of the class  
        }
    }
    
    /**
     * Convert the input Roman numeral to Arabic numeral.
     * 
     * @param inputNum the Roman numeral from input parameter
     * @throws InputOutputException if the converted integer number does not lie in the range of 1 to 3999
     */
    public void romanToArabic (String inputNum) throws InputOutputException {
        
        if (inputNum.matches("^(?=.)M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$")) {
            
            // Put the integer values in accordance with the Roman numerals in hashmap
            HashMap<Character, Integer> romanMap = new HashMap<>();
            romanMap.put('I', 1);
            romanMap.put('V', 5);
            romanMap.put('X', 10);
            romanMap.put('L', 50);
            romanMap.put('C', 100);
            romanMap.put('D', 500);
            romanMap.put('M', 1000);

            int result = 0;
        
            // Iterate through the string and get the integer values from hashmap
            for(int i = 0; i < inputNum.length(); i++){
            
                if(i > 0 && romanMap.get(inputNum.charAt(i)) > romanMap.get(inputNum.charAt(i-1))) {
                    result += romanMap.get(inputNum.charAt(i)) - 2 * romanMap.get(inputNum.charAt(i-1)); // deduct the previous added value, e.g. "IX" = +1 +10 -2 = 9
                } 
                else {
                result += romanMap.get(inputNum.charAt(i));
                }
            }
        
            // Output validation and exception handling      
            if (result < 1 || result > 3999) 
                throw new InputOutputException("An unexpected error occurred. Please try again.");       
            else
                outputNumber = Integer.toString(result);  // Result saved in private field of the class  
        }
        else
            throw new InputOutputException("An unexpected error occurred. Please try again.");    
    }

    /**
     * Add the record to the list of conversion history when called by the controller.
     * 
     * @param data the record to be added to the list of conversion history
     */
    public void addHistory(DataHistory data){
                
        history.add(data);        
    }
    
    /**
     * Clear the conversion history when called by the controller.
     * 
     */
    public void clearHistory(){
        
       history.clear();
    }
    
    /**
     * Prepare the data for table model to be updated in GUI.
     * 
     * @return vector of vector of string for JTable model
     */
    public Vector<Vector<String>> updateTableModel(){
        
        Vector<Vector<String>> result = new Vector<>();
        
        // Lambda expression with own functional interface
        DataSet dataHistory = (in, out) -> {
                    
            Vector<String> temp = new Vector<>();
            temp.add(in); temp.add(out);
            result.add(temp);
        };
               
        // Use of stream for collection and for-each loop
        Stream<DataHistory> stream;
        stream = history.stream();
        stream.forEach(history -> dataHistory.update(history.input(), history.output()));
      
      return result;      
    }   
}

/**
 * Provide an interface to use lambda expression for updating the table on GUI.
 * Used for lambda expression.
 * 
 * @author Wing Cheung Chow
 * @version 1.3
 */
interface DataSet {
   
    /**
     * Update the table of conversion history in GUI.
     * @param input data input history
     * @param output data output history
     */
    void update(String input, String output);
}