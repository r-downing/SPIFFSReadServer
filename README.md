# SPIFFSReadServer
An SPI Flash File System (SPIFFS) server extension of the ESP8266WebServer library. This is intended to handle 'read-only' static files without the need for a live editor. Of course files can still be modified programmatically or via other server handlers.

## Features
### Built-in redirect
It handles file-serving via the not-found handler. When a file is not found, it has a built-in browser redirect to the root page /. This plays nicely with captive portals, such as [PersWiFiManager](https://r-downing.github.io/PersWiFiManager/)

### Automatically searches for the smallest file
When it searches for the requested file *file.ext*, it also checks for a minified *file.min.ext*, gzipped *file.ext.gz*, and minfied+gzipped version *file.min.ext.gz* and sends the best one.

### Alternate web-redirect
The server detects when it is connected to a local wifi network, which would indicate the client also has an internet connection. It can than redirect the client to fetch a resource from an online source such as a CDN, rather than directly from the chip.

This is done via an *alt* argument of URL in the page or request. For example, this requests the */angular.min.js* file from the chip. If the chip senses a network connection, instead of streaming the whole file, it will send a 302 redirect to *ajax.googleapis.com...angular.min.js*

```html
<script src="/angular.min.js?alt=https%3A%2F%2Fajax.googleapis.com%2Fajax%2Flibs%2Fangularjs%2F1.6.6%2Fangular.min.js"></script>
```
