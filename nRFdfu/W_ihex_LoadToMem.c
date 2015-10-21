#include "W_ihex_LoadToMem.h"


#include <stdio.h>

extern "C" uint16_t crc16_compute(uint8_t * p_data, uint16_t size, uint16_t * p_crc);

int32_t W_ihex_LoadToMem( W_ihexMemImage_t *pI, char* filename )
{
    int i;
    uint16_t crc16;
    ihex_recordset_t *rs;
    int rv;
    ulong_t offset;
    ulong_t count;



    printf("\nW_ihexLoadToMem\n");

    //rs  = ihex_rs_from_file("C:/ble_nrf51/tandd/nrf51/examples/ble_peripheral/ble_app_K1__/pca10028/s110/arm4/_build/nrf51422_xxac_s110.hex");
    //rs  = ihex_rs_from_file("C:/ble_nrf51/tandd/nrf51/examples/ble_peripheral/ble_app_K2/pca10028/s110/arm4/_build/nrf51422_xxac_s110.hex");

    rs  = ihex_rs_from_file( filename );
    if( rs == 0 )
    {
        printf("Failed to get records from hex file\n");
        return( -1 );
    }

    pI->_gx_size = ihex_rs_get_size(rs);
    printf("  ihex_rs_get_size() -> _gx_size = %d\n", pI->_gx_size);

    if(pI->_gx_size > pI->gx_binCapacity )
    {
        printf("  Image File is too big for alotted memory. Image size = %d, Memory size = %d\n\n", pI->_gx_size , pI->gx_binCapacity );
        return(-1);
    }

    rv = ihex_rs_get_address_range(rs, &pI->gx_start, &pI->gx_stop);
    printf("  ihex_rs_get_address_range() rv = %d, -> gx_start = 0x%08x, -> gx_stop = 0x%08x\n", rv, pI->gx_start, pI->gx_stop);

    //dst = (uint8_t*) malloc(0x30000);//8192);

	if (rs == NULL)
	{
		//CU_FAIL("File \"res/big-a.hex\" does not exist.");
		return(-1);
	}
	
	//rv = ihex_mem_copy(rs, dst, 0x30000/*8192*/, IHEX_WIDTH_8BIT, IHEX_ORDER_BIGENDIAN);

    offset = 0;
    count = pI->_gx_size;
    rv = ihex_mem_copy_from_offset(rs, pI->gx_bin, offset, count, IHEX_WIDTH_8BIT, IHEX_ORDER_BIGENDIAN);
    printf("  ihex_mem_copy_from_offset() rv = %d\n", rv);

    /* testing
    offset = 0x08;
    count = 0x10;
    rv = ihex_mem_copy_from_offset(rs, pI->gx_bin, offset, count, IHEX_WIDTH_8BIT, IHEX_ORDER_BIGENDIAN);
    printf("rv = %d\n", rv);

    offset = 0x2744;
    count = 0x100;
    rv = ihex_mem_copy_from_offset(rs, pI->gx_bin, offset, count, IHEX_WIDTH_8BIT, IHEX_ORDER_BIGENDIAN);
    printf("rv = %d\n", rv);
    */

    /**/
    //testing - start
    pI->gx_len = pI->_gx_size - 7;
    for( i=0 ; i< 10 ; i++)
    {
        pI->gx_len++;
        crc16 = 0xFFFF;
        crc16 = crc16_compute(pI->gx_bin, pI->gx_len, &crc16);
        printf("                       gx_len = %d (0x%x), data = 0x%02x,  crc16=%04x \n", pI->gx_len, pI->gx_len, pI->gx_bin[pI->gx_len], crc16);
    }
    //testing - start
    /**/


    // real
    pI->gx_len = pI->gx_stop - pI->gx_start;
    crc16 = 0xFFFF;
    crc16 = crc16_compute(pI->gx_bin, pI->gx_len, &crc16);
    pI->gx_crc = crc16;
    printf("\n");
    printf("  (gx_stop - gx_start) gx_len = %d (0x%x), data = 0x%02x,  crc16=%04x \n", pI->gx_len, pI->gx_len, pI->gx_bin[pI->gx_len], pI->gx_crc);
    printf("\n");

    //free(dst);

    return(0);
}
