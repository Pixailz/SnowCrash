# LEVEL 08

> [!TIP]
> user: `level13` <br>
> pass: `g1qKMiRpXf53AWhDaU7FEkczr`

## STEP 01

An `ls` on the home of level13 show us that we have a level13 to deal with,
after doing `file ./level13`, we can see that the file is an ELF, so reverse
time !

## STEP 02

launching the programme give us the following message
'UID 2013 started us but we we expect 4242'. after reversing the main, we can
see at `*main+9` that a call to `getuid` is made, and right after, a `cmp eax,0x1092`
which is 4242 in hexa

## STEP 03

so to do this level, made a break at `*main+14`, run and `set $eax=4242` right
after hiting the breakpoint

## BONUS

here's a script for doing all this in one shoot

```bash
cat <<-'EOF' >/tmp/gdb
file /home/user/level13/level13
break *main+14

run
set $eax=4242
c
EOF
gdb -x /tmp/gdb --batch-silent
```
