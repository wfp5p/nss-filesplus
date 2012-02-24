#include "nss-filesplus.h"

static struct fpent_t fpwent = {
	.maptype   = FP_PASSWD,
        .parser    = pw_parser
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


enum nss_status _nss_filesplus_getpwuid_r(uid_t uid, struct passwd *pwd,
					  char *buffer, size_t buflen,
					  int *errnop)
{
	enum nss_status result = NSS_STATUS_SUCCESS;

	NSS_DEBUG("filesplus: %s\n",__func__);

	result = internal_setent(&fpwent);

	if (result != NSS_STATUS_SUCCESS)
		return result;

	return internal_getent_r((void *) pwd, &fpwent, buffer, buflen,
				 errnop, (void *) &uid, cmpuid);
}

enum nss_status _nss_filesplus_getpwnam_r(const char *name,
					  struct passwd *pwd, char *buffer,
					  size_t buflen, int *errnop)
{
	enum nss_status result = NSS_STATUS_SUCCESS;

	NSS_DEBUG("filesplus: %s\n",__func__);

	result = internal_setent(&fpwent);

	if (result != NSS_STATUS_SUCCESS)
		return result;

	return internal_getent_r((void *) pwd, &fpwent, buffer, buflen,
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
	enum nss_status result = NSS_STATUS_SUCCESS;

	NSS_DEBUG("filesplus: %s\n",__func__);

	if (fpwent.stream == NULL)
		result = internal_setent(&fpwent);

	if (result != NSS_STATUS_SUCCESS)
		return result;

	return internal_getent_r((void *) pwd, &fpwent,  buffer, buflen,
				 errnop, NULL, NULL);
}
