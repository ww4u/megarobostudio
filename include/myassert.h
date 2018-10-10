#ifndef MYASSERT_H
#define MYASSERT_H

//#include <QtGlobal>

#define MEGA_ASSERT( cond )     Q_ASSERT_X( (cond), \
                                            __FILE__, \
                                            QString("%1 %2").arg( __FUNCTION__ ).arg( __LINE__ ).data() \
                                            )

#endif // MYASSERT_H
