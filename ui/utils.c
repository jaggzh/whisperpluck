#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include "utils.h"

char *locate_script(const char *name) {
	char *ourpath = get_executable_path();
	char fullpath[PATH_MAX + 1];

	if (ourpath != NULL) {
		// Try the current executable path
		snprintf(fullpath, sizeof(fullpath), "%s/%s", ourpath, name);
		fprintf(stderr, "Checking path: %s\n", fullpath);
		if (access(fullpath, X_OK) == 0) {
			return strdup(fullpath);
		}

		// Try one directory up
		snprintf(fullpath, sizeof(fullpath), "%s/../%s", ourpath, name);
		fprintf(stderr, "Checking path: %s\n", fullpath);
		if (access(fullpath, X_OK) == 0) {
			return strdup(fullpath);
		}
	}

	// Try the PATH
	return find_in_path(name);
}

char* find_in_path(const char* name) {
	const char* path = getenv("PATH");
	if (path == NULL) {
		return NULL;
	}

	char* path_copy = strdup(path);
	if (path_copy == NULL) {
		return NULL;
	}

	char* saveptr;
	for (char* dir = strtok_r(path_copy, ":", &saveptr); dir != NULL; dir = strtok_r(NULL, ":", &saveptr)) {
		char file[PATH_MAX + 1];
		snprintf(file, sizeof(file), "%s/%s", dir, name);

		if (access(file, X_OK) == 0) {
			free(path_copy);
			return strdup(file);
		}
	}

	free(path_copy);
	return NULL;
}

char *get_executable_path() {
	static char buf[PATH_MAX + 1];
	ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (len != -1) {
		buf[len] = '\0';
		return dirname(buf);  // This will give directory of the executable
	}
	return NULL;
}
