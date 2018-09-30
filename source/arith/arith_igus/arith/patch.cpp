int getPos(igusConfig &cfg,
			float p[3], 
                        float xyz[3] )
{
        //! set para
        armLengthArray = cfg.armLength;
        sca = cfg.scal;
        vMax = cfg.vM;
        posLim = cfg.posLim;
        for (int i = 0; i < 2; i++)
        {
                deltArm[i] = cfg.offset[i];
        }

	double pIn[3], pOut[3];
	
	//! copy data
	for ( int i = 0; i < 3; i++ )
	{
                pIn[i] = p[i] + posLim[0];
	}

	int ret;
	ret = CalPosition( pIn, pOut, 0 );
	if ( ret != 0 )
	{ return ret; }

	//! shift in 
        //! double to float
        double dVal;
	for ( int i = 0; i < 3; i++ )
	{
            dVal = pOut[i] + cfg.P0[i];
            xyz[i] = double_to_float( dVal );
	}
	
	return 0;
}
