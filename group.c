#include "nss-filesplus.h"


static struct fpent_t fgrent = {
	.maptype   = FP_GROUP,
        .parser    = gr_parser
};

static inline int cmpgid(void *d1, void *d2)
{
	struct group *gr = (struct group *) d1;
	gid_t *gid = (gid_t *) d2;

	return (*gid == gr->gr_gid);
}

static inline int cmpname(void *d1, void *d2)
{
	struct group *gr = (struct group *) d1;
	char *name = (char *) d2;

	return (strcmp(gr->gr_name, name) == 0);
}


enum nss_status _nss_filesplus_setgrent(void)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_setent(&fgrent);
}

enum nss_status _nss_filesplus_endgrent(void)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_endent(&fgrent);
}

enum nss_status _nss_filesplus_getgrent_r(struct group *gr, char *buffer,
					  size_t buflen, int *errnop)
{
	enum nss_status result = NSS_STATUS_SUCCESS;

	NSS_DEBUG("filesplus: %s\n",__func__);

	if (fgrent.stream == NULL)
		result = internal_setent(&fgrent);

	if (result != NSS_STATUS_SUCCESS)
		return result;

	return internal_getent_r((void *) gr, &fgrent,  buffer, buflen,
				   errnop, NULL, NULL);

}

enum nss_status _nss_filesplus_getgrnam_r(const char *name, struct group *gr,
					  char *buffer, size_t buflen,
					  int *errnop)
{
	enum nss_status result = NSS_STATUS_SUCCESS;

	NSS_DEBUG("filesplus: %s\n",__func__);

	result = internal_setent(&fgrent);

	if (result != NSS_STATUS_SUCCESS)
		return result;

	return internal_getent_r((void *) gr, &fgrent, buffer, buflen,
				 errnop, (void *) name, cmpname);

}

enum nss_status _nss_filesplus_getgrgid_r(const gid_t gid, struct group *gr,
					  char *buffer, size_t buflen,
					  int *errnop)
{
	enum nss_status result = NSS_STATUS_SUCCESS;

	NSS_DEBUG("filesplus: %s\n",__func__);

	result = internal_setent(&fgrent);

	if (result != NSS_STATUS_SUCCESS)
		return result;

	return internal_getent_r((void *) gr, &fgrent, buffer, buflen,
				 errnop, (void *) &gid, cmpgid);

}


