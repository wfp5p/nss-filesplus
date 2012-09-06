#include "nss-filesplus.h"

static struct fpent_t fspwent = {
	.maptype   = FP_SHADOW,
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

static enum nss_status internal_getspent(struct spwd *spw, char *buffer,
					 size_t buflen, int *errnop,
					 void *cmpdata,
					 int (*cmpFunc)(void *, void *))
{
	struct spwd *spwp;
	int r = 0;
	enum nss_status result = NSS_STATUS_SUCCESS;

	if ((fspwent.stream == NULL) || (cmpdata != NULL))
		result = internal_setent(&fspwent);

	if (result != NSS_STATUS_SUCCESS)
		return result;

	while (!r) {
		r = fgetspent_r(fspwent.stream, spw, buffer, buflen, &spwp);

		if ( (cmpFunc == NULL) ||
		     (cmpFunc(spw, cmpdata)))
			break;
	}


	if (r == ERANGE) {
		*errnop = ERANGE;
		result = NSS_STATUS_TRYAGAIN;
	} else if (r) {
		result = NSS_STATUS_NOTFOUND;
	}

	return result;
}

enum nss_status _nss_filesplus_getspent_r(struct spwd *spw, char *buffer,
					  size_t buflen, int *errnop)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_getspent(spw, buffer, buflen,
				 errnop, NULL, NULL);
}


enum nss_status _nss_filesplus_getspnam_r(const char *name, struct spwd *spw,
					  char *buffer, size_t buflen,
					  int *errnop)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_getspent(spw, buffer, buflen,
				 errnop, (void *) name, cmpname);

}

