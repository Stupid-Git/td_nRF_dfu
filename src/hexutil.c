

#include "dfu2.h"



#ifdef _UNICODE
#undef _UNICODE
//#error HexUtils Can't use Unicode
#endif


int  hu_LoadHexFile_App(hu_t *P, char *fname );
int  hu_Copy32KHexTo1KHex(hu_t *P);
int  HexText_ToMemory_1K( void );
int  InMem_ToOutMem_1K( void );
int  Memory_ToHexText_1K( void );
int  Copy1KHexTo32KHex( void );
int  Save32KHexFile( char *fname );
int  SaveLast1KHexFile( char *fname );

int  WriteTo32KmemBlock( int index, int size, unsigned char *pBuffer );
int  Convert32KmemBlockToHex( void );

//int  LoadLastSegHexFile( char *fname );
//int  SaveLastSegHexFile( char *fname );

int  GetParameters( uint8_t pMem[], sParams *pParams );
int  SetParameters( uint8_t pMem[], sParams *pParams );


#include <stdio.h>

//------------------------------------------------------------------------------
// Loads a hex file into m_32KHex_data(text) format
int  hu_LoadHexFile_App(hu_t *P, char *fname )
{
    int rv=0;
    FILE *fpi;
    errno_t errno;
    int  line_idx;
    int  char_idx;
    char one_line[132];

    errno = fopen_s( &fpi, fname, "rt" );
    if( errno!=0 ) {
        //::DO ERROR STUFF
        _tprintf( _T("Could not open file %s\n") , fname );
        return( -1 );
    }


    for( line_idx=0; line_idx<HEX_32KMAXLINES ; line_idx++ ) {
        for( char_idx=0; char_idx<HEX_32KMAXCHARS; char_idx++ ) 
            P->m_32KHex_data[line_idx][char_idx] = '\0';
        fgets( one_line, 132, fpi );
        if( strlen( one_line ) >= (HEX_32KMAXCHARS-1) ) { // -1 for room for the '\0' NULL
            //::ERROR
            _tprintf( _T("Line too long reading from hex_file %s\n") , fname );
            fclose( fpi );
            return( -1 );
        }
        strcpy_s( &P->m_32KHex_data[line_idx][0], HEX_32KMAXCHARS, one_line );
    }

    fclose( fpi );
    return( rv );
}

//------------------------------------------------------------------------------
// Saves 32K of data into a file from m_32KHex_data(text)
int  hu_Save32KHexFile(hu_t *P, char *fname )
{
    int rv=0;
    FILE *fpo;
    errno_t errno;
    int  line_idx;

    errno = fopen_s( &fpo, fname, "wt" );
    if( errno!=0 ) {
        //::DO ERROR STUFF
        _tprintf( _T("Could not open file %s\n") , fname );
        return( -1 );
    }

    for( line_idx=0; line_idx<HEX_32KMAXLINES ; line_idx++ ) {
        P->m_32KHex_data[line_idx][HEX_32KMAXCHARS-1] = '\0'; // just to make sure
        fprintf( fpo, "%s", &P->m_32KHex_data[line_idx][0] );
    }

    fclose( fpo );
    return( rv );
}

//------------------------------------------------------------------------------
// Saves the Last 1K of data into a file from ... ??m_32KHex_data(text)??
int  hu_SaveLast1KHexFile(hu_t *P, char *fname )
{
    int rv=0;
    FILE *fpo;
    errno_t errno;
    int  line_idx;

    errno = fopen_s( &fpo, fname, "wt" );
    if( errno!=0 ) {
        //::DO ERROR STUFF
        _tprintf( _T("Could not open file %s\n") , fname );
        return( -1 );
    }

    for( line_idx=0; line_idx<HEX_1KMAXLINES ; line_idx++ ) {
        P->m_1KHex_data[line_idx][HEX_1KMAXCHARS-1] = '\0'; // just to make sure
        fprintf( fpo, "%s", &P->m_1KHex_data[line_idx][0] );
    }

    fclose( fpo );
    return( rv );
}

//------------------------------------------------------------------------------
int  hu_Copy32KHexTo1KHex( hu_t *P )
{
    int  rv=0;
    int  line_idx_32K;
    int  line_idx_1K;

    line_idx_1K=0;
    for( line_idx_32K=64 * 31; line_idx_32K<HEX_32KMAXLINES ; line_idx_32K++ ) {
        strcpy_s( &P->m_1KHex_data[line_idx_1K][0], HEX_1KMAXCHARS, &P->m_32KHex_data[line_idx_32K][0] );
        line_idx_1K++;
    }

    return( rv );
}

//------------------------------------------------------------------------------
int  hu_Copy1KHexTo32KHex( hu_t *P )
{
    int  rv=0;
    int  line_idx_32K;
    int  line_idx_1K;

    line_idx_1K=0;
    for( line_idx_32K=64 * 31; line_idx_32K<HEX_32KMAXLINES ; line_idx_32K++ ) {
        strcpy_s( &P->m_32KHex_data[line_idx_32K][0], HEX_32KMAXCHARS, &P->m_1KHex_data[line_idx_1K][0] );
        line_idx_1K++;
    }

    return( rv );
}


/*
//------------------------------------------------------------------------------
int  hu_LoadLastSegHexFile( char *fname )
{
int rv=0;
return( rv );
}

//------------------------------------------------------------------------------
int  hu_SaveLastSegHexFile( char *fname )
{
int rv=0;
return( rv );
}
*/



//------------------------------------------------------------------------------
int  hu_InMem_ToOutMem_1K( hu_t *P )
{
    int i;
    for( i=0; i<0x400; i++ ) P->m_1KmemSegOut[i] = P->m_1KmemSegIn[i];
    return( 0 );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//static inline uint8_t HexCharToNibble(char c )
uint8_t HexCharToNibble(char c )
{
    if( c>='a' ) c = c - ('a'-'A');
    if( c>='A' ) return( 10 + (c -'A') );
    else         return(       c -'0' );
}
//------------------------------------------------------------------------------
// converts: m_1KHex_data(text) to m_1KmemSegIn(binary)
int  hu_HexText_ToMemory_1K( hu_t *P )
{
    int i;
    int line_idx;
    //int char_idx;
    unsigned short addr;
    //char           str[128];
    unsigned char  data;
    //unsigned char  cs;
    char  hn;
    char  ln;

    //            111111111122222222223333333333444
    //  0123456789012345678901234567890123456789012
    //  :107DC000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC3

    //----- Extract data from HEX file data -----
    line_idx = 0;
    addr = 0x7C00;
    for( line_idx=0; line_idx<HEX_1KMAXLINES-1 ; line_idx++ ) { // -1 because we add the last line manually

        // do it the easy way and just assume data is from position 9,10, ... 39,40

        for( i=0; i<16; i++ ) {
            hn = P->m_1KHex_data[line_idx][9+2*i + 0];  // high nibble
            ln = P->m_1KHex_data[line_idx][9+2*i + 1];  // high nibble
            data = (HexCharToNibble( hn )<<4) | HexCharToNibble( ln );
            P->m_1KmemSegIn[ addr-0x7C00 + i ] = data;
        }
        addr += 0x10;
    }

    return( 0 );
}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
// converts m_1KmemSegOut(binary) to m_1KHex_data(text)
int  hu_Memory_ToHexText_1K( hu_t *P )
{
    int i;
    int line_idx;
    int char_idx;
    unsigned short addr;
    char           str[128];
    unsigned char  data;
    unsigned char  cs;

    //----- Generate HEX file data -----
    line_idx = 0;
    addr = 0x7C00;
    for( line_idx=0; line_idx<HEX_1KMAXLINES-1 ; line_idx++ ) { // -1 because we add the last line manually

        for( char_idx=0; char_idx<HEX_1KMAXCHARS; char_idx++ ) 
            P->m_1KHex_data[line_idx][char_idx] = '\0';

        cs=0;
        strcpy_s( &P->m_1KHex_data[line_idx][0], 128, ":10" ); // Start Code: + byte count 0x10
        cs+=0x10;
        sprintf_s( str, 128, "%04X", addr );
        strcpy_s( &P->m_1KHex_data[line_idx][3], 125, str );   // 4byte address
        cs+= (addr>>8) &0x00FF;
        cs+= (addr>>0) &0x00FF;
        strcpy_s( &P->m_1KHex_data[line_idx][7], 121, "00" );  // record type 00 == data
        cs+=0x00;

        for( i=0; i<16; i++ ) {
            data = P->m_1KmemSegOut[ addr-0x7C00 + i ];
            cs = cs + data;
            sprintf_s( str, 128, "%02X", data );
            strcpy_s( &P->m_1KHex_data[line_idx][9+2*i], 100, str );  // add data
        }
        cs = (~cs) + 1;
        sprintf_s( str, 128, "%02X", cs );
        strcpy_s( &P->m_1KHex_data[line_idx][9+2*i], 100, str );  // add CS
        i++;
        P->m_1KHex_data[line_idx][9+2*i]='\n';  // add line ending
        P->m_1KHex_data[line_idx][9+2*i+1]='\0';  // add null

        addr += 0x10;
    }

    strcpy_s( &P->m_1KHex_data[line_idx++][0], 100, ":00000001FF\n" );  // Add Record End

    return( line_idx );
}



//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static unsigned char nibbleToChar( unsigned char nibble )
{
    unsigned char ch;

    if( nibble<0x0A )
        ch = '0' + nibble;
    else
        ch = 'a' + (nibble - 0x0a); // Rule: Use lower case

    return( ch );
}
static unsigned char HexToChar_HI( unsigned char byte )
{
    unsigned char nibble;
    nibble = (byte>>4) & 0x0f;
    return( nibbleToChar(nibble) );
}
static unsigned char HexToChar_LO( unsigned char byte )
{
    unsigned char nibble;
    nibble = (byte>>0) & 0x0f;
    return( nibbleToChar(nibble) );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int  hu_WriteTo32KmemBlock(hu_t *P, int index, int size, unsigned char *pBuffer )
{
    int i;
    int rv=0;
    if( index+size > 0x8000 ) {
        rv=-1;
        return( rv );
    }
    for( i=index; i<(index+size) ; i++ ) {
        P->m_32KmemBlock[i] = *pBuffer++;
    }
    return( rv );
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
// converts m_32KmemBlock(binary) to m_32KHex_data(text)
int  hu_Convert32KmemBlockToHex( hu_t *P )
{
    int i;
    int line_idx;
    int char_idx;
    unsigned short addr;
    char           str[128];
    unsigned char  data;
    unsigned char  cs;

    //----- Generate HEX file data -----
    line_idx = 0;
    addr = 0x0000;
    for( line_idx=0; line_idx<HEX_32KMAXLINES-1 ; line_idx++ ) { // -1 because we add the last line manually

        for( char_idx=0; char_idx<HEX_32KMAXCHARS; char_idx++ ) 
            P->m_32KHex_data[line_idx][char_idx] = '\0';

        cs=0;
        strcpy_s( &P->m_32KHex_data[line_idx][0], 128, ":10" ); // Start Code: + byte count 0x10
        cs+=0x10;
        sprintf_s( str, 128, "%04X", addr );
        strcpy_s( &P->m_32KHex_data[line_idx][3], 125, str );   // 4byte address
        cs+= (addr>>8) &0x00FF;
        cs+= (addr>>0) &0x00FF;
        strcpy_s( &P->m_32KHex_data[line_idx][7], 121, "00" );  // record type 00 == data
        cs+=0x00;

        for( i=0; i<16; i++ ) {
            data = P->m_32KmemBlock[ addr + i ];
            cs = cs + data;
            sprintf_s( str, 128, "%02X", data );
            strcpy_s( &P->m_32KHex_data[line_idx][9+2*i], 100, str );  // add data
        }
        cs = (~cs) + 1;
        sprintf_s( str, 128, "%02X", cs );
        strcpy_s( &P->m_32KHex_data[line_idx][9+2*i], 100, str );  // add CS
        i++;
        P->m_32KHex_data[line_idx][9+2*i]='\n';  // add line ending
        P->m_32KHex_data[line_idx][9+2*i+1]='\0';  // add null

        addr += 0x10;
    }

    strcpy_s( &P->m_32KHex_data[line_idx++][0], 100, ":00000001FF\n" );  // Add Record End

    return( line_idx );
}
