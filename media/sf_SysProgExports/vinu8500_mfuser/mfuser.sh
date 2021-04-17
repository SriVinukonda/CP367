#!/bin/sh
who /var/log/wtmp | cut -d ' ' -f1 | sort -k1 | uniq -c | tail -n1 | cat
