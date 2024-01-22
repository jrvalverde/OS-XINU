/* C_ioctl.h */

/* ioctl constants defined */

#define IOC_SIZE_MASK		03777600000

/* my own ioctl function (to get size of buffer) */

#define IOC_BUF_SIZE(x)	((IOC_SIZE_MASK & x) >> 16)
