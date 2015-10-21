#include "W_Nrf51Hex.h"


uint32_t Nrf51Hex_I_minaddr(nRFhex_t *pih)
{
    return(42);
}
uint32_t Nrf51Hex_I_maxaddr(nRFhex_t *pih)
{
    return(42);
}


bool Nrf51Hex__has_sp(nRFhex_t *pih, uint32_t sp_address)
{
    return(false);
    /* TODO
    little_endian_sp_address = self.gets(sp_address, 4) 
        init_sp = int(little_endian_sp_address[::-1].encode('hex'), 16)
        //print "%x" % init_sp

        return True if (init_sp > 0x20000000) else False
        */
}

/*
    def Nrf51Hex_hex_info(hexfile):
        start = hexfile.minaddr()
        end   = hexfile.maxaddr() + 1
        size  = end - start
        return size, start, end
        */

bool Nrf51Hex_is_softdevice(nRFhex_t *pih)
{
    bool inside_boundry;
    
    inside_boundry = (Nrf51Hex_I_minaddr(pih) < 0x1004);

    if(inside_boundry && Nrf51Hex__has_sp(pih, 4096) )
        return(true);
    else
        return(false);

    //inside_boundry = self.minaddr() < 0x1004
    //return True if (inside_boundry and Nrf51Hex__has_sp(4096)) else False
}


bool Nrf51Hex_is_application(nRFhex_t *pih)
{
    bool inside_boundry;
    uint32_t minAddr;
    
    minAddr = Nrf51Hex_I_minaddr(pih);
    inside_boundry = ((minAddr > 0x13FFC) && (minAddr < 0x30000));
    if (inside_boundry && Nrf51Hex__has_sp(pih, minAddr))
        return(true);
    else
        return( false );
    //inside_boundry = self.minaddr() > 0x13FFC and self.minaddr() < 0x30000
    //return True if (inside_boundry and Nrf51Hex__has_sp(self.minaddr())) else False
}

uint32_t Nrf51Hex__resolve_start_address(nRFhex_t *pih, uint32_t lower_limit, uint32_t upper_limit)
{
    bool has_sp;
    uint32_t test_address = lower_limit;
    while( test_address < upper_limit )
    {
        //try:
            has_sp = Nrf51Hex__has_sp(pih, test_address);
            if(has_sp)
                break;
        //except intelhex.NotEnoughDataError, e:
            test_address += 0x400;
    }    
    // Return the found start address if this is less than the upper limit
    // else return None to indicate nothing was found        
    if( test_address < upper_limit )
        return( test_address);
    else 
        return(0);
/*
    test_address = lower_limit
    while test_address < upper_limit:
        try:
            has_sp = Nrf51Hex__has_sp(test_address)
            if has_sp:
                break
        except intelhex.NotEnoughDataError, e:
            test_address += 0x400
        
    // Return the found start address if this is less than the upper limit
    // else return None to indicate nothing was found        
    return test_address if test_address < upper_limit else None
*/
}

uint32_t Nrf51Hex__resolve_end_address(nRFhex_t *pih, uint32_t lower_limit, uint32_t upper_limit)
{
    // find the last word in the bootloader, by going from the back until code is hit.
    uint32_t end_address = upper_limit;
    while( upper_limit >= lower_limit)
    {
        //try:
//TODO      data = self.gets(end_address, 4);
            end_address = end_address + 0x04;
            break;
        //except intelhex.NotEnoughDataError, e:
            end_address = end_address - 0x04;
    }
    return end_address;
/*
    // find the last word in the bootloader, by going from the back until code is hit.
    end_address = upper_limit
    while upper_limit >= lower_limit:
        try:
            data = self.gets(end_address, 4)
            end_address = end_address + 0x04
            break
        except intelhex.NotEnoughDataError, e:
            end_address = end_address - 0x04

    return end_address
*/
}

bool Nrf51Hex_is_bootloader(nRFhex_t *pih)
{
    bool inside_boundry;
    bool has_sp;
    uint32_t addr_start;
    uint32_t addr_end;
    uint32_t start_address;

    inside_boundry = (Nrf51Hex_I_maxaddr(pih) > 0x30000);
    has_sp = false;
    addr_start = 0x30000;
    addr_end = 0x40000;
    start_address = Nrf51Hex__resolve_start_address(pih, addr_start, addr_end);

    // if not None, we have a valid SP in the start address
    if( start_address )
        has_sp = true;

    if (inside_boundry && has_sp)
        return(true);
    else
        return( false );

    /*
    inside_boundry = self.maxaddr() > 0x30000 
    has_sp = False

    addr_start = 0x30000
    addr_end = 0x40000

    start_address = self._resolve_start_address(addr_start, addr_end)

    // if not None, we have a valid SP in the start address
    if start_address:
        has_sp = True

    return True if (inside_boundry and has_sp) else False
*/
}


/*
class Nrf51Hex(intelhex.IntelHex):
       
    def Nrf51Hex__init__(self, *args, **kwargs):
        super(Nrf51Hex, self).__init__(*args, **kwargs)
        self.sd_address_start = 0
        self.sd_address_end = 0
        self.bl_address_start = 0
        self.bl_address_end = 0
        self.app_address_start = 0
        self.app_address_end = 0

        if self.Nrf51Hex_is_softdevice():
            self.sd_address_start = 4096
            self.sd_address_end = int(self.gets(0x3008, 4)[::-1].encode('hex'), 16)

        if self.Nrf51Hex_is_bootloader():
            self.bl_address_start = self._resolve_start_address(0x30000, 0x40000)
            self.bl_address_end = self._resolve_end_address(0x30000, 0x40000)
            
        if self.Nrf51Hex_is_application():
            self.app_address_start = self.minaddr()
            self.app_address_end = self.maxaddr() + 1
*/

uint32_t Nrf51Hex_hex_type_get(nRFhex_t *pih)
{
    if( Nrf51Hex_is_softdevice(pih) && Nrf51Hex_is_bootloader(pih))
        return HexType_SD_BL;
    if( Nrf51Hex_is_softdevice(pih) )
        return HexType_SOFTDEVICE;
    if( Nrf51Hex_is_bootloader(pih) )
        return HexType_BOOTLOADER;
    if( Nrf51Hex_is_application(pih) )
        return HexType_APPLICATION;

    //BOGUS
    return(HexType_APPLICATION);//BOGUS
    //BOGUS
    //BOGUS    return(0);
}

nRFhex_t * G_open_hex(char *filename)
{
/*TODFO
    try:
        ih = intelhex.IntelHex(filename)
        return ih
    except (IOError, intelhex.IntelHexError), e:
        print ('Error reading file: %s\n' % e)
        raise Exception("Could not read hex format")
        */
    return(0);

}

