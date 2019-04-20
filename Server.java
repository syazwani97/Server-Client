import java.net.*;
 2   import java.io.*;
 3   import java.util.*;
 4
 5   public class strmsrv {
 6
 7     public static void main( String[] args ) {
 8
 9       int port = 13;
10       String line;
11       ServerSocket servsock;
12       Socket clisock;
13       PrintWriter output;
14
15       try {
16
17         // Create a new Server socket
18         servsock = new ServerSocket( port );
19
20         while ( true ) {
21
22           // Await a client connection
23           clisock = servsock.accept();
24
25           try {
26
27             // Create a new PrintWriter for output
28             output = new 
29                PrintWriter( clisock.getOutputStream(),
30                             true );
31
32             // Generate a time string
33             Date now = new Date();
34             String curtime = now.toString();
35
36             // Write the time out to the socket
37             output.println( curtime );
38
39             // Try to close the socket
40             try {
41               clisock.close();
42             }
43             catch( IOException e ) {
44               System.out.println( e );
45             }
46
47           }
48           catch( IOException e ) {
49             System.out.println( e );
50           }
51
52         }
53
54       }
55       catch ( UnknownHostException e ) {
56         System.out.println( e );
57         System.exit( -1 );
58       }
59       catch ( IOException e ) {
60         System.out.println( e );
61         System.exit( -1 );
62       }
63
64     }
65
66   }
