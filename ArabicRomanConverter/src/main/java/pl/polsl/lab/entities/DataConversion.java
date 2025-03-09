package pl.polsl.lab.entities;

import jakarta.persistence.*;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import java.io.Serializable;

/**
 * Entity representing the conversion of data.
 * 
 * @author Wing Cheung Chow
 * @version 1.5
 */
@Entity
public class DataConversion implements Serializable {
    
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
     * Initialize the entity for the foreign key on child table.
     */
    @OneToOne(mappedBy = "dataConversion", cascade = CascadeType.ALL)
    private DataConversionHistory dataConversionHistory;

    /**
     * Get the entity for child table.
     * 
     * @return entity of child table
     */
    public DataConversionHistory getDataConversionHistory() {
        return dataConversionHistory;
    }
    
    /**
     * Set up the entity corresponding to the child table.
     * 
     * @param dataConversionHistory entity of child table
     */
    public void setDataConversionHistory(DataConversionHistory dataConversionHistory) {
        this.dataConversionHistory = dataConversionHistory;
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
     * Function that check if the object is an instance of the DataConversion class.
     * 
     * @param object object of any type
     * @return true - it is an instance of the DataConversionHistory class, false - it is not
     */
    @Override
    public boolean equals(Object object) {
        // TODO: Warning - this method won't work in the case the id fields are not set
        if (!(object instanceof DataConversion)) {
            return false;
        }
        DataConversion other = (DataConversion) object;
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
