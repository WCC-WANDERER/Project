package pl.polsl.lab.entities;

import jakarta.persistence.*;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import java.io.Serializable;

/**
 * Entity representing the conversion of data history and relevant information.
 * 
 * @author Wing Cheung Chow
 * @version 1.5
 */
@Entity
public class DataConversionHistory implements Serializable {
    
    /**
     * Unique identifier used during the serialization and deserialization processes.
     */
    private static final long serialVersionUID = 1L;
    
    /**
     * Identifier of the database entries.
     */
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    /**
     * Input number in conversion.
     */
    @Column(nullable = false)
    private String inputNum;
    
    /**
     * Output number in conversion.
     */
    @Column(nullable = false)
    private String outputNum;
    
    /**
     * Date and time for the conversion completed.
     */
    @Column(nullable = false)
    private String dateAndTime;
    
    /**
     * Status of the conversion operation. Success or failure.
     */
    @Column(nullable = false)
    private String operation;

    /**
     * Initialize the entity from the parent table with foreign key.
     */
    @OneToOne
    @JoinColumn(name = "data_conversion_id", nullable = false)  // Creates the foreign key in this table
    private DataConversion dataConversion;
  
    /**
     * Get the entity for parent table.
     * 
     * @return entity of parent table
     */
    public DataConversion getDataConversion() {
        return dataConversion;
    }
    
    /**
     * Set up the entity corresponding to the parent table.
     * 
     * @param dataConversion entity of parent table
     */
    public void setDataConversion(DataConversion dataConversion) {
        this.dataConversion = dataConversion;
    }

    /**
     * Get the identifier of the database entry.
     * 
     * @return identifier of the database entry
     */
    public Long getId() {
        return id;
    }

    /**
     * Set the identifier of the database entry.
     * 
     * @param id identifier of the database entry
     */
    public void setId(Long id) {
        this.id = id;
    }
    
    /**
     * Get the data conversion identifier (foreign key) from the parent table.
     * 
     * @return identifier of the entity from parent table
     */
    public Long getDataConversionId() {
        return dataConversion.getId();
    }
    
    /**
     * Set the data conversion identifier (foreign key) from the parent table.
     * 
     * @param id identifier of the entity from parent table
     */
    public void setDataConversionId(Long id) {
        this.dataConversion.setId(id);
    }

    /**
     * Get the value of input number.
     *
     * @return the value of input number
     */
    public String getInputNumber() {
        return inputNum;
    }
    
    /**
     * Set the value of input number.
     *
     * @param input new value of input number
     */
    public void setInputNumber(String input) {
        this.inputNum = input;
    }
    
    /**
     * Get the value of output number.
     *
     * @return the value of output number
     */
    public String getOutputNumber() {
        return outputNum;
    }
    
    /**
     * Set the value of output number.
     *
     * @param output new value of output number
     */
    public void setOutputNumber(String output) {
        this.outputNum = output;
    }
    
    /**
     * Get the date and time of completed conversion.
     * 
     * @return date and time of the conversion
     */
    public String getDateAndTime() {
        return dateAndTime;
    }
    
    /**
     * Set the date and time of completed conversion.
     * 
     * @param time date and time of the conversion
     */
    public void setDateAndTime(String time) {
        this.dateAndTime = time;
    }
    
    /**
     * Get the status of the conversion.
     * 
     * @return status of the conversion
     */
    public String getOperation() {
        return operation;
    }
    
    /**
     * Set the status of the conversion.
     * 
     * @param operation status of the conversion
     */
    public void setOperation(String operation) {
        this.operation = operation;
    }

    /**
     * Function that check if the identifier of the entity is null or not, if it is not null, return a hash code value for an object.
     * Otherwise it will return 0.
     * 
     * @return hash code value
     */
    @Override
    public int hashCode() {
        int hash = 0;
        hash += (id != null ? id.hashCode() : 0);
        return hash;
    }

    /**
     * Function that check if the object is an instance of the DataConversionHistory class.
     * 
     * @param object object of any type
     * @return true - it is an instance of the DataConversionHistory class, false - it is not
     */
    @Override
    public boolean equals(Object object) {
        // TODO: Warning - this method won't work in the case the id fields are not set
        if (!(object instanceof DataConversionHistory)) {
            return false;
        }
        DataConversionHistory other = (DataConversionHistory) object;
        if ((this.id == null && other.id != null) || (this.id != null && !this.id.equals(other.id))) {
            return false;
        }
        return true;
    }

    /**
     * Function that return the identifier of the entity as a string.
     * 
     * @return identifier of the entity
     */
    @Override
    public String toString() {
        return "pl.polsl.lab.entities.DataConversion[ id=" + id + " ]";
    }

}
