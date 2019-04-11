import java.net.*;
import java.io.*;
import java.lang.*;
import java.util.*;
public class Server
{
    public static void main (String[] args)
    {
      try
      {      
      ServerSocket srvr = new ServerSocket(3000);
      Socket skt = srvr.accept();
      
      InputStreamReader bf = new InputStreamReader(skt.getInputStream());
      BufferedReader in= new BufferedReader(bf);
      PrintWriter out = new PrintWriter(skt.getOutputStream(),true);
      Scanner s=new Scanner(System.in);
      
      Thread send=new Thread(new Runnable(){
      String msg;
          
      public void run(){
            while(true){
  
                  msg=s.nextLine();
                  out.print(msg);
                  out.flush();
            }
      }
      });
      send.start();
      
      Thread receive=new Thread(new Runnable(){
      String msg;
      
      public void run(){
            while(true){
                  try{
                        msg=in.readLine();
                  }catch(IOException e){
                        e.printStackTrace();
                  }
                  System.out.print("Client: "+msg);
            }
      }
      });
      receive.start();
            
      
      
      //closing connection
      //out.close();
      //skt.close();
      //srvr.close();
      }catch (IOException e){
            e.printStackTrace();
      }
    }
}
