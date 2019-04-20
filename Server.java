 import java.net.*;
 import java.io.*;
 import java.util.*;
 
   public class Server {
 
      public static void main( String[] args ) {
 
        int port = 3000;
      String line;
      ServerSocket servsock;
       Socket clisock;
       PrintWriter output;

       try {

         // Create a new Server socket
         servsock = new ServerSocket( port );

         while ( true ) {

           // Await a client connection
           clisock = servsock.accept();

           try {

             // Create a new PrintWriter for output
             output = new 
                PrintWriter( clisock.getOutputStream(),
                             true );

             // Generate a time string
             Date now = new Date();
             String curtime = now.toString();

             // Write the time out to the socket
             output.println( curtime );

             // Try to close the socket
             try {
               clisock.close();
             }
             catch( IOException e ) {
               System.out.println( e );
             }

           }
           catch( IOException e ) {
             System.out.println( e );
           }

         }

       }
       catch ( UnknownHostException e ) {
         System.out.println( e );
         System.exit( -1 );
       }
       catch ( IOException e ) {
         System.out.println( e );
         System.exit( -1 );
       }

     }

   }
