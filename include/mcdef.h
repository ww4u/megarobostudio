#ifndef _MC_DEF_H_
#define _MC_DEF_H_

#define MATH_PI     (3.1415926)
#define RAD_TO_DEG( rad )       ( ( rad )*180/MATH_PI )
#define DEG_TO_RAD( deg )       ( (deg) * MATH_PI/180 )

#define SINdeg( deg )           qSin( DEG_TO_RAD(deg) )
#define COSdeg( deg )           qCos( DEG_TO_RAD(deg) )

#define sizeof_array( array )	 	(int)( sizeof(array)/sizeof(array[0]) )
#define i_sizeof( a )                   (int)( sizeof(a) )

#define MEMCPY( dst, src, cnt )         memcpy( dst, src, cnt )
#define rMEMCPY( src, dst, cnt )        memcpy( dst, src, cnt )

#define sizeof_double( item )           (int)( sizeof(item)/sizeof(double) )
#define offsetof_double( a, b )         (int)( offsetof( a, b )/sizeof(double) )

#define str_is( a, b )          ( QString::compare( (a),(b), Qt::CaseInsensitive) == 0 )
#define str_is_2( a, b1, b2 )   ( str_is(a,b1) || str_is(a,b2) )
#define str_is_4( a, b1, b2, b3, b4 )   ( str_is_2(a,b1,b2) || str_is_2(a,b3,b4) )

#define str_equ( a, b )         ( QString::compare( (a),(b), Qt::CaseSensitive) == 0 )

//! bit ops
#define set_bit( val, bitN )     val |= (1<<(bitN));
#define unset_bit( val, bitN )     val &= (~(1<<(bitN)));
#define is_bit1( val, bitN )     ( (val & (1<<(bitN)) ) == (1<<(bitN)) )
#define is_bit0( val, bitN )     ( (val & (1<<(bitN)) ) != (1<<(bitN)) )

#define is_mask( val, mask )     ( ( (val) & (mask) ) == (mask) )

#define checked_call( call )            ret = call;\
                                        if ( ret != 0 ){ return ret; }

#define checked_call_x( call, n, r )            ret = call;\
                                        if ( ret != n ){ return r; }

#define gc_array( ary )                 if ( NULL != ary ){ delete []ary; ary = NULL; }
#define gc_array2( ary1, ary2 )         gc_array( ary1 );gc_array( ary2 );
#define gc_array3( ary1, ary2, ary3 )   gc_array2( ary1, ary2 ); gc_array( ary3 );

#define gc_item( item )			if ( NULL != item ){ delete item; item = NULL; }

#define time_us( us )    (us)
#define time_ms( ms )    ((ms)*time_us(1000))
#define time_s( s )      ((s)*time_ms(1000))


#define has_attr( val, attr )       ( ( (val) & (attr) ) == (attr) )

#define double_to_float( dv )       ( ( (dv) > -FLT_MIN && (dv) < FLT_MIN ) ? (0) : (dv) )

#define delete_all( container )     { qDeleteAll( container ); container.clear(); }


//! sync status
#define wait_sth( ret, cond, tmo, tick ) \
                                    ret = 0;  \
                                    { \
                                        int _ltmous = tmo; \
                                        \
                                        do{ \
                                        \
                                            if ( cond ) { ret = 0; break; } \
                                            QThread::usleep( tick ); \
                                        }while( (  _ltmous-=(tick), ret = -1, _ltmous > 0) );\
                                    }

#define wait_on( ret, cond )             wait_sth( ret, cond, 10000000, 10000 )

#endif
