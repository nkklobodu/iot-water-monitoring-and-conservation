// generate HTML page and list contents of the data directory on SD card.
  void sdDir() {
    if (sdStatus) {
      if (server.args() > 0 ) {
        String Order = server.arg(0);
        if (Order.indexOf("download_")>=0) {
          Order.remove(0,9);
          sdFileDownload(Order);
        }
      }

      File data = SD.open("/DATA");
      if (data) {
        data.rewindDirectory();
        sendHTMLHeader();
        webpage += F("<table align='center'>");
        webpage += F("<tr><th>Name/Type</th><th style='width:20%'>Type File/Dir</th><th>File Size</th></tr>");
        printDirectory("/DATA",0);
        webpage += F("</table>");
        sendHTMLContent();
        data.close();
      }
      else
      {
        sendHTMLHeader();
        webpage += F("<h3>No Files Found</h3>");
      }
      appendPageFooter();
      sendHTMLContent();
      sendHTMLStop();
    } else reportSDNotPresent();
  }


// recursively list contents of a directory on SD card as html tables
  void printDirectory(const char* dirname, uint8_t levels) {
    File data = SD.open(dirname);
    if (!data) return;
    if (!data.isDirectory()) return;

    File file = data.openNextFile();
    int i = 0;
    while(file) {
      if (webpage.length() > 1000) sendHTMLContent();
      if (file.isDirectory()) {
        webpage += "<tr><td>"+String(file.isDirectory()?"Dir":"File")+"</td><td>"+String(file.name())+"</td><td></td></tr>";
        printDirectory(file.name(), levels-1);
      }
      else
      {
        webpage += "<tr><td>"+String(file.name())+"</td>";
        webpage += "<td>"+String(file.isDirectory()?"Dir":"File")+"</td>";
          String fsize = fileSize(file.size());
        webpage += "<td>"+fsize+"</td>";
        webpage += "<td>";
        webpage += F("<FORM action='/' method='post'>");
        webpage += F("<button type='submit' name='download'");
        webpage += F("' value='"); webpage +="download_"+String(file.name()); webpage +=F("'>Download</button>");
        webpage += "</td></tr>";
      }
      file = data.openNextFile();
      i++;
    }
    file.close();
  }


// handle client requests to download files from SD card
  void sdFileDownload(String filename) {
    if (sdStatus) {
      File download = SD.open("/"+filename);
      if (download) {
        server.sendHeader("Content-Type", "text/text");
        server.sendHeader("Content-Disposition", "attachment; filename="+filename);
        server.sendHeader("Connection", "close");
        server.streamFile(download, "application/octet-stream");
        download.close();
      } else reportFileNotPresent();
    } else reportSDNotPresent();
  }


// send HTTP header to client and begin an HTML page
  void sendHTMLHeader() {
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.setContentLength(CONTENT_LENGTH_UNKNOWN); 
    server.send(200, "text/html", "");
    appendPageHeader();
    server.sendContent(webpage);
    webpage = "";
  }


//  send the contents of the 'webpage' variable to the client.
  void sendHTMLContent() {
    server.sendContent(webpage);
    webpage = "";
  }


// send empty string to client and terminate connection.
  void sendHTMLStop() {
    server.sendContent("");
    server.client().stop();
  }


// generates HTML page with message indicating no SD card is present
  void reportSDNotPresent() {
    sendHTMLHeader();
    webpage += F("<h3>No SD Card present</h3>");
    webpage += F("<a href='/'>[Back]</a><br><br>");
    appendPageFooter();
    sendHTMLContent();
    sendHTMLStop();
  }


// generates HTML page with message indicating the requested file is not present on the SD card
  void reportFileNotPresent() {
    sendHTMLHeader();
    webpage += F("<h3>File does not exist</h3>");
    webpage += F("<a href='/'>[Back]</a><br><br>");
    appendPageFooter();
    sendHTMLContent();
    sendHTMLStop();
  }


// convert file size to human-readable values
  String fileSize(int bytes) {
    String fsize = "";
    if (bytes < 1024)                  fsize = String(bytes)+" B";
    else if (bytes < (1024*1024))      fsize = String(bytes/1024.0,3)+" KB";
    else if (bytes < (1024*1024*1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
    else                               fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
    return fsize;
  }