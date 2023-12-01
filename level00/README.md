# LEVEL 00

user:		level00
password:	level00

## STEP 01

Searching for file belonging to flag00

```
level00@SnowCrash:~$ find / -user flag00 2>/dev/null
/usr/sbin/john
/rofs/usr/sbin/john
level00@SnowCrash:~$ cat /usr/sbin/john
cdiiddwpgswtgt
```
   > [!NOTE]
   > `cdiiddwpgswtgt` seems to be a Rot13 like encoding

## STEP 02

Bruteforcing Rot13

going to [here](https://gchq.github.io/CyberChef/#recipe=ROT13_Brute_Force(true,true,false,100,0,true,'')&input=Y2RpaWRkd3Bnc3d0Z3Q)
give us the number of rotation operated on the string above, 11, which give us
the password for the `flag00` user

> `nottoohardhere`
