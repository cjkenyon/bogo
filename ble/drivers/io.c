#include "i2c.h"
#include "io.h"

/**
 * Set all pins as output pins
 * 
 * @param val The value to be written
 * 
*/
void io_expander_set_configuration(uint8_t val) {

    uint8_t write_buffer[2];

    write_buffer[0] = IO_CONFIG_REG;
    write_buffer[1] = val;

    /* Use cyhal_i2c_master_write to write the required data to the device. */
	if ( CY_RSLT_SUCCESS == cyhal_i2c_master_write(
							&i2c_master_obj, 				// I2C Object
							IO_SUBORDINATE_ADDR_W,		    // I2C Address
							write_buffer, 					// Array of data to write
							2, 								// Number of bytes to write
							0, 								// Block until completed
							true))							// Generate Stop Condition
    {
        // printf("We wrote %x to the config register %x\r\n", write_buffer[1], write_buffer[0]);
    } else {
        // printf("We did NOT write to the config register\r\n");
    }
}

/**
 * Set the the LEDs to be enabled
 * 
 * @param val
 * 
*/
void io_expander_set_output_port(uint8_t val) {
    
    uint8_t write_buffer[2];

    write_buffer[0] = IO_OUTPUT_REG;
    write_buffer[1] = val;
    printf("inside io set output port\r\n");

    /* Use cyhal_i2c_master_write to write the required data to the device. */
	if ( CY_RSLT_SUCCESS == cyhal_i2c_master_write(
							&i2c_master_obj, 				// I2C Object
							IO_SUBORDINATE_ADDR_W,		    // I2C Address
							write_buffer, 					// Array of data to write
							2, 								// Number of bytes to write
							0, 								// Block until completed
							true))							// Generate Stop Condition
    {
         printf("We wrote %x to the output reg %x\r\n", write_buffer[1], write_buffer[0]);
    } else {
         printf("We did NOT write to the output reg\r\n");
    }
}

/**
 * Get current status of output register
 * 
 * @param
 * 
*/
uint8_t io_expander_get_curr_output(void) {

    uint8_t write_buffer[1];
    uint8_t read_buffer[1];
    uint8_t result;

    write_buffer[0] = IO_OUTPUT_REG;

    /* Use cyhal_i2c_master_write to write the required data to the device. */
    if ( CY_RSLT_SUCCESS == cyhal_i2c_master_write(
							&i2c_master_obj, 				// I2C Object
							IO_SUBORDINATE_ADDR_W,		    // I2C Address
							write_buffer, 					// Array of data to write
							1, 								// Number of bytes to write
							10, 								// Block until completed
							false))							// Do NOT Generate Stop Condition
	{
		/* Use cyhal_i2c_master_read to read the required data from the device. */
		// The register address has already been set in the write above, so read a single byte
		// of data.
		if ( CY_RSLT_SUCCESS == cyhal_i2c_master_read(
								&i2c_master_obj, 			    // I2C Object
								IO_SUBORDINATE_ADDR_R,			// I2C Address
								read_buffer, 					// Read Buffer
								1, 								// Number of bytes to read
								10, 								// Block until completed
								true)) 							// Generate Stop Condition
		{

			result = read_buffer[0];
			return result;
		}
		// REMOVED ERROR HANDLING TO ALLOW BOGO TO KEEP FUNCTIONING

		// We think i2c is losing power when the pump is turned on so we enter these while loops
		// We commenting them out to ensure we can continue functioning. 

		//else
		//{
		//     /* Disable all interrupts. */
		//    __disable_irq();

		//    CY_ASSERT(0);

		//    while(1){};
		//}
	}
	//else
	//{
	//     /* Disable all interrupts. */
	//    __disable_irq();

	//    CY_ASSERT(0);

	//    while(1){};
	//}

	return 0xFF; // Should never get here
}

/**
 * Get current status of input register
 * 
 * @param
 * 
*/
uint8_t io_expander_get_curr_input(void) {

    uint8_t write_buffer[1];
    uint8_t read_buffer[1];
    uint8_t result;

    write_buffer[0] = IO_INPUT_REG;

    /* Use cyhal_i2c_master_write to write the required data to the device. */
    if ( CY_RSLT_SUCCESS == cyhal_i2c_master_write(
							&i2c_master_obj, 				// I2C Object
							IO_SUBORDINATE_ADDR_W,		    // I2C Address
							write_buffer, 					// Array of data to write
							1, 								// Number of bytes to write
							0, 								// Block until completed
							false))							// Do NOT Generate Stop Condition
	{
		/* Use cyhal_i2c_master_read to read the required data from the device. */
		// The register address has already been set in the write above, so read a single byte
		// of data.
		if ( CY_RSLT_SUCCESS == cyhal_i2c_master_read(
								&i2c_master_obj, 			    // I2C Object
								IO_SUBORDINATE_ADDR_R,			// I2C Address
								read_buffer, 					// Read Buffer
								1, 								// Number of bytes to read
								0, 								// Block until completed
								true)) 							// Generate Stop Condition
		{

			result = read_buffer[0];
			return result;
		}
		else
		{
		     /* Disable all interrupts. */
		    __disable_irq();

		    CY_ASSERT(0);

		    while(1){};
		}
	}
	else
	{
	     /* Disable all interrupts. */
	    __disable_irq();

	    CY_ASSERT(0);

	    while(1){};
	}

	return 0xFF; // Should never get here
}

/**
 * Turn decimal into bit mask for the LED
 * 
 * @param val
 * 
*/
uint8_t get_bit_mask(int val) {

    uint8_t result;

    switch (val)
    {
        case 0:
        {
            // 0000 0001
            result = 0x01;
            break;
        }
        case 1:
        {
            // 0000 0010
            result = 0x02;
            break;
        }
        case 2:
        {
            // 0000 0100
            result = 0x04;
            break;
        }
        case 3:
        {
            // 0000 1000
            result = 0x08;
            break;
        }
        case 4:
        {
            // 0001 0000
            result = 0x10;
            break;
        }
        case 5:
        {
            // 0010 0000
            result = 0x20;
            break;
        }
        case 6:
        {
            // 0100 0000
            result = 0x40;
            break;
        }
        case 7:
        {
            // 1000 0000
            result = 0x80;
            break;
        }
        case 8:
        {
            // ALL of them!
            // 1111 1111
            result = 0xFF;
            break;
        }
        default:
        {
            printf("Please give a number between 0 and 7\r\n");
            return 0x00;
        }
    }
    return result;
}

