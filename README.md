socktest
========

A demonstration of a simple server-client program pair over TCP.

Check out this [tutorial](http://www.rainydayz.org/beej/bgnet/index.html) for more info and an exlanation of the interface used.

usage
-----

Build the project using

````
make
````

In one terminal, start the server with

````
server
````

Then, in another terminal, start a client

````
client localhost
````

You can also start clients on other hosts, just replace `localhost` with the IP address of the host the server is running on.

Connect multiple `client`s for extra fun.
