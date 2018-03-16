import socket as SOCKET
import time as TIME 

class MRQ(object):
    #_mName = ""
    #_mSocket = None

    # timeout = 1s
    _timeout_tick = 1

    def __init__( self, deviceName, ip="127.0.0.1",port=1234 ):
        self._mName = deviceName
        self._mSocket = SOCKET.socket() 

        # connect
        ipPort = ( ip, port )
        self._mSocket.connect( ipPort )

        self.send( deviceName )

    # send to device
    def send( self, cmd ):
        self._mSocket.send( cmd + "\n" )
        # print(cmd)

    # recv from device        
    def recv( self ):
        return self._mSocket.recv( 1024 )
        # return "abc"

    # send and receive
    def query( self, cmdq ):
        self.send( cmdq )
        return self.recv()        

    @property
    def IDN( self ):
        return self.query("*IDN?")

    @property 
    def IDENTITY_DISTDEVICE( self ):
        return self.query( "IDENTITY:DISTDEVICE?" )

    @IDENTITY_DISTDEVICE.setter
    def IDENTITY_DISTDEVICE( self, value ):
        self.send( "IDENTITY:DISTDEVICE " + value )

    def roate( self, ch, angle, t = 0.1 ):
        fmtStr = "ROTATE %d,%g,%g" % ( ch, t, angle )
        self.send( fmtStr )

    def MOTION_STATE( self, ch ):
        fmtStr = "MOTION:STATE? %d" % ( ch )
        return self.query( fmtStr )      

    # state value
    # 0 -- idle
    def FSM_STATE( self, ch ):
        fmtStr = "FSMSTATE? %d" % ( ch )
        stateStr = self.query( fmtStr )      
        return int(stateStr)

    # timeout = 20s    
    def wait_idle( self, ch, tmo = 20.0 ):
        while( tmo > 0 ):
            TIME.sleep( self._timeout_tick )
            tmo = tmo - self._timeout_tick
            
            state = self.FSM_STATE( ch )
            if ( state == 0 ):
                return 0
        return 1                




if __name__ == '__main__':
    mrq = MRQ( "seq1" )
    idn = mrq.IDN
    print(idn)
    mrq.IDENTITY_DISTDEVICE = "ON"
    mrq.IDENTITY_DISTDEVICE = "OFF"