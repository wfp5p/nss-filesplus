#include "nss-filesplus.h"

static struct fpent_t fspwent = {
	.maptype   = FP_SHADOW,
        .parser    = spw_parser
};

static inline int cmpname(void *d1, void *d2)
{
	struct spwd *spw = (struct spwd *) d1;
	char *name = (char *) d2;

	return (strcmp(spw->sp_namp, name) == 0);
}

enum nss_status _nss_filesplus_setspent(void)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_setent(&fspwent);
}

enum nss_status _nss_filesplus_endspent(void)
{

	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_endent(&fspwent);
}


enum nss_status _nss_filesplus_getspent_r(struct spwd *spw, char *buffer,
					  size_t buflen, int *errnop)
{
	enum nss_status result = NSS_STATUS_SUCCESS;

	NSS_DEBUG("filesplus: %s\n",__func__);

	if (fspwent.stream == NULL)
		result = internal_setent(&fspwent);

	if (result != NSS_STATUS_SUCCESS)
		return result;

	return internal_getent_r((void *) spw, &fspwent, buffer, buflen,
				 errnop, NULL, NULL);
}


enum nss_status _nss_filesplus_getspnam_r(const char *name, struct spwd *spw,
					  char *buffer, size_t buflen,
					  int *errnop)
{
	enum nss_status result = NSS_STATUS_SUCCESS;

	NSS_DEBUG("filesplus: %s *%s*\n",__func__, name);

	result = internal_setent(&fspwent);

	if (result != NSS_STATUS_SUCCESS)
		return result;

	result = internal_getent_r((void *) spw, &fspwent, buffer, buflen,
				   errnop, (void *) name, cmpname);

	NSS_DEBUG("filesplus: %s returning %i\n",__func__, result);
	return result;
}

