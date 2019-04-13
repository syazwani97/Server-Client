import java.io.*;
import java.net.*;
class Server
{
  public static void main(String[] args) throws Exception
  {
      ServerSocket sersock = new ServerSocket(3000);
      System.out.println("This is a server program\n");
      Socket sock = sersock.accept( );                          
                             
      BufferedReader keyRead = new BufferedReader(new InputStreamReader(System.in));
	                      
      OutputStream ostream = sock.getOutputStream(); 
      PrintWriter pwriter = new PrintWriter(ostream, true);

      InputStream istream = sock.getInputStream();
      BufferedReader receiveRead = new BufferedReader(new InputStreamReader(istream));

      String receiveMessage, sendMessage;               
      while(true)
      {
        if((receiveMessage = receiveRead.readLine()) != null)  
        {
           System.out.println(receiveMessage);         
        }         
        sendMessage = keyRead.readLine(); 
        pwriter.println(sendMessage);             
        pwriter.flush();
      }               
    }                    
}                        
