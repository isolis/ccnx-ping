CCNx Simple Ping
====

This is a trivial ping based on the Hello World.   It is just a simple example,
it is not meant to be complete or correct.

Copyright 2015,2016 Palo Alto Research Center Inc.



Building
-----

This code assumes an Ubuntu 14.04 with CCNx installed.  It is meant to show a
simple example of a makefile and the libraries required to build a simple piece
of software.

You will need to set the location of the CCNx libraries by setting the
`CCNX_HOME` environemnt variable. If needed, the `LIBEVENT_HOME` environment
variable can be set to point to where Libevent has been installed.

To compile, type make.

Running
----
* Run the forwarder (metis)
* Run the ccnx-ping-server
* Run ccnx-ping

Limitations
----
This code doesn't have the ability to select the name being pinged on the
command line.  It's just a test for a single forwarder.

Contact
----

You can find more information at the ccnx website: http://www.ccnx.org/


License
----

This software is licensed under a BSD license. See LICENSE file.
