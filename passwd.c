#include "nss-filesplus.h"

static struct fpent_t fpwent = {
	.maptype   = FP_PASSWD,
};


static inline int cmpuid(void *d1, void *d2)
{
	struct passwd *pw = (struct passwd *) d1;
	uid_t *uid = (uid_t *) d2;

	return (*uid == pw->pw_uid);
}

static inline int cmpname(void *d1, void *d2)
{
	struct passwd *pw = (struct passwd *) d1;
	char *name = (char *) d2;

	return (strcmp(pw->pw_name, name) == 0);
}

static enum nss_status internal_getpwent(struct passwd *pw, char *buffer,
					 size_t buflen, int *errnop,
					 void *cmpdata,
					 int (*cmpFunc)(void *, void *))
{
	struct passwd *pwp;
	int r = 0;
	enum nss_status result = NSS_STATUS_SUCCESS;

	if ((fpwent.stream == NULL) || (cmpdata != NULL))
		result = internal_setent(&fpwent);

	if (result != NSS_STATUS_SUCCESS)
		return result;

	while (!r) {
		r = fgetpwent_r(fpwent.stream, pw, buffer, buflen, &pwp);

		if ( (cmpFunc == NULL) ||
		     (cmpFunc(pw, cmpdata)))
			break;
	}


	if (r == ERANGE) {
		*errnop = ERANGE;
		return NSS_STATUS_TRYAGAIN;
	}

	if (r)
		return NSS_STATUS_NOTFOUND;

	return result;
}

enum nss_status _nss_filesplus_getpwuid_r(uid_t uid, struct passwd *pwd,
					  char *buffer, size_t buflen,
					  int *errnop)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_getpwent(pwd, buffer, buflen,
				 errnop, (void *) &uid, cmpuid);
}

enum nss_status _nss_filesplus_getpwnam_r(const char *name,
					  struct passwd *pwd, char *buffer,
					  size_t buflen, int *errnop)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_getpwent(pwd, buffer, buflen,
				 errnop, (void *) name, cmpname);

}


enum nss_status _nss_filesplus_setpwent(void)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_setent(&fpwent);
}


enum nss_status _nss_filesplus_endpwent(void)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_endent(&fpwent);
}


enum nss_status _nss_filesplus_getpwent_r(struct passwd *pwd, char *buffer,
					  size_t buflen, int *errnop)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_getpwent(pwd, buffer, buflen,
				 errnop, NULL, NULL);

}
