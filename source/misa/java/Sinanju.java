package mrq;

import java.io.IOException;

public class Sinanju extends MegaRobo
{
    public Sinanju( String name, String ip, int socket ) throws IOException
    {
        super( name, ip, socket );
    }

    public Sinanju( String name, String ip ) throws IOException
    {
        this( name, ip, 1234 );
    }

    public Sinanju( String name ) throws IOException
    {
        this( name, "127.0.0.1" );
    }

    public int align( int ax, int page )
    { return write( "ALIGN", ax, page ); }

    public int fold( int ax, int page )
    { return write( "FOLD", ax, page ); }

    public int stepX( int ax, int page, float dx, float dt )
    { 
        return write( String.format("STEP:X %d,%d,-1,%g,%g", ax, page, dx, dt ) );
    }

    public int stepY( int ax, int page, float dy, float dt )
    { 
        return write( String.format("STEP:Y %d,%d,-1,%g,%g", ax, page, dy, dt ) );
    }

    public int stepZ( int ax, int page, float dz, float dt )
    { 
        return write( String.format("STEP:Z %d,%d,-1,%g,%g", ax, page, dz, dt ) );
    }

    //! goto
    

    //! get pos
    public String pose( ) throws IOException 
    {
        write("POSE?");

        return read();
    }
}