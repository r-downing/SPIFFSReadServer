# SPIFFSReadServer
An SPI Flash File System (SPIFFS) server extension of the ESP8266WebServer library. This is intended to handle 'read-only' static files without the need for a live editor. Of course files can still be modified programmatically or via other server handlers.

It is tested/working, but still very much in progress and there is much more I would like to do.

- [SPIFFSReadServer](#spiffsreadserver)
	- [How to Use](#how-to-use)
	- [Installation](#installation)
		- [Via Arduino IDE Library Manager](#via-arduino-ide-library-manager)
		- [Via zip](#via-zip)
	- [Features](#features)
		- [Built-in redirect](#built-in-redirect)
		- [Automatically searches for the smallest file](#automatically-searches-for-the-smallest-file)
		- [Alternate web-redirect](#alternate-web-redirect)

## How to Use

Instead of the declaration:

```cpp
ESP8266WebServer server(80);
```

Just replace it with:

```cpp
SPIFFSReadServer server(80);
```

[Full Example Code](https://github.com/r-downing/SPIFFSReadServer/blob/master/examples/spiffs_rest_api/spiffs_rest_api.ino)

The SPIFFSReadServer inherits all the normal functions from the ESP8266WebServer library, and automatically sets up the file-serving function on the onNotFound handler.

## Installation

### Via Arduino IDE Library Manager

**Sketch -> Include Library -> Manage Libraries… ->** search for “PersWiFiManager” 

### Via zip

[Download the latest zip](https://github.com/r-downing/SPIFFSReadServer/archive/master.zip) and extract to your Arduino Libraries folder.

## Features
### Built-in redirect
It handles file-serving via the not-found handler. When a file is not found, it has a built-in browser redirect to the root page /. This plays nicely with captive portals, such as [PersWiFiManager](http://ryandowning.net/PersWiFiManager/)

### Automatically searches for the smallest file
When it searches for the requested file *file.ext*, it also checks for a minified *file.min.ext*, gzipped *file.ext.gz*, and minfied+gzipped version *file.min.ext.gz* and sends the best one.

### Alternate web-redirect
The server detects when it is connected to a local wifi network, which would indicate the client also has an internet connection. It can than redirect the client to fetch a resource from an online source such as a CDN, rather than directly from the chip.

This is done via an *alt* argument of URL in the page or request. For example, this requests the */angular.min.js* file from the chip. If the chip senses a network connection, instead of streaming the whole file, it will send a 302 redirect to *ajax.googleapis.com...angular.min.js*

```html
<script src="/angular.min.js?alt=https%3A%2F%2Fajax.googleapis.com%2Fajax%2Flibs%2Fangularjs%2F1.6.6%2Fangular.min.js"></script>
```
