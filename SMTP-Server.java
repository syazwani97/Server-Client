import java.util.*;
import java.io.*;
import java.nio.file.*;
import javax.mail.*;
import javax.mail.internet.*;

/**
* Simple demonstration of using the javax.mail API.
*
* Run from the command line. Please edit the implementation
* to use correct email addresses and host name.
*/
public class SMTP {

  public static void main( String... aArguments ){
    Emailer emailer = new Emailer();
    //the domains of these email addresses should be valid,
    //or the example will fail:
    emailer.sendEmail(
      "fromblah@blah.com", "toblah@blah.com",
       "Testing 1-2-3", "blah blah blah"
    );
   }

  /**
  * Send a single email.
  */
  public void sendEmail(
    String aFromEmailAddr, String aToEmailAddr,
    String aSubject, String aBody
  ){
    //Here, no Authenticator argument is used (it is null).
    //Authenticators are used to prompt the user for user
    //name and password.
    Session session = Session.getDefaultInstance(fMailServerConfig, null);
    MimeMessage message = new MimeMessage(session);
    try {
      //the "from" address may be set in code, or set in the
      //config file under "mail.from" ; here, the latter style is used
      //message.setFrom(new InternetAddress(aFromEmailAddr));
      message.addRecipient(
        Message.RecipientType.TO, new InternetAddress(aToEmailAddr)
      );
      message.setSubject(aSubject);
      message.setText(aBody);
      Transport.send(message);
    }
    catch (MessagingException ex){
      System.err.println("Cannot send email. " + ex);
    }
  }

  /**
  * Allows the config to be refreshed at runtime, instead of
  * requiring a restart.
  */
  public static void refreshConfig() {
    fMailServerConfig.clear();
    fetchConfig();
  }

  // PRIVATE 

  private static Properties fMailServerConfig = new Properties();

  static {
    fetchConfig();
  }

  /**
  * Open a specific text file containing mail server
  * parameters, and populate a corresponding Properties object.
  */
  private static void fetchConfig() {
    //This file contains the javax.mail config properties mentioned above.
    Path path = Paths.get("C:\\Temp\\MyMailServer.txt");
    try (InputStream input = Files.newInputStream(path)) {
      fMailServerConfig.load(input);
    }
    catch (IOException ex){
      System.err.println("Cannot open and load mail server properties file.");
    }
  }
} 
