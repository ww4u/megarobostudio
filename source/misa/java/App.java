package mrq;

import java.io.IOException;

public class App {

    public static void testMrq() throws IOException, InterruptedException
    {
        MRQ a; 
        a = new MRQ("device1" );

        a.write( "*idn?" );

        String info = a.read();
        if ( info != null )
        { System.out.println( info ); }

        System.out.println( a.qStatus( 0, 0) );

        a.move(0, 0, 1, 180 );
        a.waitIdle( 0, 0 );

        for ( int i = 0; i < 10; i++ )
        {
            a.call( 0, 0 );
            a.waitIdle(0, 0);
        }

        a.close();

        System.out.println("normal end");
    }

    public static void testT4() throws IOException
    {
        MegaRobo a; 
        a = new Sinanju("mrx-t4" );

        a.write( "*idn?" );

        String info = a.read();
        if ( info != null )
        { System.out.println( info ); }

        System.out.println( a.qStatus( 0, 0) );

        a.close();

        System.out.println("normal end");
    }

    public static void main( String[] args )
    {
        try
        {
            testMrq();
        }
        catch( Exception excp )
        { System.out.println(excp.getMessage()); }
        finally
        { System.out.println("final end"); }            
    }
}