/*
SPIFFSReadServerSecure
Ryan Downing
version 0.0.4
Secure version modified by Donn Morrison
*/

// just replace "ESP8266WebServerSecure server(443);" declaration with "SPIFFSReadServerSecure server(443);"

#define DEBUG_SPIFFSREADSERVER(x)

#include <ESP8266WebServerSecure.h>
#include <FS.h>

#ifndef SPIFFSREADSERVERSECURE_H
#define SPIFFSREADSERVERSECURE_H

class SPIFFSReadServerSecure: public ESP8266WebServerSecure {
  public:
    //static constexpr const char *metaRefreshStr = "<head><meta http-equiv=\"refresh\" content=\"1; url=/\" /></head><body><a href=\"/\">redirecting...</a></body>";
    //pure js redirect, better browser compatibility
    static constexpr const char *redirectStr = "<script>window.location='/'</script>Not found. <a href='/'>Home</a>";
    
    SPIFFSReadServerSecure(int port = 443) : ESP8266WebServerSecure( port) {
      //serve files from SPIFFS
      ESP8266WebServerSecure::onNotFound([&]() {
        if (!handleFileRead(ESP8266WebServerSecure::uri())) {
          ESP8266WebServerSecure::sendHeader("Cache-Control", " max-age=172800");
          ESP8266WebServerSecure::send(302, "text/html", redirectStr);
        }
      }); //ESP8266WebServerSecure::onNotFound
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
        if (ESP8266WebServerSecure::hasArg("download"))
          ESP8266WebServerSecure::sendHeader("Content-Disposition", " attachment;");
        if (ESP8266WebServerSecure::uri().indexOf("nocache") < 0)
          ESP8266WebServerSecure::sendHeader("Cache-Control", " max-age=172800");

        //optional alt arg (encoded url), server sends redirect to file on the web
        if (WiFi.status() == WL_CONNECTED && ESP8266WebServerSecure::hasArg("alt")) {
          ESP8266WebServerSecure::sendHeader("Location", ESP8266WebServerSecure::arg("alt"), true);
          ESP8266WebServerSecure::send ( 302, "text/plain", "");
        } else {
          //server sends file
          size_t sent = ESP8266WebServerSecure::streamFile(file, contentType);
        }
        file.close();
        return true;
      } //if SPIFFS.exists
      return false;
    } //bool handleFileRead


}; //class

#endif

