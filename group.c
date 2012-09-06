#include "nss-filesplus.h"
#include <stdlib.h>

static struct fpent_t fgrent = {
	.maptype   = FP_GROUP,
};

static inline int cmpname(void *d1, void *d2)
{
        struct group *gr = (struct group *) d1;
        char *name = (char *) d2;

        return (strcmp(gr->gr_name, name) == 0);
}

static inline int cmpgid(void *d1, void *d2)
{
	struct group *gr = (struct group *) d1;
	gid_t *gid = (gid_t *) d2;

	return (*gid == gr->gr_gid);
}

static enum nss_status internal_getgrent(struct group *gr, char *buffer,
					 size_t buflen, int *errnop,
					 void *cmpdata,
					 int (*cmpFunc)(void *, void *))
{
	struct group *grpp;
	int r = 0;
	enum nss_status result = NSS_STATUS_SUCCESS;

	if ( (fgrent.stream == NULL) || (cmpdata != NULL) )
		result = internal_setent(&fgrent);

	if (result != NSS_STATUS_SUCCESS)
		return result;

	while (!r) {
		r = fgetgrent_r(fgrent.stream, gr, buffer, buflen, &grpp);

		if ( (cmpFunc == NULL) ||
		     (cmpFunc(gr, cmpdata)))
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
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_getgrent(gr, buffer, buflen, errnop, NULL, NULL);

}

enum nss_status _nss_filesplus_getgrnam_r(const char *name, struct group *gr,
					  char *buffer, size_t buflen,
					  int *errnop)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_getgrent(gr, buffer, buflen, errnop,
				 (void *) name, cmpname);

}

enum nss_status _nss_filesplus_getgrgid_r(const gid_t gid, struct group *gr,
					  char *buffer, size_t buflen,
					  int *errnop)
{
	NSS_DEBUG("filesplus: %s\n",__func__);

	return internal_getgrent(gr, buffer, buflen, errnop,
				 (void *) &gid, cmpgid);
}


enum nss_status _nss_filesplus_initgroups_dyn(const char *user,
					      gid_t gid,
					      long int *start,
					      long int *size,
					      gid_t **groupsp,
					      long int limit,
					      int *errnop)
{
	enum nss_status result = NSS_STATUS_SUCCESS;
	struct group gr;
	char *buffer;
	size_t buffer_size;
	int found_one = 0;
	int i;

	gid_t *groups = *groupsp;

	NSS_DEBUG("filesplus: %s\n",__func__);

	result = internal_setent(&fgrent);
	if (result != NSS_STATUS_SUCCESS)
		return result;

	buffer_size = MAX_LINE_SIZE;
	buffer = (char *) malloc(buffer_size);

	while (result == NSS_STATUS_SUCCESS) {
		result = internal_getgrent(&gr, buffer, buffer_size,
					   errnop, NULL, NULL);

		if ((result == NSS_STATUS_TRYAGAIN) &&
		    ( *errnop == ERANGE)) {
			char *new_buf;
			buffer_size = 2 * buffer_size;
			new_buf = (char *) realloc(buffer, buffer_size);
			if (new_buf == NULL) {
				*errnop = errno;
				free(buffer);
				result = NSS_STATUS_UNAVAIL;
				break;
			}
			buffer = new_buf;
			*errnop = 0;
			result = NSS_STATUS_SUCCESS;
			continue;
		}

		if (result != NSS_STATUS_SUCCESS)
			continue;

		if (gr.gr_gid == gid)
			continue;

		for (i = 0; ; i++) {
			if (gr.gr_mem[i] == NULL)
				break;

			if (strcmp(gr.gr_mem[i], user) == 0) {
				/* Matches user.  Insert this group.  */
				if (*start == *size) {
					/* Need a bigger buffer.  */
					if (limit > 0 && *size == limit)
						/* We reached the maximum.  */
						goto out;

					long int newsize;
					if (limit <= 0)
						newsize = 2 * *size;
					else
						newsize = MIN (limit, 2 * *size);

					gid_t *newgroups = realloc (groups,
								    newsize * sizeof (*groups));
					if (newgroups == NULL)
					{
						*errnop = ENOMEM;
						result = NSS_STATUS_TRYAGAIN;
						goto out;
					}
					*groupsp = groups = newgroups;
					*size = newsize;
				}
				groups[*start] = gr.gr_gid;
				*start += 1;
				found_one = 1;
				break;
			}
		}
	}

out:
	free(buffer);
	internal_endent(&fgrent);

	if (NSS_STATUS_NOTFOUND && found_one)
		result = NSS_STATUS_SUCCESS;

	return result;
}
