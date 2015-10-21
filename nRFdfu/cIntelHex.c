
#include <windows.h>

#include "cintelhex.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h> //uint32_t etc


void test_memory_is_copied_1(void)
{
    uint32_t addr_min;
    uint32_t addr_max;
	ihex_recordset_t *rs;
    int rv;
	uint8_t *dst;
    ulong_t size;
    ulong_t ofst;
    ulong_t n;

    rs  = ihex_rs_from_file("C:/ble_nrf51/tandd/nrf51/examples/ble_peripheral/ble_app_K1/pca10028/s110/arm4/_build/nrf51422_xxac_s110.hex");
 
    size = ihex_rs_get_size(rs);
    printf("ihex_rs_get_size: size = %d\n", size);

    rv = ihex_rs_get_address_range(rs, &addr_min, &addr_max);
    printf("ihex_rs_get_address_range: rv = %d, addr_min = 0x%08x, addr_max = 0x%08x\n", rv, addr_min, addr_max);

    dst = (uint8_t*) malloc(0x30000);//8192);

	if (rs == NULL)
	{
		//CU_FAIL("File \"res/big-a.hex\" does not exist.");
		return;
	}
	
	//rv = ihex_mem_copy(rs, dst, 0x30000/*8192*/, IHEX_WIDTH_8BIT, IHEX_ORDER_BIGENDIAN);

    ofst = 0;
    n = 0x20;
    rv = ihex_mem_copy_from_offset(rs, dst, ofst, n, IHEX_WIDTH_8BIT, IHEX_ORDER_BIGENDIAN);
    printf("rv = %d\n", rv);

    ofst = 0x08;
    n = 0x10;
    rv = ihex_mem_copy_from_offset(rs, dst, ofst, n, IHEX_WIDTH_8BIT, IHEX_ORDER_BIGENDIAN);
    printf("rv = %d\n", rv);

    ofst = 0x2744;
    n = 0x100;
    rv = ihex_mem_copy_from_offset(rs, dst, ofst, n, IHEX_WIDTH_8BIT, IHEX_ORDER_BIGENDIAN);
    printf("rv = %d\n", rv);

    free(dst);
}
