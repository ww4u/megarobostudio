#include "deltamotiongroup.h"

DeltaMotionGroup::DeltaMotionGroup( const QString &className,
                                    const QString &name ) : SinanjuMotionGroup( className, name )
{
    mExportOptList.clear();
    if ( str_is(className,"mrx-dt") )
    { mExportOptList<<"t-x-z"<<"t-y-hand"; }
    else
    { mExportOptList<<"t-x-y-z"<<"t-hand"; }

    mSectionAble.clear();
    mSectionAble<<true<<true;

    mbStepAble = true;
    mbPrefAble = false;
}
