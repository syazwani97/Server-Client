int sendMail(struct mailHeader *mail)
    {
      int connfd, result, ret, goodMsg = 0;
      struct sockaddr_in servaddr;
      char mailRcpt[129];
      char line[256];
 
      connfd = socket(AF_INET, SOCK_STREAM, 0);
 
     bzero((void *)&servaddr, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_port = htons(25);

     servaddr.sin_addr.s_addr = inet_addr(mailServer);

     /* if the prior inet_addr results in a '-1' (or error),
      * then we assume that the gateway symbolic is not a
      * dotted-notation IP address. It must therefore be a
      * fully-qualified domain name and we use gethostbyname
      * to resolve it.
      */
     if (servaddr.sin_addr.s_addr == 0xffffffff) {

       struct hostent *hptr =
               (struct hostent *)gethostbyname(mailServer);

       if (hptr == NULL) {
         /* Don't know what the mailServer represents... */
         return(-1);
       } else {

         struct in_addr **addrs;
         addrs = (struct in_addr **)hptr->h_addr_list;
         memcpy(&servaddr.sin_addr,
         *addrs, sizeof(struct in_addr));

       }

     }

     /* Connect to the SMTP server */
     result = connect(connfd, (struct sockaddr *)&servaddr,
                       sizeof(servaddr));

     do {

       /* Look for initial salutation */
       if ( dialog( connfd, NULL, "220" ) ) break;

       /* Send HELO and await response */
       if ( dialog( connfd, hello_msg, "250" ) ) break;

       /* Send MAIL FROM and await response */
        sprintf(line, "MAIL FROM:<%s>\n", mail->sender);
        if ( dialog( connfd, line, "250" ) ) break;

       /* Send RCPT TO and await response */
       sprintf(line, "RCPT TO:<%s>\n", mail->recipient);
       if ( dialog( connfd, line, "250" ) ) break;

       /* Send DATA and await response */
       if ( dialog( connfd, "DATA\n", "354" ) ) break;

       /* Send out the header first */
       sprintf(line, "From: %s\n", mail->sender);
       if ( dialog( connfd, line, NULL ) ) break;

       sprintf(line, "To: %s\n", mail->recipient);
       if ( dialog( connfd, line, NULL ) ) break;

       sprintf(line, "Subject: %s\n", mail->subject);
       if ( dialog( connfd, line, NULL ) ) break;

       if (mail->contentType[0] != 0) {
         sprintf(line, "Content-Type: %s\n",
                  mail->contentType);
         if ( dialog( connfd, line, NULL ) ) break;
       }

       if ( dialog( connfd, mail->contents, NULL ) ) break;

       /* Send mail-end and await response */
       if ( dialog( connfd, "\n.\n", "250" ) ) break;

       if ( dialog( connfd, "QUIT\n", "221" ) ) break;

       goodMsg = 1;

     } while (0);

     close(connfd);

     return(goodMsg);
   }
