#include "deltamotiongroup.h"

DeltaMotionGroup::DeltaMotionGroup( const QString &className,
                                    const QString &name ) : SinanjuMotionGroup( className, name )
{
    mExportOptList.clear();
    mExportOptList<<"t-x-z"<<"t-y-hand";

    mSectionAble.clear();
    mSectionAble<<true<<true;

    mbStepAble = true;
    mbPrefAble = false;
}
