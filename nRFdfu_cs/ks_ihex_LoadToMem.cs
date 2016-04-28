using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;

namespace nRFdfu_cs
{



    //=====================================================================
    //=====================================================================
    //=====================================================================
    class W_ihexMemImage_t //W_ihexMemImage_s
    {
        public Byte[] image_array;
        public UInt32 image_start;
        public UInt32 image_stop;  
        public UInt32 image_length;
        public UInt16 image_crc;   

        public W_ihexMemImage_t()
        {
            //image_array = new Byte[0x8000];
        }
    }




    class intelStuff
    {
        // CONSTANT DEFINITIONS

        const Byte IHEX_ERR_INCORRECT_CHECKSUM = 0x01;
        const Byte IHEX_ERR_NO_EOF = 0x02;
        const Byte IHEX_ERR_PARSE_ERROR = 0x03;
        const Byte IHEX_ERR_WRONG_RECORD_LENGTH = 0x04;
        const Byte IHEX_ERR_NO_INPUT = 0x05;
        const Byte IHEX_ERR_UNKNOWN_RECORD_TYPE = 0x06;
        const Byte IHEX_ERR_PREMATURE_EOF = 0x07;
        const Byte IHEX_ERR_ADDRESS_OUT_OF_RANGE = 0x08;
        const Byte IHEX_ERR_MMAP_FAILED = 0x09;
        const Byte IHEX_ERR_READ_FAILED = 0x0B;
        const Byte IHEX_ERR_MALLOC_FAILED = 0x0A;

        public enum ihex_rtype_t
        {
            IHEX_DATA = 0x00, IHEX_EOF = 0x01, IHEX_ESA = 0x02,
            IHEX_SSA = 0x03, IHEX_ELA = 0x04, IHEX_SLA = 0x05
        }

        public enum ihex_width_t
        {
            IHEX_WIDTH_8BIT = 1, IHEX_WIDTH_16BIT = 2,
            IHEX_WIDTH_32BIT = 4, IHEX_WIDTH_64BIT = 8
        }

        public enum ihex_byteorder_t { IHEX_ORDER_BIGENDIAN, IHEX_ORDER_LITTLEENDIAN, IHEX_ORDER_NATIVE }


        //typedef Byte*   ihex_rdata_t;
        //typedef Byte [] ihex_rdata_t;

        //typedef Byte     ihex_rlen_t
        //typedef Byte     ihex_rchks_t;
        //typedef UInt32   ihex_error_t;
        //typedef UInt16   ihex_addr_t

        // STRUCTURE DEFINITIONS

        /// Models a single Intel HEX record.
        /** This structure models a single Intel HEX record, i.e. a single line
         *  in an Intel HEX file. Each record basically consists of a type, a
         *  base address and a data segment of variable length. */
        public class ihex_record_t
        {
            public UInt32 ihr_length;   //!< Length of the record in bytes.
            public ihex_rtype_t ihr_type;     //!< Record type (see ihex_rtype_t).
            public UInt16 ihr_address;  //!< Base address offset.
            public Byte[] ihr_data;     //!< Record data.
            public Byte ihr_checksum; //!< The record's checksum.
        }

        /// Models a set of Intel HEX records.
        /** This structure models an entire set of Intel HEX record, i.e. a
         *  complete Intel HEX input file. Basically, it just consists of a list
         *  of ihex_record_t structures.
         *  The last record must be a special EOF record. */
        public class ihex_recordset_t
        {
            public UInt32 ihrs_count;   //!< Amount of records.
            public ihex_record_t[] ihrs_records; //!< A list of record (with ihrs_count elements).
        }

        // GLOBAL VARIABLES

        //#ifdef IHEX_PARSE_C
        static UInt32 ihex_last_errno = 0;
        static String ihex_last_error = "";
        //#else
        //extern UInt32 ihex_last_errno; //!< Error code of last error.
        //extern char*        ihex_last_error; //!< Description of last error.
        //#endif

        // METHOD DECLARATIONS

        /// Parse Intel HEX string from file.
        /** This method parses an Intel HEX string from a file. The file will be
         *  mapped into memory and then parsed. This method returns a pointer to
         *  a newly generated ihex_recordset_t object.
         * 
         *  @param filename The filename of the input file.
         *  @return         A pointer to a newly generated recordset object. */
        //ihex_recordset_t  ihex_rs_from_file(String filename);

        /// Parse Intel HEX string from memory input.
        /** This method parses an Intel HEX string stored in memory. This
         *  method returns a pointer to a newly generated ihex_recordset_t
         *  object.
         * 
         *  @param data The input start address.
         *  @param size The input size in bytes.
         *  @return         A pointer to a newly generated recordset object. */
        //ihex_recordset_t* ihex_rs_from_mem( Byte [] data, size_t size);

        /// Parse Intel HEX string from string input.
        /** This method parses an Intel HEX string from a string input. This
         *  method returns a pointer to a newly generated ihex_recordset_t
         *  object.
         * 
         *  @param data The input string (NUL-terminated).
         *  @return         A pointer to a newly generated recordset object. */
        //ihex_recordset_t ihex_rs_from_string(String data);

        /// Iterate over all records in a record set.
        /** This method should be called repeatedly to process all data
         *  records in a record set.  A counter variable must be provided to
         *  track the progress, initially set to zero.  When reaching the end
         *  of the record set, the counter is reset to zero and NULL is
         *  returned as the current record.  End-Of-File and address offset
         *  records are automatically handled, returning the offset address to
         *  be applied to the current record.  Example:
         * 
         *  @code
         *    UInt32 i = 0;
         *    ihex_record_t *record;
         *    uint32_t offset;
         *    int err;
         *    do {
         *       err = ihex_rs_iterate_data(rs, &i, &record, &offset);
         *       if (err || record == 0) break;
         *       printf("record %u at %lu with length %u\n",
         *              i, offset + record->ihr_address, record->ihr_length);
         *    } while (i > 0);
         *  @endcode
         *
         *  @param rs  [in] A pointer to the record set.
         *  @param i   [in,out] Track the number of the next record to process
         *  @param rec [out] A pointer to the current record, not updated if NULL is passed
         *  @param off [out] Offset to apply to the record's address field
         *  @return    0 on success, an error code otherwise. */

        //TODO        int ihex_rs_iterate_data(ihex_recordset_t* rs, UInt32 *i, ihex_record_t **rec, uint32_t *off);

        /// Gets a record set's size.
        /** This method determines a record set's size. This is done by adding
         *  the length of all records, however without regard to address offsets
         *  etc.
         * 
         *  @param rs A pointer to the record set.
         *  @return   The record set's size. */
        //TODO        UInt64 ihex_rs_get_size(ihex_recordset_t* rs);

        /// Finds the address range which is contained in a record set.
        /** This method determines a record set's size with regard to address
         *  offsets and record lengths.  The boundaries specify a memory
         *  region large enough to hold all contained data, for example:
         * 
         *  @code
         *    uint32_t min, max;
         *    ihex_rs_get_address_range(rs, &min, &max);
         *    void *data = malloc(max - min);
         *  @endcode
         * 
         *  @param rs  A pointer to the record set.
         *  @param min [out] Lowest address contained.
         *  @param max [out] Upper address boundary (just past highest address).
         *  @return    0 on success, an error code otherwise. */
        //TODO        int ihex_rs_get_address_range(ihex_recordset_t* rs, uint32_t *min, uint32_t *max);

        /// Frees resources associated with a record set.
        /** This method frees all memory allocated in one of the
         *  ihex_rs_from_*() functions.  Passing NULL as input is allowed and
         *  has no effect.
         * 
         *  @param rs A pointer to the record set. */
        //TODO        void ihex_rs_free(ihex_recordset_t* rs);

        /// Return error code, or 0 if no error occurred.
        /** This method returns the error code of the latest error.
         *  @return The error code of the latest error. */
        //TODO        UInt32 ihex_errno(void);

        /// Checks if a record's checksum is valid.
        /** Validate the record by adding up all bytes of a record.
         *  Including the checksum, the lower 8 bits of the sum of all
         *  bytes must be 0x00.
         * 
         *  @param r The record that is to be validated.
         *  @return  0 if the record is correct, otherwise 1. */
        //TODO        int ihex_check_record(ihex_record_t *r);

        /// Copy the content of a record set.
        /** This method copies the content of a record set to a certain
         *  location in memory.
         * 
         *  @param rs  The record set that is to be copied.
         *  @param dst A pointer to the destination address.
         *  @param n   The size of the allocated target area.
         *  @param w   The width of data words to be copied.
         *  @param o   Defines whether data words are big or little endian.
         *  @return    0 on success, an error code otherwise. */
        //TODO        int ihex_mem_copy(ihex_recordset_t *rs, void* dst, UInt64 n, ihex_width_t w, ihex_byteorder_t o);


        //TODO        int ihex_mem_copy_from_offset(ihex_recordset_t *rs, Byte [] /*void**/ dst, UInt64 ofst, UInt64 n, ihex_width_t w, ihex_byteorder_t o);

        /// Fill a memory area with zeroes.
        /** This method fills a whole memory area with zeros.
         * 
         *  @param dst Target area.
         *  @param n   The size of the target area.
         *  @return    0 on success, an error code otherwise. */
        //TODO        int ihex_mem_zero(void* dst, UInt64 n);

        /// Return error string, or NULL if no error occurred.
        //TODO        char* ihex_error(void);

        /// Parse 8-bit hex input.
        //TODO        Byte ihex_fromhex8(Byte *input);

        /// Parse 16-bit hex input.
        //TODO        UInt16 ihex_fromhex16(Byte *input);

        //=====================================================================
        //========== ihex_copy.c ==========
        //========== ihex_copy.c ==========
        //========== ihex_copy.c ==========


        //#include "config.h"
        //#define inline __INLINE
        //#include <windows.h>
        //#include "cintelhex.h"
        //#include <stdio.h>
        //#include <string.h>
        //#include <stdint.h> //UInt32 etc

        //#define IHEX_SET_ERROR(errnum, error, ...) \
        //{ char *e = (char*)malloc(512); \
        //snprintf(e, 512, error, ##__VA_ARGS__); \
        //ihex_set_error(errnum, e); }

        //#define IHEX_SET_ERROR_RETURN(errnum, error, ...) \
        //{ IHEX_SET_ERROR(errnum, error, ##__VA_ARGS__); \
        //return errnum; }

        //void ihex_set_error(UInt32 errnum, char* error);

        /*I AM NOT USING THIS ONE
        int ihex_mem_copy(ihex_recordset_t rs, ref Byte[] dst, UInt64 n, ihex_width_t w, ihex_byteorder_t o)
        {
            int r;
            UInt32 i = 0, j, l;
            UInt32 offset = 0x00, address = 0x00;

            Byte[] d = dst;
            ihex_record_t x = new ihex_record_t();

            if ((r = ihex_mem_zero(dst, n)) != 0)
            {
                return r;
            }

            i = 0; //start with record number 0
            do
            {
                r = ihex_rs_iterate_data(rs, ref i, ref x, ref offset);
                if (r != 0)
                {
                    return r;
                }
                else if (x == null)
                {
                    break;
                }

                address = (offset + x.ihr_address);

                if (address >= n)
                {
                    //IHEX_SET_ERROR_RETURN(IHEX_ERR_ADDRESS_OUT_OF_RANGE,  "Address 0x%08x is out of range", address);
                    return (IHEX_ERR_ADDRESS_OUT_OF_RANGE);
                }

                for (j = 0; j < x.ihr_length; j += ((uint)(w)) )
                {
                    UInt32 v = 0;
                    UInt32* target = (UInt32*)(d + address + j);

                    for (l = 0; (l < (uint)w) && (j + l < x.ihr_length); l++)
                    {
                        v += (uint) ((uint)(x.ihr_data[j + l]) << (8 * ((o == ihex_byteorder_t.IHEX_ORDER_BIGENDIAN) ? (uint)(((uint)w - 1) - l) : (uint)l )) );
                    }
                    *(target) = v;

                    //#ifdef IHEX_DEBUG
                    //printf("%08x . %08x = %08x\n", address + j, v, *target);
                    //#endif
                }
            }
            while (i > 0);

            return 0;
        }
        I AM NOT USING THIS ONE*/





        static int ihex_mem_zero(Byte[] dst, UInt64 n)
        {
            //#ifdef HAVE_MEMSET
            //	memset(dst, 0 , n);
            //#else
            UInt64 i = 0;

            for (i = 0; i < n; i++)
            {
                if (i >= (UInt64)dst.Length) break;
                dst[i] = 0x00;
            }
            //#endif

            return 0;
        }

        static public int ihex_mem_copy_from_offset(ihex_recordset_t rs, ref Byte[] dst, UInt64 ofst, UInt64 byte_count, ihex_width_t w, ihex_byteorder_t o)
        {
            int r;
            UInt32 i, j, l;
            UInt32 offset = 0x00, address = 0x00;

            UInt32 start = 0x00, stop = 0x00;

            Byte[] d = (Byte[])dst;
            ihex_record_t x = new ihex_record_t();


            if ((r = ihex_mem_zero(dst, byte_count)) != 0)
            {
                return r;
            }

            UInt32 target_idx;

            target_idx = 0;
            UInt32 bean_counter = 0;

            i = 0; // Start at first (0th) data record
            do
            {
                r = ihex_rs_iterate_data(rs, ref i, ref x, ref offset);
                if (r != 0)
                {
                    return r;
                }
                else if (x == null)
                {
                    break;
                }
                /*
                x.ihr_address;
                x.ihr_checksum;
                x.ihr_data;
                x.ihr_length;
                x.ihr_type;
                */
                //x.ihr_type == ihex_rtype_t.IHEX_DATA;

                if (start == 0)
                {
                    start = (uint)(offset + x.ihr_address + ofst);
                    stop = (uint)(start + byte_count);
                }
                /**/
                address = (offset + x.ihr_address);

                /*
                if (address >= byte_count)
                {
                    //IHEX_SET_ERROR_RETURN(IHEX_ERR_ADDRESS_OUT_OF_RANGE, "Address 0x%08x is out of range", address);
                    return(IHEX_ERR_ADDRESS_OUT_OF_RANGE);
                }
                */
                //address = 0;

                Int32 _shift;
                UInt32 v;
                for (j = 0; j < x.ihr_length; j += (uint)w)
                {
                    v = 0;
                    for (l = 0; (l < (uint)w) && (j + l < x.ihr_length); l++)
                    {
                        _shift = (Int32)(8 * ((o == ihex_byteorder_t.IHEX_ORDER_BIGENDIAN) ? (((uint)w - 1) - l) : (l)));
                        v += (UInt32)(((UInt32)(x.ihr_data[j + l])) << _shift);
                        //v += x.ihr_data[j + l] << (8 * ((o == ihex_byteorder_t.IHEX_ORDER_BIGENDIAN) ? (((uint)w - 1) - l) : (l)));
                    }
                    if (((address + j) >= start) && ((address + j) < stop))
                    {
                        if (w == ihex_width_t.IHEX_WIDTH_8BIT)
                        {
                            dst[target_idx + 0] = (Byte)((v >> 0) & 0x000000FF);
                            bean_counter += 1;
                        }
                        if (w == ihex_width_t.IHEX_WIDTH_16BIT)
                        {
                            dst[target_idx + 0] = (Byte)((v >> 0) & 0x000000FF);
                            dst[target_idx + 1] = (Byte)((v >> 8) & 0x000000FF);
                            bean_counter += 2;
                        }
                        if (w == ihex_width_t.IHEX_WIDTH_32BIT)
                        {
                            dst[target_idx + 0] = (Byte)((v >> 0) & 0x000000FF);
                            dst[target_idx + 1] = (Byte)((v >> 8) & 0x000000FF);
                            dst[target_idx + 2] = (Byte)((v >> 16) & 0x000000FF);
                            dst[target_idx + 3] = (Byte)((v >> 24) & 0x000000FF);
                            bean_counter += 4;
                        }

                        //dst[target_idx] = v;
                        target_idx += (uint)w;

                        //#ifdef IHEX_DEBUG
                        //printf("%08x . %08x = %08x\n", (address + j), v, *target);
                        //#endif
                    }
                    if ((address + j) >= stop)
                        break;
                }


            } while (i > 0);  // i == 0 implies we have run out of data records to read

            Console.Write("bean_counter = 0x{0:x08}, {0}\n", bean_counter); 

            return 0;
        }
        //========== ihex_copy.c ==========
        //========== ihex_copy.c ==========
        //========== ihex_copy.c ==========
        //========== ihex_copy.c ==========
        //=====================================================================

        //=====================================================================
        //========== ihex_parse.c ==========
        //========== ihex_parse.c ==========
        //========== ihex_parse.c ==========
        const Byte IHEX_CHR_RECORDMARK = 0x3A; // ':'

        static void IHEX_SET_ERROR(int errnum, int error)//, ...)
        {
        }
        /*
        #define IHEX_SET_ERROR(errnum, error, ...) \
            { char *e = (char*)malloc(512); \
              snprintf(e, 512, error, ##__VA_ARGS__); \
              ihex_set_error(errnum, e); }
        #define IHEX_SET_ERROR_RETURN(errnum, error, ...) \
            { IHEX_SET_ERROR(errnum, error, ##__VA_ARGS__); \
              return errnum; }
        */

        static Byte[] _read_file(String file_path)
        {
            Byte[] file_content = null;// = "";

            // 1.
            using (BinaryReader b = new BinaryReader(File.Open(file_path, FileMode.Open)))
            {
                // 2.
                // Position and length variables.
                int pos = 0;
                // 2A.
                // Use BaseStream.
                int length = (int)b.BaseStream.Length;
                file_content = new Byte[length];
                while (pos < length)
                {
                    file_content[pos] = b.ReadByte();  // SLOW !!! but 
                    pos++;
                }
            }

            //Console.Write("_read_file: file_content.Length = {0}\n", file_content.Length);

            return (file_content);
        }


        static public ihex_recordset_t ihex_rs_from_file(String filename)
        {
            Byte[] c;
            ihex_recordset_t r;

            c = _read_file(filename);

            if (c == null)
                return (null);

            r = ihex_rs_from_mem(c, c.Length);
            return r;
        }

        static ihex_recordset_t ihex_rs_from_mem(Byte[] data, int size)
        {
            UInt32 i = 0;
            int r = 0, c = 0;
          //ihex_record_t rec;
            ihex_recordset_t recls;
            Byte l;

            ihex_last_errno = 0;
            ihex_last_error = "";

            //-----------------------------------------------------------------
            // Count number of record marks in input string.
            c = 0;
            for (i = 0; i < size; i++)
            {
                if (data[i] == 0) // if we get a null that signifies the end
                    break;
                if (data[i] == IHEX_CHR_RECORDMARK) // == ':')
                    c++;
            }

            //Console.Write("ihex_rs_from_mem: number of record marks,   c = {0}\n", c);


            //-----------------------------------------------------------------
            // Allocate the set of records
            //
            //rec = new ihex_record_t();
            recls = new ihex_recordset_t();
            recls.ihrs_count = (UInt32)c;
            recls.ihrs_records = new ihex_record_t[c];
            for (i = 0; i < c; i++)
            {
                recls.ihrs_records[i] = new ihex_record_t();

                recls.ihrs_records[i].ihr_address = 0;
                recls.ihrs_records[i].ihr_data = null;
                recls.ihrs_records[i].ihr_checksum = 0x00;
                recls.ihrs_records[i].ihr_length = 0;
                //recls.ihrs_records[i].ihr_type = ihex_rtype_t.IHEX_DATA; // whatever
            }
            /*
            // Allocate memory for the record container structure and for each
            // individual record.
            if ((rec = (ihex_record_t*) calloc(c, sizeof(ihex_record_t))) == NULL)
            {
                IHEX_SET_ERROR(IHEX_ERR_MALLOC_FAILED, "Could not allocate memory");
                goto malloc_rec_failed;
            }

            if ((recls = (ihex_recordset_t*) malloc(sizeof(ihex_recordset_t))) == NULL)
            {
                IHEX_SET_ERROR(IHEX_ERR_MALLOC_FAILED, "Could not allocate memory");
                goto malloc_recls_failed;
            }
            recls.ihrs_count   = c;
            recls.ihrs_records = rec;
            */

            //-----------------------------------------------------------------
            // Go through the data again and grab all the records
            int idx;
            idx = 0;
            i = 0;
            while ((idx < size) && (data[idx] != 0x00))
            {
                i++;

                if (idx + 3 >= size)
                {
                    break;
                }

                l = ihex_fromhex8(data, (idx + 1)); //(((Byte []) data) + 1);
                if (idx + 9 + l * 2 >= size || (r = ihex_parse_single_record(data, (UInt32)idx, l, recls.ihrs_records[i - 1])) != 0)
                {
                    //TODO IHEX_SET_ERROR(r, "Line %i: %s", i, ihex_last_error);
                    goto parse_single_failed;
                }

                /*
                if (i < 10)
                {
                    //Console.Write("\n");
                    Console.Write("record {0}, Len = {1}, data[0..7] = {2:x02}, ", i - 1, recls.ihrs_records[i - 1].ihr_length, recls.ihrs_records[i - 1].ihr_data[0]);

                    if (recls.ihrs_records[i - 1].ihr_data.Length > 1) Console.Write("{0:x02} ", recls.ihrs_records[i - 1].ihr_data[1]);
                    if (recls.ihrs_records[i - 1].ihr_data.Length > 2) Console.Write("{0:x02} ", recls.ihrs_records[i - 1].ihr_data[2]);
                    if (recls.ihrs_records[i - 1].ihr_data.Length > 3) Console.Write("{0:x02} ", recls.ihrs_records[i - 1].ihr_data[3]);
                    if (recls.ihrs_records[i - 1].ihr_data.Length > 4) Console.Write("{0:x02} ", recls.ihrs_records[i - 1].ihr_data[4]);
                    if (recls.ihrs_records[i - 1].ihr_data.Length > 5) Console.Write("{0:x02} ", recls.ihrs_records[i - 1].ihr_data[5]);
                    if (recls.ihrs_records[i - 1].ihr_data.Length > 6) Console.Write("{0:x02} ", recls.ihrs_records[i - 1].ihr_data[6]);
                    if (recls.ihrs_records[i - 1].ihr_data.Length > 7) Console.Write("{0:x02} ", recls.ihrs_records[i - 1].ihr_data[7]);
                    Console.Write("\n");
                }
                */
                idx += (int)(recls.ihrs_records[i - 1].ihr_length * 2) + 10;

                while ((idx < size) && (data[idx] != IHEX_CHR_RECORDMARK) && (data[idx] != 0x00))
                {
                    idx++; //data ++;
                }
            }
            /*
            while (data < end && *(data) != 0x00)
            {
                i ++;
		
                if (data + 3 >= end)
                {
                    break;
                }

                l = ihex_fromhex8(((Byte []) data) + 1);
                if (data + 9 + l * 2 >= end ||
                    (r = ihex_parse_single_record((Byte []) data, l, rec + i - 1)) != 0)
                {
                    IHEX_SET_ERROR(r, "Line %i: %s", i, ihex_last_error);
                    goto parse_single_failed;
                }
		
                data += (rec[i - 1].ihr_length * 2) + 10;
                while (data < end && *(data) != IHEX_CHR_RECORDMARK && *(data) != 0x00)
                {
                    data ++;
                }
            }
	        */

            //-----------------------------------------------------------------
            // Finally check that the last record is the IHEX_EOF record.
            if (recls.ihrs_records[recls.ihrs_count - 1].ihr_type != ihex_rtype_t.IHEX_EOF)
            {
                //TODO IHEX_SET_ERROR(IHEX_ERR_NO_EOF, "Missing EOF record");
                goto missing_eof_record;
            }

            return recls;

        missing_eof_record:
        //for (i = 0; i < recls.ihrs_count; i ++)
        //{
        //    free(recls.ihrs_records[i].ihr_data);
        //}
        parse_single_failed:
        //free(recls);
        //malloc_recls_failed:
        //free(rec);
        //malloc_rec_failed:

            return null;
        }


        //---------------------------------------------------------------------
        //TODO  static ihex_recordset_t ihex_rs_from_string(String data)
        //TODO  {
        //TODO      return ihex_rs_from_mem(data, strlen(data));
        //TODO  }

        //---------------------------------------------------------------------
        static int ihex_parse_single_record(Byte[] data, UInt32 _ofst, UInt32 length, ihex_record_t record)
        {
            UInt32 i;

            // Records needs to begin with record mark (usually ":")
            if (data[_ofst] != IHEX_CHR_RECORDMARK)
            {
                //IHEX_SET_ERROR_RETURN(IHEX_ERR_PARSE_ERROR, "Missing record mark");
                return (IHEX_ERR_PARSE_ERROR);
            }

            // Record layout:
            //               1         2         3         4
            // 0 12 3456 78 90123456789012345678901234567890 12
            // : 10 0100 00 214601360121470136007EFE09D21901 40

            //   +3  +7+9                       +9+2*len|
            // 1  1   1 0123456789abcdef0123456789abcdef|                                   
            // 1  4   8                                 V    
            // :10800000E0570020F58001000F810100118101007F --karels
            record.ihr_length = (Byte)length;
            record.ihr_address = (UInt16)ihex_fromhex16(data, (int)(_ofst + 3));
            record.ihr_type = (ihex_rtype_t)ihex_fromhex8(data, (int)(_ofst + 7));
            record.ihr_checksum = (Byte)ihex_fromhex8(data, (int)(_ofst + 9 + record.ihr_length * 2));

            record.ihr_data = new Byte[record.ihr_length];
            /*
            if ((record.ihr_data = (Byte []) malloc(record.ihr_length)) == NULL)
            {
                //IHEX_SET_ERROR_RETURN(IHEX_ERR_MALLOC_FAILED, "Could not allocate memory");
                return(IHEX_ERR_MALLOC_FAILED);
            }
            */

            // Records needs to end with CRLF or LF.
            if ((data[_ofst + 11 + record.ihr_length * 2] != 0x0D || data[_ofst + 12 + record.ihr_length * 2] != 0x0A)
                && (data[_ofst + 11 + record.ihr_length * 2] != 0x0A))
            {
                //free(record.ihr_data);
                //IHEX_SET_ERROR_RETURN(IHEX_ERR_WRONG_RECORD_LENGTH, "Incorrect record length");
                return (IHEX_ERR_WRONG_RECORD_LENGTH);
            }

            for (i = 0; i < record.ihr_length; i++)
            {
                if (data[_ofst + 9 + i * 2] == 0x0A || data[_ofst + 9 + i * 2] == 0x0D)
                {
                    //free(record.ihr_data);
                    //IHEX_SET_ERROR_RETURN(IHEX_ERR_WRONG_RECORD_LENGTH, "Unexpected end of line");
                    return (IHEX_ERR_WRONG_RECORD_LENGTH);

                }
                record.ihr_data[i] = ihex_fromhex8(data, (int)(_ofst + 9 + i * 2));
            }

            if (ihex_check_record(record) != 0)
            {
                //free(record.ihr_data);
                //IHEX_SET_ERROR_RETURN(IHEX_ERR_INCORRECT_CHECKSUM, "Checksum validation failed");
                return (IHEX_ERR_INCORRECT_CHECKSUM);

            }

            return 0;
        }


        //---------------------------------------------------------------------
        static int ihex_check_record(ihex_record_t r)
        {
            UInt32 i;
            //Byte t = 0;
            Int64 t = 0;

            t += r.ihr_length + ((r.ihr_address >> 8) & 0xFF)
                 + (r.ihr_address & 0xFF)
                 + (Byte)(r.ihr_type)
                 + r.ihr_checksum;

            for (i = 0; i < r.ihr_length; i++)
            {
                t += r.ihr_data[i];
            }

            return ((t & 0xFF) == 0) ? 0 : 1;
        }

        //---------------------------------------------------------------------
        UInt32 ihex_errno()
        {
            return ihex_last_errno;
        }

        //---------------------------------------------------------------------
        String ihex_error()
        {
            return ihex_last_error;
        }

        //---------------------------------------------------------------------
        void ihex_set_error(UInt32 errnum, String error)
        {
            ihex_last_errno = errnum;
            ihex_last_error = error;

            //#ifdef IHEX_DEBUG
            //printf(error);
            //#endif
        }

        //---------------------------------------------------------------------
        static Byte ihex_fromhex4(Byte i)
        {
            if (i >= 0x61 && i <= 0x66) return ((Byte)(i - 0x61 + 10));
            else if (i >= 0x41 && i <= 0x46) return ((Byte)(i - 0x41 + 10));
            else if (i >= 0x30 && i <= 0x39) return ((Byte)(i - 0x30));
            else return 0;
        }

        //---------------------------------------------------------------------
        static Byte ihex_fromhex8(Byte[] i, int _ofst)
        {
            return ((Byte)((ihex_fromhex4(i[_ofst + 0]) << 4) + ihex_fromhex4(i[_ofst + 1])));
        }

        //---------------------------------------------------------------------
        static UInt16 ihex_fromhex16(Byte[] i, int _ofst)
        {
            return ((UInt16)((ihex_fromhex4(i[_ofst + 0]) << 12) +
                              (ihex_fromhex4(i[_ofst + 1]) << 8) +
                              (ihex_fromhex4(i[_ofst + 2]) << 4) +
                               ihex_fromhex4(i[_ofst + 3])
                            )
                  );
        }
        //========== ihex_parse.c ==========
        //========== ihex_parse.c ==========
        //========== ihex_parse.c ==========




        //========== ihex_record.c ==========
        //========== ihex_record.c ==========
        //========== ihex_record.c ==========
        /*
#define IHEX_SET_ERROR(errnum, error, ...) \
	{ char *e = (char*)malloc(512); \
	  snprintf(e, 512, error, ##__VA_ARGS__); \
	  ihex_set_error(errnum, e); }
#define IHEX_SET_ERROR_RETURN(errnum, error, ...) \
	{ IHEX_SET_ERROR(errnum, error, ##__VA_ARGS__); \
	  return errnum; }
        */

        static public UInt64 ihex_rs_get_size(ihex_recordset_t rs)
        {
            UInt64 s = 0;
            UInt32 i = 0;

            for (i = 0; i < rs.ihrs_count; i++)
            {
                s += rs.ihrs_records[i].ihr_length;
            }

            return s;
        }


        static int ihex_rs_iterate_data(ihex_recordset_t rs, ref UInt32 pIdx, /* ihex_record_t**rec*/ ref ihex_record_t pRec, ref UInt32 pOff)
        {
            UInt32 offset;

            ihex_record_t x;

            if (pIdx == 0 && pOff != 0)
            {
                pOff = 0; // force the offset to zero at start
            }

            for (; pIdx < rs.ihrs_count; ++pIdx)
            {
                x = (rs.ihrs_records[pIdx]);
                if (pRec != null)
                {
                    pRec = x;	//point the caller to current record
                }

                switch (x.ihr_type)
                {
                    case ihex_rtype_t.IHEX_DATA:
                        ++pIdx;		//proceed to next record in next call
                        return 0;	//let the caller process the data

                    case ihex_rtype_t.IHEX_EOF:
                        if (pIdx < rs.ihrs_count - 1)
                        {
                            //IHEX_SET_ERROR_RETURN(IHEX_ERR_PREMATURE_EOF,  "Premature EOF in record %i", p__i + 1);
                            return (IHEX_ERR_PREMATURE_EOF);
                        }
                        else
                        {
                            //FIXME signal end of records
                            pIdx = 0; //signal next index is 0 => we've finished
                            if (pRec != null)
                            {
                                pRec = null;
                            }
                            return 0;
                        }
                    case ihex_rtype_t.IHEX_ESA:
                        //offset = *(x.ihr_data) << 4; //?????????????????????????????????
                        offset = x.ihr_data[0];// << 4;
                        offset = offset << 4;
                        if (pOff != null)
                        {
                            pOff = offset;
                        }

                        //#ifdef IHEX_DEBUG
                        //printf("Switched offset to 0x%08x.\n", offset);
                        //#endif

                        break;

                    case ihex_rtype_t.IHEX_ELA:
                        offset = (UInt32)((x.ihr_data[0] << 24) + (x.ihr_data[1] << 16));
                        if (pOff != null)
                        {
                            pOff = offset;
                        }

                        //#ifdef IHEX_DEBUG
                        //printf("Switched offset to 0x%08x.\n", offset);
                        //#endif				
                        break;

                    case ihex_rtype_t.IHEX_SSA:
                    case ihex_rtype_t.IHEX_SLA:
                        //skip body; next
                        break;

                    default:
                        //IHEX_SET_ERROR_RETURN(IHEX_ERR_UNKNOWN_RECORD_TYPE, "Unknown record type in record %i: 0x%02x",  p__i + 1, x.ihr_type);
                        break;
                }
            }

            //IHEX_SET_ERROR_RETURN(IHEX_ERR_NO_EOF, "Missing EOF record");
            return (IHEX_ERR_NO_EOF);
        }




        static public int ihex_rs_get_address_range(ihex_recordset_t rs, ref UInt32 pmin, ref UInt32 pmax)
        {
            int r;
            UInt32 offset = 0x00, address = 0x00;//, dummy_min, dummy_max;
            UInt32 min, max;

            UInt32 i = 0;
            ihex_record_t x = new ihex_record_t();

            // Initialize range boundaries
            //if (pmin == null)
            //{
                //min = dummy_min;
            //}
            //if (pmax == null)
            //{
                //max = dummy_max;
            //}

            min = 0xFFFFFFFF; // UINT32_MAX;
            max = 0x00;
            int dd = 0;
            uint lasti = 0;
            do
            {
                lasti = i;
                r = ihex_rs_iterate_data(rs, ref i, ref x, ref offset);
                if (i == 0)
                {
                    Console.Write("Last  i = {0}, ", lasti);
                }
                if (r != 0)
                {
                    return r;
                }
                else if (x == null)
                {
                    break;
                }

                if (dd == 0)
                {
                    Console.Write("First i = {0}, ", i);
                    dd++;
                }

                address = (offset + x.ihr_address);

                if (address < min)
                {
                    min = address;
                }
                /*
                if (address + x.ihr_length > max)
                {
                    max = address + x.ihr_length;
                }
                */
                if (address /*+ x.ihr_length*/ > max)
                {
                    max = address + (x.ihr_length - 1);
                }
            }
            while (i > 0);

            if (pmin != null) pmin = min;
            if (pmax != null) pmax = max;

            return 0;
        }


        /*
        void ihex_rs_free(ihex_recordset_t* rs)
        {
            UInt32 i = 0;
	
            if (rs == NULL)
            {
                return;
            }
	
            if (rs.ihrs_records != NULL)
            {
                for (i = 0; i < rs.ihrs_count; i++)
                {
                    free(rs.ihrs_records[i].ihr_data);
                }
            }
	
            free(rs.ihrs_records);
            free(rs);
        }
        */


        //========== ihex_record.c ==========
        //========== ihex_record.c ==========
        //========== ihex_record.c ==========

    }
    //=====================================================================
    //=====================================================================
    //=====================================================================
    class ks_ihex_LoadToMem
    {
        static public Int32 W_ihex_LoadToMem(W_ihexMemImage_t pI, String filename)
        {
            int i;
            UInt16 crc16;
            intelStuff.ihex_recordset_t rs;
            int rv;
            UInt64 offset; //UInt64
            UInt64 count;  //UInt64

            Console.Write("\nW_ihexLoadToMem\n");

            //rs  = ihex_rs_from_file("C:/ble_nrf51/tandd/nrf51/examples/ble_peripheral/ble_app_K1__/pca10028/s110/arm4/_build/nrf51422_xxac_s110.hex");
            //rs  = ihex_rs_from_file("C:/ble_nrf51/tandd/nrf51/examples/ble_peripheral/ble_app_K2/pca10028/s110/arm4/_build/nrf51422_xxac_s110.hex");

            //----- Step 1: Load the file into the rs Record Set array -----
            rs = intelStuff.ihex_rs_from_file(filename);
            if (rs == null)
            {
                Console.Write("Failed to get records from hex file\n");
                return (-1);
            }

            //----- Setep ?: get total bye count of all data in record set ? -----
            /*
            pI._gx_size = (UInt32)intelStuff.ihex_rs_get_size(rs); //Note gets total size of all records
            Console.Write("  ihex_rs_get_size() . _gx_size = {0}\n", pI._gx_size);
            if (pI._gx_size > pI.gx_binCapacity)
            {
                Console.Write("  Image File is too big for alotted memory. Image size = {0}, Memory size = {1}\n\n", pI._gx_size, pI.gx_binCapacity);
                return (-1);
            }
            */

            //----- Step ?: Get the address range -----
            rv = intelStuff.ihex_rs_get_address_range(rs, ref pI.image_start, ref pI.image_stop);
            if (rv != 0)
            {
                Console.Write("  ihex_rs_get_address_range() rv = {0}, . image_start = 0x{1:x08}, . image_stop = 0x{2:x08}\n", rv, pI.image_start, pI.image_stop);
                return (rv);
            }


            //rv = ihex_mem_copy(rs, dst, 0x30000/*8192*/, IHEX_WIDTH_8BIT, IHEX_ORDER_BIGENDIAN);

            offset = 0;
            count = (pI.image_stop + 1) - pI.image_start;
            //pI.image_array = new Byte[count]; //Real solution
            pI.image_array = new Byte[count + 256]; //Room to fiddle arout

            rv = intelStuff.ihex_mem_copy_from_offset(rs, ref pI.image_array, offset, count, intelStuff.ihex_width_t.IHEX_WIDTH_8BIT, intelStuff.ihex_byteorder_t.IHEX_ORDER_BIGENDIAN);
            //Console.Write("  ihex_mem_copy_from_offset() rv = {0}\n", rv);

            /* testing
            offset = 0x08;
            count = 0x10;
            rv = ihex_mem_copy_from_offset(rs, pI.image_array, offset, count, IHEX_WIDTH_8BIT, IHEX_ORDER_BIGENDIAN);
            Console.Write("rv = {0}\n", rv);

            offset = 0x2744;
            count = 0x100;
            rv = ihex_mem_copy_from_offset(rs, pI.image_array, offset, count, IHEX_WIDTH_8BIT, IHEX_ORDER_BIGENDIAN);
            Console.Write("rv = {0}\n", rv);
            */

            /**/
            //testing - start
            pI.image_length = (pI.image_stop + 1) - pI.image_start; // Real
            pI.image_length -= 5;
            for (i = 0; i < 10; i++)
            {
                pI.image_length++;
                crc16 = 0xFFFF;
              //crc16 = dfu_crc16.crc16_compute(pI.image_array, (UInt16)pI.image_length, crc16);// = 0xffff);
                crc16 = dfu_crc16.calc_crc16(pI.image_array, crc16);
                Console.Write("                             image_length = {0} (0x{1:x}), data[{2:x04}] = 0x{3:x02},  crc16={4:x04} \n", pI.image_length, pI.image_length, pI.image_length - 1, pI.image_array[pI.image_length - 1], crc16);
            }
            //testing - start
            /**/


            // real
            pI.image_length = (pI.image_stop + 1) - pI.image_start; // Real
            crc16 = 0xFFFF;
          //crc16 = dfu_crc16.crc16_compute(pI.image_array, (UInt16)pI.image_length, crc16);// = 0xffff);
            crc16 = dfu_crc16.calc_crc16(pI.image_array, crc16);
            pI.image_crc = crc16;
            Console.Write("\n");
            Console.Write("  (image_stop - image_start) image_length = {0} (0x{1:x}), data[{2:x04}] = 0x{3:x02},  crc16={4:x04} \n", pI.image_length, pI.image_length, pI.image_length-1, pI.image_array[pI.image_length-1], pI.image_crc);
            Console.Write("\n");

            return (0);
        }

    }
}
