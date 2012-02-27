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


struct parser_data;
extern int _nss_files_parse_pwent (char *line, struct passwd *result,
				   struct parser_data *data,
				   size_t datalen, int *errnop);

extern int _nss_files_parse_grent (char *line, struct group *result,
				   struct parser_data *data,
				   size_t datalen, int *errnop);

extern int _nss_files_parse_spent (char *line, struct spwd *result,
                                   struct parser_data *data,
                                   size_t datalen, int *errnop);

static inline int pw_parser(char *p, void *r,
			struct parser_data *data,
			size_t buflen, int *errnop)
{
	return  _nss_files_parse_pwent(p, (struct passwd *) r,
				       data, buflen, errnop);
}

static inline int spw_parser(char *p, void *r,
			struct parser_data *data,
			size_t buflen, int *errnop)
{
	return  _nss_files_parse_spent(p, (struct spwd *) r,
				       data, buflen, errnop);
}

static inline int gr_parser(char *p, void *r,
			struct parser_data *data,
			size_t buflen, int *errnop)
{
	return  _nss_files_parse_grent(p, (struct group *) r,
				       data, buflen, errnop);
}

struct fpent_t
{
	FILE *stream;
	enum filesplus_maptype maptype;
	int (*parser)(char *, void *, struct parser_data *, size_t, int *);
};


extern enum nss_status internal_setent(struct fpent_t *fpinfo);
extern enum nss_status internal_endent(struct fpent_t *fpinfo);
extern enum nss_status internal_getent_r (void *data,
					  struct fpent_t *fpent,
					  char *buffer, size_t buflen,
					  int *errnop,
					  void *cmpdata,
					  int (*cmpFunc)(void *, void*));

#endif
