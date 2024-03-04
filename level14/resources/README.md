# LEVEL 08

> [!TIP]
> user: `level14` <br>
> pass: `2A31L79asukciNyi8uppkEuSx`

## STEP 01

An `ls` on the home of level14, we can see that we have nothing.
so my first guess is to reverse the `/bin/getflag` binary to bypass the check
and get all flag from all user

## STEP 02

First we need to get ride of the `You should not reverse this` message
to do so, we break at `*main+74` and `set $eflags &= ~(1 << $SF)`, doing so
make the `jns 0x80489a8 <main+98>` to be taken

## STEP 03

For the next step we have to break right after the getuid call, at `*main+444`.
when it's done, `set $eax` to the `$UID` you wan't to have the flag for.

```bash
$ id flag14
uid=3014(flag14) gid=3014(flag14) groups=3014(flag14),1001(flag)
```

so, `set $eax=3014`, will give you the flag for the flag14 user

## BONUS

here's a script for doing all this in one shoot

```bash
cat <<-'EOF' >/tmp/gdb
file /bin/getflag
set $UID=3014
set $SF=7
break *main+74
break *main+444

run
set $eflags &= ~(1 << $SF)
c
set $eax=$UID
c
EOF
gdb -x /tmp/gdb --batch-silent
```
