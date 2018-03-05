# -*- coding=utf-8 -*-

# step1
import mrq as MRQ

# step2 
# create the device
myMrq = MRQ.MRQ( "device1" )

# step3
# read the idn
idn = myMrq.IDN 
print( idn )

# step4 
# control the device
myMrq.IDENTITY_DISTDEVICE = "ON"

# get the value
idDist = myMrq.IDENTITY_DISTDEVICE
print( idDist )

# control the motor
# loop
for i in range( 1, 100 ):
    myMrq.roate( 0, 360 )
    myMrq.wait_idle( 0 )

    myMrq.roate( 0, -360 )
    myMrq.wait_idle( 0 )
