// variable containing HTML page codes  
  String webpage = "";


// add beginning of the HTML page to the 'webpage' variable.
  void appendPageHeader() {
    webpage  = F("<!DOCTYPE html><html>");
    webpage += F("<head>");
    webpage += F("<title>Water Data Server</title>");
    webpage += F("<meta name='viewport' content='user-scalable=yes,initial-scale=1.0,width=device-width'>");
    webpage += F("<style>");
    webpage += F("body{max-width:65%;margin:0 auto;font-family:Cambria;font-size: 100%;}");
    webpage += F("ul{list-style-type:none;padding:0;border-radius:0px;overflow:hidden;background-color:white;font-size:25px;}");
    webpage += F("li{float:left;border-radius:0px;border-right:0px solid #bbb;}");
    webpage += F("li a{color:#093c7a;display:block;border-radius:6px;padding:7px 7px;text-decoration:none;font-size:100%}");
    webpage += F("li a:hover{color:white;background-color:#093c7a;border-radius:0px;font-size:100%}");
    webpage += F("h1{color:white;border-radius:0px;font-size:32px;padding:3.2px 3.2px;background:#093c7a;}");
    webpage += F("h2{color:#093c7a;font-size:12.8px;}");
    webpage += F("h3{font-size:17px;}");
    webpage += F("table{font-family:Cambria;font-size:14.4px;border-collapse:collapse;width:85%;}"); 
    webpage += F("th,td {border:0.96px solid #dddddd;text-align:left;padding:4.8px;border-bottom:0.96px solid #dddddd;}"); 
    webpage += F("tr:nth-child(odd) {background-color:#eeeeee;}");
    webpage += F(".rcorners_n {border-radius:8px;background:#558ED5;padding:4.8px 4.8px;width:20%;color:white;font-size:75%;}");
    webpage += F(".rcorners_m {border-radius:8px;background:#558ED5;padding:4.8px 4.8px;width:50%;color:white;font-size:75%;}");
    webpage += F(".rcorners_w {border-radius:8px;background:#558ED5;padding:4.8px 4.8px;width:70%;color:white;font-size:75%;}");
    webpage += F(".column{float:left;width:50%;height:45%;}");
    webpage += F(".row:after{content:'';display:table;clear:both;}");
    webpage += F("*{box-sizing:border-box;}");
    webpage += F("a{font-size:75%;}");
    webpage += F("p{font-size:75%;}");
    webpage += F("</style></head><body><h1><center>IoT Water Monitoring & Conservation</center></h1>");
    webpage += F("<ul>");
    webpage += F("<li><a href='/'>Files</a></li>");
    webpage += F("</ul>");
  }


// add the end of the HTML page to the 'webpage' variable.
  void appendPageFooter() {
    webpage += F("</body></html>");
  }
