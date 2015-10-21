
#include "myutils.h"


int32_t cb_push( uint8_t data_byte, uint8_t* buffer, uint16_t rdPtr, uint16_t* pwrPtr, uint16_t capacity )
{
    int next_wp;
    next_wp = *pwrPtr + 1;
    if( next_wp >= capacity ) // if it is equal then it is already out of bounds
        next_wp = 0;
    if( next_wp == rdPtr ) //Full
        return(0);

    buffer[*pwrPtr] = data_byte;
    *pwrPtr = next_wp;
    return(1);
}

int32_t cb_pop( uint8_t* pdata_byte, uint8_t* buffer, uint16_t* prdPtr, uint16_t wrPtr, uint16_t capacity )
{
    if( *prdPtr == wrPtr ) //Empty
        return(0);

    *pdata_byte = buffer[*prdPtr];
    (*prdPtr)++;
    if( *prdPtr >= capacity ) // if it is equal then it is already out of bounds
        *prdPtr = 0;
    return(1);
}

int32_t cb_peek( uint8_t* pdata_byte, uint8_t* buffer, uint16_t rdPtr, uint16_t wrPtr )
{
    if( rdPtr == wrPtr ) //Empty
        return(0);
    *pdata_byte = buffer[rdPtr];
    return(1);
}

 
int32_t cb_available(uint16_t rdPtr, uint16_t wrPtr, uint16_t capacity)
{
    int32_t size1;
    int32_t size2;
    int32_t size = 0;

    if( rdPtr == wrPtr ) //Empty
        return(capacity-1);

    if( rdPtr < wrPtr )
    {
        size1 = rdPtr - 0;
        size2 = (capacity - wrPtr) - 1;
        size = size1 + size2;
        return(size);
    }

    if( wrPtr < rdPtr )
    {
        size = (rdPtr - wrPtr) - 1;
        return(size);
    }
    return(size);
}

int32_t cb_count( uint16_t rdPtr, uint16_t wrPtr, uint16_t capacity )
{
    int32_t size1;
    int32_t size2;
    int32_t size = 0;

    if( rdPtr == wrPtr ) //Empty
        return(0);

    if( rdPtr < wrPtr )
    {
        size = wrPtr - rdPtr;
        return(size);
    }

    if( wrPtr < rdPtr )
    {
        size1 = capacity - rdPtr;
        size2 = wrPtr - 0;
        size = size1 + size2;
        return(size);
    }
    return(size);
}

int32_t cb_delete_range( uint8_t* buffer, uint16_t startPtr, uint16_t stop, uint16_t capacity )
{
    return(-1);
}
 


    
int32_t cb16_available(cb16_t*cb)
{
    return( cb_available(cb->rdPtr, cb->wrPtr, cb->capacity) );
}

int32_t cb16_push( cb16_t*cb, uint8_t data_byte )
{
    return( cb_push(data_byte, cb->buffer, cb->rdPtr, &cb->wrPtr, cb->capacity) );
}

int32_t cb16_pop( cb16_t*cb , uint8_t* pdata_byte )
{
    return( cb_pop(pdata_byte, cb->buffer, &cb->rdPtr, cb->wrPtr, cb->capacity) );
}

int32_t cb16_peek( cb16_t*cb, uint8_t* pdata_byte )
{
    return( cb_peek(pdata_byte, cb->buffer, cb->rdPtr, cb->wrPtr) );
}

int32_t cb16_count( cb16_t*cb )
{
    return( cb_count(cb->rdPtr, cb->wrPtr, cb->capacity) );
}

int32_t cb16_delete_range( cb16_t*cb, uint16_t startPtr, uint16_t endPtr)
{
    return( cb_delete_range( cb->buffer, startPtr, endPtr, cb->capacity) );
}

int32_t cb16_clear( cb16_t*cb )
{
    cb->rdPtr = 0;
    cb->wrPtr = 0;
    return(0);
}



#include <stdio.h>
#include <string.h> //memcpy
#include <stdlib.h> //malloc

buf32_t* buf32_Create(int byteCount)
{
    buf32_t* pbuf;
    pbuf = (buf32_t*)malloc(sizeof(buf32_t));
    pbuf->buffer = (uint8_t*)malloc( byteCount );
    pbuf->length = 0;
    pbuf->capacity = byteCount;

    return(pbuf);
}

void buf32_Destroy(buf32_t** p_pbuf)
{
    buf32_t* pbuf;

    if(p_pbuf == 0)
        return;

    pbuf = *p_pbuf;
    if( pbuf != 0)
    {
        if( pbuf->buffer != 0 )
        {
            free(pbuf->buffer);
            pbuf->buffer = 0;
        }
        pbuf->length = 0;
    }
    *p_pbuf = 0;
    return;
}

void buf32_zero(buf32_t *P)
{
    P->length = 0;
}

void buf32_join(buf32_t *P, buf32_t *P2)
{
    memcpy(&P->buffer[P->length], &P2->buffer[0], P2->length);
    P->length += P2->length;
}

void buf32_cpy(buf32_t *P, void* p_bytes, int count)
{
    memcpy(&P->buffer[P->length], p_bytes, count);
    P->length += count;
}

uint32_t buf32_count(buf32_t *P, uint8_t the_byte)
{
    uint32_t r = 0;
    uint32_t i;
    for( i=0; i<P->length; i++)
    {
        if( P->buffer[i] == the_byte)
            r++;
    }
    return(r);
}

void buf32_print(char * s, buf32_t *P)
{
    uint32_t i;
    //int  j;
    printf("%s\n", s );

    for(i=0 ; i < P->length; i++)
    {
        printf("%02x ", P->buffer[i] );
        if( (i % 16) == 15 )
            printf("\n");
    }
    printf("\n");
}

void buf32_printShort(char * s, buf32_t *P)
{
    uint32_t i;
    uint32_t LO, HI;
    //int  j;

    LO = 32;
    HI = 32;
    if( P->length > 32)
    {
        //printf("P->length = %d\n", P->length); 
        HI = P->length - 32;
        //printf("    HI = %d\n", HI);
        HI = HI / 16;
        //printf("    HI = %d\n", HI);
        HI = HI * 16;
        //printf("    HI = %d\n", HI);
        if( ( P->length - HI) > 32)
            HI = HI + 16;
    }

    printf("%s\n", s );

    for(i=0 ; i < P->length; i++)
    {     
        if( (i<LO) || (i>=HI) )
        {
            printf("%02x ", P->buffer[i] );
            if( (i % 16) == 15 )
                printf("\n");
        }
        else
            if( i== LO)
            {
                //printf("------------------------------------------------\n");
                printf(" ...\n");
            }
    }
    printf("\n");
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
uint16_t crc16_compute(uint8_t * p_data, uint16_t size, uint16_t * p_crc)
{
    uint16_t i;
    uint16_t crc = (p_crc == NULL) ? 0xffff : *p_crc;

    for (i = 0; i < size; i++)
    {
        crc  = (unsigned char)(crc >> 8) | (crc << 8);
        crc ^= p_data[i];
        crc ^= (unsigned char)(crc & 0xff) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xff) << 4) << 1;

        //printf("crc=%04x  ", crc);
    }
    return crc;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include <Windows.h>
void time_sleep(float time_in_seconds)
{
    uint32_t time_in_milliseconds;

    time_in_milliseconds = (uint32_t)(time_in_seconds * 1000.0);
    Sleep(time_in_milliseconds);

}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
uint8_t * int32_to_bytes(uint32_t nr)
{
    static uint8_t ints[4];
    ints[0] = (nr & 0x000000FF) ;
    ints[1] = (nr & 0x0000FF00) >> 8;
    ints[2] = (nr & 0x00FF0000) >> 16;
    ints[3] = (nr & 0xFF000000) >> 24;
    return(ints);
    //return ''.join(chr(b) for b in ints)
}

uint8_t * int16_to_bytes(uint32_t nr)
{
    static uint8_t ints[2];
    ints[0] = (nr & 0x000000FF) ;
    ints[1] = (nr & 0x0000FF00) >> 8;
    return(ints);
}
//def G_int16_to_bytes(nr):
//    ints = [0,0]
//    ints[0] = (nr & 0x00FF) 
//    ints[1] = (nr & 0xFF00) >> 8
//    return ''.join(chr(b) for b in ints)     
