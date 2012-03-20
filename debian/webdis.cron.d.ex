#
# Regular cron jobs for the webdis package
#
0 4	* * *	root	[ -x /usr/bin/webdis_maintenance ] && /usr/bin/webdis_maintenance
