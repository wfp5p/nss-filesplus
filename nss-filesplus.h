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


/* common.c */
extern enum nss_status internal_setent(struct fpent_t *fpinfo);
extern enum nss_status internal_endent(struct fpent_t *fpinfo);

/* group.c */
extern enum nss_status _nss_filesplus_setgrent(void);
extern enum nss_status _nss_filesplus_endgrent(void);
extern enum nss_status _nss_filesplus_getgrent_r(struct group *gr, char *buffer,
						 size_t buflen, int *errnop);
extern enum nss_status _nss_filesplus_getgrnam_r(const char *name, struct group *gr,
						 char *buffer, size_t buflen,
						 int *errnop);
extern enum nss_status _nss_filesplus_getgrgid_r(const gid_t gid, struct group *gr,
						 char *buffer, size_t buflen,
						 int *errnop);
extern enum nss_status _nss_filesplus_initgroups_dyn(const char *user,
						     gid_t gid,
						     long int *start,
						     long int *size,
						     gid_t **groupsp,
						     long int limit,
						     int *errnop);

/* passwd.c */
extern enum nss_status _nss_filesplus_getpwuid_r(uid_t uid, struct passwd *pwd,
						 char *buffer, size_t buflen,
						 int *errnop);
extern enum nss_status _nss_filesplus_getpwnam_r(const char *name,
						 struct passwd *pwd, char *buffer,
						 size_t buflen, int *errnop);
extern enum nss_status _nss_filesplus_setpwent(void);
extern enum nss_status _nss_filesplus_endpwent(void);
extern enum nss_status _nss_filesplus_getpwent_r(struct passwd *pwd, char *buffer,
						 size_t buflen, int *errnop);


/* shadow.c */
extern enum nss_status _nss_filesplus_setspent(void);
extern enum nss_status _nss_filesplus_endspent(void);
extern enum nss_status _nss_filesplus_getspent_r(struct spwd *spw, char *buffer,
						 size_t buflen, int *errnop);
extern enum nss_status _nss_filesplus_getspnam_r(const char *name, struct spwd *spw,
						 char *buffer, size_t buflen,
						 int *errnop);


#endif
