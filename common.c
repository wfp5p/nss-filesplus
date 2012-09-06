#include "nss-filesplus.h"
#include <regex.h>

struct filesplus_config
{
	int configed;
	char *filename[3];
};

static struct filesplus_config _config = {
	.configed = 0,
};

static const char *config_fields[] = {
	"passwd", "group", "shadow", NULL
};

/* must be in same order as config_fields */
static const char *config_files[] = {
	"/etc/filesplus/passwd",
	"/etc/filesplus/group",
	"/etc/filesplus/shadow",
	NULL
};

/* if the use of regex causes issues, use nss_parse_file logic instead */
static int readconfig(void)
{
	FILE *fh;
	char line[MAX_LINE_SIZE];
	char *key, *val;
	regex_t re;
	regmatch_t re_match[3];
	int x, i;

	fh = fopen(CONFIG_FILE, "r");
	if (fh == NULL)
		return 1;

	if (regcomp(&re,"^(passwd|group|shadow):[[:space:]]+(.+)$",
		    REG_EXTENDED|REG_NEWLINE)) {
		NSS_DEBUG("filesplus: %s regcomp failed\n",__func__);
		return 1;
	}

	while (fgets(line, MAX_LINE_SIZE, fh) != 0) {
		x = regexec(&re, line, 3, re_match, 0);
		if (x == 0) {
			key = line;
			*(line + re_match[1].rm_eo) = '\0';

			val = (line+re_match[2].rm_so);
			*(line + re_match[2].rm_eo) = '\0';

			for (i = 0; config_fields[i] != NULL; i++) {
				if (strcmp(config_fields[i],key) == 0)
					_config.filename[i] = strdup(val);
			}
		}
	}

	fclose(fh);

	regfree(&re);
	return 0;
}

void _nss_filesplus_init(void)
{
	int x;

	NSS_DEBUG("filesplus: %s\n",__func__);
	if (_config.configed == 0)
		readconfig();

	for (x = 0; x < 3; x++) {
		if (_config.filename[x] == NULL)
			_config.filename[x] = (char *) config_files[x];
		NSS_DEBUG("filesplus: %s fname %i = %s\n",__func__,
			  x, _config.filename[x]);
	}
	_config.configed = 1;

	return;
}


enum nss_status internal_setent(struct fpent_t *fpinfo)
{
	if (fpinfo->stream) {
		rewind(fpinfo->stream);
		return NSS_STATUS_SUCCESS;
	}

	if (_config.configed == 0)
		_nss_filesplus_init();

	if (_config.configed == 0)
		return NSS_STATUS_UNAVAIL;

	fpinfo->stream = fopen(_config.filename[fpinfo->maptype], "rme");

	if (fpinfo->stream == NULL) {
		if (errno == EAGAIN)
			return NSS_STATUS_TRYAGAIN;
		return NSS_STATUS_UNAVAIL;
	}

	return NSS_STATUS_SUCCESS;
}

enum nss_status internal_endent(struct fpent_t *fpinfo)
{
	if (fpinfo->stream != NULL) {
		fclose(fpinfo->stream);
		fpinfo->stream = NULL;
	}

	return NSS_STATUS_SUCCESS;
}

