/*
SPIFFSReadServer
Ryan Downing
version 0.0.4
*/

// just replace "ESP8266WebServer server(80);" declaration with "SPIFFSReadServer server(80);"

#define DEBUG_SPIFFSREADSERVER(x) 

#include <ESP8266WebServer.h>
#include <FS.h>

#ifndef SPIFFSREADSERVER_H
#define SPIFFSREADSERVER_H

class SPIFFSReadServer: public ESP8266WebServer {
  public:
    //static constexpr const char *metaRefreshStr = "<head><meta http-equiv=\"refresh\" content=\"1; url=/\" /></head><body><a href=\"/\">redirecting...</a></body>";
    //pure js redirect, better browser compatibility
    static constexpr const char *redirectStr = "<script>window.location='/'</script>Not found. <a href='/'>Home</a>";
    
    SPIFFSReadServer(int port = 80) : ESP8266WebServer( port) {
      //serve files from SPIFFS
      ESP8266WebServer::onNotFound([&]() {
        if (!handleFileRead(ESP8266WebServer::uri())) {
          ESP8266WebServer::sendHeader("Cache-Control", " max-age=172800");
          ESP8266WebServer::send(302, "text/html", redirectStr);
        }
      }); //ESP8266WebServer::onNotFound
    } //constructor


    //code from fsbrowser example, consolidated.
    bool handleFileRead(String path) {
      DEBUG_SPIFFSREADSERVER("handlefileread" + path);
      if (path.endsWith("/")) path += "index.htm";
      String contentType;
      if (path.endsWith(".htm") || path.endsWith(".html")) contentType = "text/html";
      else if (path.endsWith(".css")) contentType = "text/css";
      else if (path.endsWith(".js")) contentType = "application/javascript";
      else if (path.endsWith(".png")) contentType = "image/png";
      else if (path.endsWith(".gif")) contentType = "image/gif";
      else if (path.endsWith(".jpg")) contentType = "image/jpeg";
      else if (path.endsWith(".ico")) contentType = "image/x-icon";
      else if (path.endsWith(".xml")) contentType = "text/xml";
      else if (path.endsWith(".pdf")) contentType = "application/x-pdf";
      else if (path.endsWith(".zip")) contentType = "application/x-zip";
      else if (path.endsWith(".gz")) contentType = "application/x-gzip";
      else if (path.endsWith(".json")) contentType = "application/json";
      else contentType = "text/plain";

      //split filepath and extension
      String prefix = path, ext = "";
      int lastPeriod = path.lastIndexOf('.');
      if (lastPeriod >= 0) {
        prefix = path.substring(0, lastPeriod);
        ext = path.substring(lastPeriod);
      }

      //look for smaller versions of file
      //minified file, good (myscript.min.js)
      if (SPIFFS.exists(prefix + ".min" + ext)) path = prefix + ".min" + ext;
      //gzipped file, better (myscript.js.gz)
      if (SPIFFS.exists(prefix + ext + ".gz")) path = prefix + ext + ".gz";
      //min and gzipped file, best (myscript.min.js.gz)
      if (SPIFFS.exists(prefix + ".min" + ext + ".gz")) path = prefix + ".min" + ext + ".gz";

      if (SPIFFS.exists(path)) {
        DEBUG_SPIFFSREADSERVER("sending file " + path);
        File file = SPIFFS.open(path, "r");
        if (ESP8266WebServer::hasArg("download"))
          ESP8266WebServer::sendHeader("Content-Disposition", " attachment;");
        if (ESP8266WebServer::uri().indexOf("nocache") < 0)
          ESP8266WebServer::sendHeader("Cache-Control", " max-age=172800");

        //optional alt arg (encoded url), server sends redirect to file on the web
        if (WiFi.status() == WL_CONNECTED && ESP8266WebServer::hasArg("alt")) {
          ESP8266WebServer::sendHeader("Location", ESP8266WebServer::arg("alt"), true);
          ESP8266WebServer::send ( 302, "text/plain", "");
        } else {
          //server sends file
          size_t sent = ESP8266WebServer::streamFile(file, contentType);
        }
        file.close();
        return true;
      } //if SPIFFS.exists
      return false;
    } //bool handleFileRead


}; //class

#endif

