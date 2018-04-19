# Server


The structure of the server part consists of:

- **Working with network (HttpServer)**;
- **Working with DataBase**.

## HttpServer Realization

We use library "libevent" for working with HTTP, "jansson" for working with JSON. As an ORM for PostgreSQL we use library "libpqxx".

## INSTALLING

### Library "jannson"

The Jansson source is available at http://www.digip.org/jansson/releases/.
- **bunzip2 -c jansson-1.1.11.tar.bz2 | tar xf -**
- **cd jansson-1.1.11**
- **./configure**
- **make**
- **make check**
- **make install**


### Library "libevent"

For macOs:
- **brew install libevent**

For Ubuntu:
- **sudo apt-get install libevent-dev**