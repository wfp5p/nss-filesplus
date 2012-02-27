#ifndef NSS_FILESPLUS_H
#define NSS_FILESPLUS_H

#define _GNU_SOURCE

#include <stdio.h>
#include <syslog.h>
#include <nss.h>
#include <pwd.h>
#include <shadow.h>
#include <grp.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define MAX_LINE_SIZE 1024 /* max length of a line in config file */
#define CONFIG_FILE "/etc/filesplus/nss-filesplus.cfg"
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#ifdef DEBUG
#define NSS_DEBUG(msg, ...) syslog(LOG_DEBUG, (msg), ## __VA_ARGS__)
#else
#define NSS_DEBUG(msg, ...)
#endif

#define NSS_ERROR(msg, ...) syslog(LOG_ERR, (msg), ## __VA_ARGS__)

enum filesplus_maptype {
	FP_PASSWD,
	FP_GROUP,
	FP_SHADOW
};

struct fpent_t
{
	FILE *stream;
	enum filesplus_maptype maptype;
};


extern enum nss_status internal_setent(struct fpent_t *fpinfo);
extern enum nss_status internal_endent(struct fpent_t *fpinfo);

#endif
