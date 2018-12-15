package mrq;

import java.io.IOException;

public class MRQ extends MegaRobo
{
    public MRQ( String name, String ip, int socket ) throws IOException
    {
        super( name, ip, socket );
    }

    public MRQ( String name, String ip ) throws IOException
    {
        this( name, ip, 1234 );
    }

    public MRQ( String name ) throws IOException
    {
        this( name, "127.0.0.1" );
    }

    public int move( int ax, int page, float t, float angle, float endV )
    {
        return write( String.format("MOVE %d,%d,%g,%g,%g", ax, page, t, angle, endV) );
    }

    public int move( int ax, int page, float t, float angle )
    {
        return write( String.format("MOVE %d,%d,%g,%g,0", ax, page, t, angle ) );
    }
}