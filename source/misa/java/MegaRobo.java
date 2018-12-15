
package mrq;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
// import java.net.ServerSocket;
import java.net.Socket;

public class MegaRobo extends Socket 
{
    //! static status
    static int STATE_UNK = -1;
    static int STATE_IDLE = 0;
    static int STATE_RUN_REQED = 1;
    static int STATE_PROGRAM = 2;
    static int STATE_CALCLING = 3;

    static int STATE_CALCEND = 4;
    static int STATE_STANDBY = 5;
    static int STATE_PRERUN = 6;
    static int STATE_RUNNING = 7;

    static int STATE_PRESTOP = 8;

    //! members
    protected String mName;
 
    protected OutputStream mOutStream;
    protected PrintWriter mPrintWriter;

    protected InputStream mInStream;
    protected BufferedReader mInReader;

    public MegaRobo( String name, String ip, int socket ) throws IOException
    {
        super( ip, socket ); 
        
        setSoTimeout( 2000 );

        //! stream
        mOutStream = getOutputStream(); 
        mPrintWriter = new PrintWriter( mOutStream );
        
        mInStream = getInputStream();
        mInReader = new BufferedReader(  new InputStreamReader( mInStream ) );

        //! set name
        this.write( name );
    }
    
    synchronized public int write( String cmd )
    {
        mPrintWriter.print( cmd + "\r\n" );
        mPrintWriter.flush();
        
        return 0;
    }

    synchronized public String read( ) throws IOException
    { 
        return mInReader.readLine();
    }

    public int write( String cmd, int ax, int page )
    {
        return write( String.format( "%s %d,%d" ), ax, page );
    }

    //! program
    public int program( int ax, int page, String strfile )
    { 
        return write( String.format("PROGRAM %d,%d,\"%s\"", ax, page, strfile ) );
    }

    public int call( int ax, int page )
    { 
        return write( String.format("CALL %d,%d", ax, page ) );
    }

    public int stop( int ax, int page )
    { 
        return write( String.format("STOP %d,%d", ax, page ) );
    }

    public int qStatus( int ax, int page ) throws IOException
    { 
        //! write
        write( String.format("STATE? %d,%d", ax, page ) );

        //! receive
        String info = read();
        if ( info == null )
        { throw new IOException(); }
        else
        { return Integer.valueOf(info); }
    }

    //! operation
    public int center( int ax, int page )
    {
        return write( "CENTER", ax, page );
    }

    //! wait for
    public int waitFor( int stat, int ax, int page, int tmo, int tickms ) throws IOException, InterruptedException
    {
        while( tmo > 0 )
        {
            Thread.sleep( tickms );

            if ( qStatus(ax, page) == stat )
            { return 0; }
        }

        return -1;
    }

    //! wait end
    public int waitEnd( int ax, int page, int tmo, int tickms ) throws IOException, InterruptedException
    {
        return waitFor(  STATE_CALCEND, ax, page, tmo, tickms);
    }

    //! wait idle
    public int waitIdle( int ax, int page, int tmo, int tickms ) throws IOException, InterruptedException
    {
        return waitFor(  STATE_IDLE, ax, page, tmo, tickms);
    }

    public int waitEnd( int ax, int page, int tmo) throws IOException, InterruptedException
    {
        return waitEnd( ax, page, tmo, 500 );
    }

    public int waitIdle( int ax, int page, int tmo ) throws IOException, InterruptedException
    {
        return waitIdle(  ax, page, tmo, 500 );
    }

    public int waitEnd( int ax, int page) throws IOException, InterruptedException
    {
        return waitEnd(  ax, page, 60000 );
    }

    public int waitIdle( int ax, int page ) throws IOException, InterruptedException
    {
        return waitIdle( ax, page, 60000 );
    }
}