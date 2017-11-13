#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_DEFAULT_PREFIX "/usr/local"
#define CONFIG_DEFAULT_LIBDIR "lib"
#define CONFIG_VERSION "0.9.1"

const char *Config_Prefix(void);
const char *Config_LibDir(void);

#endif
