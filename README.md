# Rot - The remote access toaster
###### A nifty little remote control program

## Contents
1. Why
2. How does it work
3. Further steps

### 1. Why
ROT is a part of a grade 12 computer science project my friend and I are working
on. The whole point of the program is to allow for remote access to a exec 
from a REST client without needing to go through the hassle of setting up port 
forwarding. the whole exec runs on REST because we couldn't be bothered with
a custom TCP protocol

### 2. How does it work?
ROT works in a relatively simple (dumb) way, which can be summarized as follows
1. Server runs independently, waiting for a connection from a client
2. When a client is run, it first sends a REST requesst for an ID from the 
   sever, of which it will use until the client dies. they're generated as a 
   snowflake ID.
3. The client will periodically ping the server for new tasks, of which it 
   will execute one by one, as requested.
4. The server can also be sent new tasks from another API endpoint which 
   will be added to the queue for the specified ID.

### 3. Further steps
There's always more to add to programs, making them faster more efficient, 
smaller. No specific steps are needed at this time