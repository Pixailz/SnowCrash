# LEVEL 06

> [!TIP]
> user: `level11` <br>
> pass: `feulo4b72j7edeahuete3no7c`

## STEP 01

After login as level11, we can that we have an `level11.lua` with suid/sgid bits
set

a quick cat on the file show us the full script, but we gonna focus on the
following part
```lua
cat level11.lua
[...]
function hash(pass)
  prog = io.popen("echo "..pass.." | sha1sum", "r")
  data = prog:read("*all")
[...]
```

We can see that a command injection is possible, because the variable `pass` is
our user input, is concatenate with other command and passed as is to the
`io.popen`

## STEP 02, Crafting payload

The first i think of was to brutally put and `-n; getflag` but it give use the
following error message `Password: Erf nope..`. that is because the output of
the command is piped to the `sha1sum`. that behing said, redirecting the output
of the command to a file we have access to is enought to bypass this constraint

so the do this level here's a oneline command:
`rm -f /tmp/f; { echo '-n; getflag >/tmp/f' | nc localhost 5151; }; cat /tmp/f`

> [!IMPORTANT]
> the password for the next level is `fa6v5ateaw21peobuub8ipe6s`
