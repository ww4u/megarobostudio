
#include "slide.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

robotSlide::robotSlide()
{
    mClass = "Slide";
    mId = robot_slide;

    //! default gpid
    mCanGroupId = group_id_from + mId - robot_complex;

    setAxes( 1 );
    setAxesDefName( 1 );
    setJointName( 1 );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Slide");

    //! debug used
    //! alter the axes name
    logDbg()<<mAxesConnectionName.size();
    Q_ASSERT( mAxesConnectionName.size() == 1 );
    mAxesConnectionName[0] = "CH1@device1"; //! slider
}




