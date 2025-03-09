package pl.polsl.lab.model;

import java.util.Arrays;
import java.util.List;
import java.util.Vector;
import java.util.stream.Stream;
import org.junit.jupiter.api.*;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.CsvSource;
import org.junit.jupiter.params.provider.EmptySource;
import org.junit.jupiter.params.provider.MethodSource;
import org.junit.jupiter.params.provider.NullSource;
import org.junit.jupiter.params.provider.ValueSource;

/**
 * Prepare unit tests for all public methods in NumConversion class.
 * 
 * @author Wing Cheung Chow
 * @version 1.5
 */
public class NumConversionTest {
       
    /**
     * Declare NumConversion class variable for testing.
     */
    private NumConversion testConversion;
    
    /**
     * Initialize the testing variable before each test so that the data stored inside the variable are refreshed.
     */
    @BeforeEach
    public void setUp() {
        
        testConversion = new NumConversion();  
    }
    
    
    // Testing all public methods in NumConversion class

    /**
     * Check whether the method could distinguish the type of input number (on integer).
     * 
     * @param input input number by user
     */
    @ParameterizedTest
    @ValueSource(strings = {"1", "3999", "0", "-1", "10000"})
    public void testIsStringInteger(String input) {
        
        assertTrue(testConversion.isStringInteger(input), "Integer input should be here!");      
    }
    
    /**
     * Check whether the method could distinguish the type of input number (on null value, empty string and special characters).
     * 
     * @param input input number by user
     */
    @ParameterizedTest
    @NullSource
    @EmptySource
    @ValueSource(strings = {"abc", "MMM", "", " ", "\t", "\n", "."})
    public void testIsIntegerWithNonIntegerAndNullValue(String input) {
        
        assertFalse(testConversion.isStringInteger(input), "Non-integer input should be here!");       
    }
    
    /**
     * Check whether the method could convert the input number.
     * 
     * @param input input number by user
     * @param expected expected output number
     */
    @ParameterizedTest
    @CsvSource({"1,I", "3999,MMMCMXCIX", "I,1" , "MMMCMXCIX,3999"})
    public void testConvertNumWithoutException(String input, String expected) {

        assertDoesNotThrow(
        () -> testConversion.convertNum(input), 
        "The exception should not be thrown");
        assertEquals(expected, testConversion.getOutputNumber(), "The conversion is incorrect!");
    }
    
    /**
     * Check whether the method could throw custom exception with incorrect input number.
     * 
     * @param input input number by user
     */
    @ParameterizedTest
    @ValueSource(strings = {"-1", "0", "4000", "IIX", "MMMM", "c"})
    public void testConvertNumWithException(String input) {
    
        InputOutputException InputException = assertThrows(
                InputOutputException.class,
                () -> testConversion.convertNum(input),
                "The exception should be thrown");       
        assertEquals("Invalid input parameter. Please try again.", InputException.getMessage());       
    }
    
    /**
     * Check whether the method could convert the input number from Arabic notation to Roman notation.
     * 
     * @param input input number by user
     * @param expected expected output number
     */
    @ParameterizedTest
    @CsvSource({"1,I", "3999,MMMCMXCIX"})
    public void testArabicToRomanWithoutException (int input, String expected) {

        assertDoesNotThrow(
        () -> testConversion.arabicToRoman(input), 
        "The exception should not be thrown");
        assertEquals(expected, testConversion.getOutputNumber(), "Arabic to Roman conversion is incorrect!");
    }
    
    /**
     * Check whether the method could throw custom exception with incorrect Arabic input number.
     * 
     * @param input input number by user
     */
    @ParameterizedTest
    @ValueSource(ints = {-1, 0, 4000})
    public void testArabicToRomanWithException (int input) {

        InputOutputException RomanOutputException = assertThrows(
                InputOutputException.class,
                () -> testConversion.arabicToRoman(input),
                "The exception should be thrown");      
        assertEquals("An unexpected error occurred. Please try again.", RomanOutputException.getMessage());  
    }
    
    /**
     * Check whether the method could convert the input number from Roman notation to Arabic notation.
     * 
     * @param input input number by user
     * @param expected expected output number
     */
    @ParameterizedTest
    @CsvSource({"I,1", "MMMCMXCIX,3999"})
    public void testRomanToArabicWithoutException (String input, String expected) {

        assertDoesNotThrow(
        () -> testConversion.romanToArabic(input), 
        "The exception should not be thrown");       
        assertEquals(expected, testConversion.getOutputNumber(), "Roman to Arabic conversion is incorrect!");  
    }
    
    /**
     * Check whether the method could throw custom exception with incorrect Roman input number.
     * 
     * @param input input number by user
     */
    @ParameterizedTest
    @ValueSource(strings = {"MMMM", "XXXXX", "XCC", "ix"})
    public void testRomanToArabicWithException (String input) {

        InputOutputException ArabicOutputException = assertThrows(
                InputOutputException.class,
                () -> testConversion.romanToArabic(input),
                "The exception should be thrown");       
        assertEquals("An unexpected error occurred. Please try again.", ArabicOutputException.getMessage());  
    }
    
    /**
     * Check whether the method could add the conversion history to the list stored in the testing variable.
     * 
     * @param input input number by user
     * @param output output number generated by algorithm
     */
    @ParameterizedTest
    @CsvSource({"1,I", "MMMCMXCIX,3999"})
    public void testAddHistory (String input, String output) {

        DataHistory data = new DataHistory(input, output);   // record class
        testConversion.addHistory(data);       
        assertEquals(testConversion.getHistory().get(0), data, "The conversion history stored is not the same!");
    }
    
    
    /**
     * Check whether the method could clear all conversion history in the list stored in the testing variable.
     */
    @Test
    //@ParameterizedTest
    //@CsvSource({"1,I", "MMMCMXCIX,3999"})
    public void testClearHistory () {

        testConversion.clearHistory();
        assertTrue(testConversion.getHistory().isEmpty(), "The list should be empty after clearing history!");
    }
    
    /**
     * Check whether the method could update the data for the table in GUI.
     * 
     * @param input input number by user
     * @param output output number generated by algorithm
     */
    @ParameterizedTest
    @CsvSource({"1,I", "MMMCMXCIX,3999", "1000,M", "IX,9"})
    public void testUpdateTableModel (String input, String output) {
        
        Vector<Vector<String>> actual = new Vector<>();
        
        DataHistory data = new DataHistory(input, output);   // record class
        testConversion.addHistory(data);
        actual = testConversion.updateTableModel();
        
        Vector<Vector<String>> expected = new Vector<>();
        Vector<String> temp = new Vector<>();
        temp.add(input); temp.add(output);
        expected.add(temp);
        
        // Check list of single element
        assertEquals(actual.get(0).get(0), expected.get(0).get(0), "The list of records of conversion history are not the same!");   
        assertEquals(actual.get(0).get(1), expected.get(0).get(1), "The list of records of conversion history are not the same!");             
    }
    
    /**
     * Provide a stream of arguments in terms of a list of conversion history (input number and output number).
     * 
     * @return stream of arguments in list of conversion history
     */
    private static Stream<Arguments> dataHistoryProvider() {
    return Stream.of(
      Arguments.of(Arrays.asList("1", "I", "MMMCMXCIX", "3999", "1000", "M")),
      Arguments.of(Arrays.asList("2058", "MMLVIII", "MDCLXXI", "1671", "2146", "MMCXLVI")),
      Arguments.of(Arrays.asList("IX", "9", "100", "C", "DCCCLXXXVIII", "888")));
    }
    
    /**
     * Check whether the method could update multiple data for the table in GUI.
     * 
     * @param list list of input and output number in conversion history
     */
    @ParameterizedTest   
    @MethodSource("dataHistoryProvider")
    public void testUpdateTableModelWithMultipleElements (List<String> list) {
               
        Vector<Vector<String>> actual = new Vector<>();
               
        // Check list of multiple elements
        for (int i = 0; i < list.size(); i += 2) {
            
            DataHistory data = new DataHistory(list.get(i), list.get(i+1));   // record class
            testConversion.addHistory(data);   // Insert values for actual vector
        }
        
        actual = testConversion.updateTableModel();       // Update inserted values for actual vector
        
        // Insert values for expected vector
        Vector<Vector<String>> expected = new Vector<>();
        
        for (int i = 0; i < list.size(); i += 2) {
            
            Vector<String> temp = new Vector<>();
            temp.add(list.get(i)); 
            temp.add(list.get(i+1)); 
            expected.add(temp);
        }      
              
        // Check the size of vectors before comparison
        if (actual.size() == expected.size()) {
            for (int i = 0; i < actual.size(); i++) {
             
                assertEquals(actual.get(i).get(0), expected.get(i).get(0), "The list of records of conversion history are not the same!");   
                assertEquals(actual.get(i).get(1), expected.get(i).get(1), "The list of records of conversion history are not the same!");
            }
        }
        else {
            System.out.println("Error! The size of vectors for comparison are different.");
        }
    }    
}
