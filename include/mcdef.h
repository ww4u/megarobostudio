#ifndef _MC_DEF_H_
#define _MC_DEF_H_

#define MATH_PI     (3.1415926)
#define RAD_TO_DEG( rad )       ( ( rad )*180/MATH_PI )
#define DEG_TO_RAD( deg )       ( deg * MATH_PI/180 )

#define sizeof_array( array )	 	(int)( sizeof(array)/sizeof(array[0]) )

#define sizeof_double( item )           (int)( sizeof(item)/sizeof(double) )
#define offsetof_double( a, b )         (int)( offsetof( a, b )/sizeof(double) )

#define checked_call( call )            ret = call;\
                                        if ( ret != 0 ){ return ret; }

#define gc_array( ary )                 if ( NULL != ary ){ delete []ary; ary = NULL; }
#define gc_array2( ary1, ary2 )         gc_array( ary1 );gc_array( ary2 );
#define gc_array3( ary1, ary2, ary3 )   gc_array2( ary1, ary2 ); gc_array( ary3 );

#define gc_item( item )			if ( NULL != item ){ delete item; item = NULL; }

#define time_us( us )    (us)
#define time_ms( ms )    ((ms)*time_us(1000))
#define time_s( s )      ((s)*time_ms(1000))


#define has_attr( val, attr )       ( ( (val) & (attr) ) == (attr) )

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
