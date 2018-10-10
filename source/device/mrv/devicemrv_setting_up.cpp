#include "devicemrv.h"

namespace MegaDevice {

//! upload all data
int deviceMRV::_uploadSetting()
{
    int ret = -1;
MRV_PROGRESS( 0, ("LINK") );

    checked_call( getLINK_INTFC(&mLINK_INTFC) );
    checked_call( getSYSTEM_POWERON(&mSYSTEM_POWERON));

    checked_call( getRS232_BAUD(&mRS232_BAUD) );
    checked_call( getRS232_WORDLEN(&mRS232_WORDLEN) );
    checked_call( getRS232_FLOWCTL(&mRS232_FLOWCTL) );
    checked_call( getRS232_PARITY(&mRS232_PARITY) );

    checked_call( getRS232_STOPBIT(&mRS232_STOPBIT) );
    checked_call( getCAN_TYPE(&mCAN_TYPE) );
    checked_call( getCAN_BAUD(&mCAN_BAUD) );
    checked_call( getCAN_GROUP(&mCAN_GROUP) );

    checked_call( getCAN_NETMANAGESTATE(&mCAN_NETMANAGESTATE) );
    checked_call( getCAN_NETMANAGEHASH(&mCAN_NETMANAGEHASH) );

MRV_PROGRESS( 10, ("GLOBAL") );
    for ( int i = 0; i < axes(); i++ )
    {
        checked_call( getGLOBAL_GROUP( i, MRV_GLOBAL_GROUP_GROUP1, &mGLOBAL_GROUP[i][0] ) );
        checked_call( getGLOBAL_GROUP( i, MRV_GLOBAL_GROUP_GROUP2, &mGLOBAL_GROUP[i][1] ) );

        checked_call( getGLOBAL_DISTINGUISH( i, &mGLOBAL_DISTINGUISH[i] ) );
    }

MRV_PROGRESS( 20, ("VALVE") );
    checked_call( getVALVECTRL_PWMFREQ( &mVALVECTRL_PWMFREQ ) );
    for ( int i = 0; i < axes(); i++ )
    {
        checked_call( getVALVECTRL_DEVICE( i, mVALVECTRL_DEVICE+i ) );
        checked_call( getVALVECTRL_RTDUTY( i, mVALVECTRL_RTDUTY+i ) );
        checked_call( getVALVECTRL_ACTION( i, mVALVECTRL_ACTION+i ) );
        checked_call( getVALVECTRL_IDLEDUTY( i, mVALVECTRL_IDLEDUTY + i ) );

        checked_call( getVALVECTRL_HOLDDUTY( i, mVALVECTRL_HOLDDUTY + i ) );
        checked_call( getVALVECTRL_OPENDUTY( i , mVALVECTRL_OPENDUTY + i ) );
        checked_call( getVALVECTRL_OPENDLYTIME( i, mVALVECTRL_OPENDLYTIME + i ) );
        checked_call( getVALVECTRL_OPENTIME( i, mVALVECTRL_OPENTIME + i ) );

        checked_call( getVALVECTRL_RECORDSTATE( i, mVALVECTRL_RECORDSTATE + i ) );
        checked_call( getVALVECTRL_ONNUMS( i, mVALVECTRL_ONNUMS + i ) );
        checked_call( getVALVECTRL_OFFNUMS( i, mVALVECTRL_OFFNUMS + i ) );
        checked_call( getVALVECTRL_TIME( i, mVALVECTRL_TIME + i ) );
    }

MRV_PROGRESS( 30, "PVT" );
    for ( int i = 0; i < axes(); i++ )
    {
        checked_call( getPVT_EXECMODE( i, mPVT_EXECMODE+i ) );
        checked_call( getPVT_CYCLES( i, mPVT_CYCLES+i ) );
    }

MRV_PROGRESS( 40, "THRESHOLD" );
    for ( int i=0; i < axes(); i++ )
    {
        checked_call( getTHRESHOLD_HIGHPRESSURE( i, mTHRESHOLD_HIGHPRESSURE + i ) );
        checked_call( getTHRESHOLD_HIPRESSUREACTION( i, mTHRESHOLD_HIPRESSUREACTION + i ) );
        checked_call( getTHRESHOLD_LOWPRESSURE( i, mTHRESHOLD_LOWPRESSURE + i ) );
        checked_call( getTHRESHOLD_LOWPRESSUREACTION( i, mTHRESHOLD_LOWPRESSUREACTION + i ) );

        checked_call( getTHRESHOLD_HIGHCURRENT( i, mTHRESHOLD_HIGHCURRENT + i ) );
        checked_call( getTHRESHOLD_HICURRENTACTION( i, mTHRESHOLD_HICURRENTACTION + i ) );
        checked_call( getTHRESHOLD_LOWCURRENT( i, mTHRESHOLD_LOWCURRENT + i ) );
        checked_call( getTHRESHOLD_LOWCURRENTACTION( i, mTHRESHOLD_LOWCURRENTACTION + i ) );

        checked_call( getTHRESHOLD_ONNUMS( i, mTHRESHOLD_ONNUMS + i ) );
        checked_call( getTHRESHOLD_NUMSACTION( i, mTHRESHOLD_NUMSACTION + i ) );
        checked_call( getTHRESHOLD_TIME( i, mTHRESHOLD_TIME + i ) );
        checked_call( getTHRESHOLD_TIMEACTION( i, mTHRESHOLD_TIMEACTION + i ) );
    }

MRV_PROGRESS( 50, "IOCONFIG" );
    for ( int i = 0; i < axes(); i++ )
    {
        checked_call( getIOCONFIG_IOFUNCSEL( i, mIOCONFIG_IOFUNCSEL + i ) );
        checked_call( getIOCONFIG_SWFILTERDELAY( i, mIOCONFIG_SWFILTERDELAY + i ) );
        checked_call( getIOCONFIG_SWFILTERCOUNTER( i, mIOCONFIG_SWFILTERCOUNTER + i ) );
        checked_call( getIOCONFIG_TRIGEDGE( i, mIOCONFIG_TRIGEDGE + i ) );

        checked_call( getIOCONFIG_BAUD( i, mIOCONFIG_BAUD + i ) );
        checked_call( getIOCONFIG_WORDLEN( i, mIOCONFIG_WORDLEN + i ) );
        checked_call( getIOCONFIG_FLOWCTL( i, mIOCONFIG_FLOWCTL + i ) );
        checked_call( getIOCONFIG_PARITY( i, mIOCONFIG_PARITY + i ) );

        checked_call( getIOCONFIG_STOPBIT( i, mIOCONFIG_STOPBIT + i ) );
    }

    return 0;
}

}
