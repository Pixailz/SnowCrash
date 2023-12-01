# LEVEL 00

> [!TIP]
> user: `level00` <br>
> pass: `level00`

## STEP 01

Searching for files belonging to **flag00**

```bash
find / -user flag00 2>/dev/null
#/usr/sbin/john
#/rofs/usr/sbin/john
cat /usr/sbin/john
#cdiiddwpgswtgt
```

> [!NOTE]
> `cdiiddwpgswtgt` seems to be a **Rot13** like encoding

## STEP 02

Bruteforcing Rot13

Going to [CyberChef](https://gchq.github.io/CyberChef/#recipe=ROT13_Brute_Force(true,true,false,100,0,true,'')&input=Y2RpaWRkd3Bnc3d0Z3Q)
give us the number of rotation operated on the string above, 11, which give us
the password for **flag00**

> [!IMPORTANT]
> the next password is `nottoohardhere`
