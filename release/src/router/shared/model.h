#ifndef __MODEL_H__
#define __MODEL_H__

#define MODEL_UNUSED(x)	__MDL_UNUSED(x)
#define __MDL_UNUSED(s)	MODEL_unused ## s

/* NOTE: Do not insert new entries in the middle of this enum,
 * always add them to the end! */
enum {
	MODEL_GENERIC = -1,
	MODEL_UNKNOWN = 0,
	MODEL_DSLN55U,
	MODEL_DSLAC68U,
	MODEL_EAN66,
	MODEL_RTN11P,
	MODEL_RTN300,
	MODEL_RTN13U,
	MODEL_RTN14U,
	MODEL_RTAC52U,
	MODEL_RTAC51U,
	MODEL_RTN54U,
	MODEL_RTAC54U,
	MODEL_RTN56UB1,
	MODEL_RTN56UB2,
	MODEL_RTAC1200HP,
	MODEL_RTAC55U,
	MODEL_RTAC55UHP,
	MODEL_RT4GAC55U,
	MODEL_RTAC59U,
	MODEL_PLN12,
	MODEL_PLAC56,
	MODEL_PLAC66U,
	MODEL_RTAC58U,
	MODEL_RT4GAC53U,
	MODEL_RTAC82U,
	MODEL_MAPAC1300,
	MODEL_MAPAC2200,
	MODEL_VZWAC1300,
	MODEL_MAPAC1750,
	MODEL_MAPAC3000,
	MODEL_MAPAC2200V,
	MODEL_RTN36U3,
	MODEL_RTN56U,
	MODEL_RTN65U,
	MODEL_RTN67U,
	MODEL_RTN12,
	MODEL_RTN12B1,
	MODEL_RTN12C1,
	MODEL_RTN12D1,
	MODEL_RTN12VP,
	MODEL_RTN12HP,
	MODEL_RTN12HP_B1,
	MODEL_APN12,
	MODEL_APN12HP,
	MODEL_RTN16,
	MODEL_RTN18U,
	MODEL_RTN15U,
	MODEL_RTN53,
	MODEL_RTN66U,
	MODEL_RTAC66U,
	MODEL_RTAC68U,
	MODEL_UNUSED(__LINE__),
	MODEL_RTAC87U,
	MODEL_RTAC56S,
	MODEL_RTAC56U,
	MODEL_RTAC53U,
	MODEL_RTAC3200,
	MODEL_RTAC88U,
	MODEL_RTAC3100,
	MODEL_RTAC5300,
	MODEL_GTAC5300,
	MODEL_RTN14UHP,
	MODEL_RTN10U,
	MODEL_RTN10P,
	MODEL_RTN10D1,
	MODEL_RTN10PV2,
	MODEL_RTAC1200,
	MODEL_RTAC1200G,
	MODEL_RTAC1200GP,
	MODEL_RTAC1200GA1,
	MODEL_RTAC1200GU,
	MODEL_RPAC66,
	MODEL_RPAC51,
	MODEL_RTAC51UP,
	MODEL_RTAC53,
	MODEL_RTN11P_B1,
	MODEL_RPAC87,
	MODEL_RTAC85U,
	MODEL_RTAC85P,
	MODEL_RTACRH26,
	MODEL_RTN800HP,
	MODEL_RTAC88N,
	MODEL_BRTAC828,
	MODEL_RTAC88S,
	MODEL_RPAC53,
	MODEL_RPAC68U,
	MODEL_RPAC55,
	MODEL_RTAC86U,
	MODEL_GTAC9600,
	MODEL_BLUECAVE,
	MODEL_RTAD7200,
	MODEL_GTAX6000,
	MODEL_GTAX6000N,
	MODEL_GTAX6000S,
	MODEL_RTAC1200V2,
	MODEL_RTN19,
	MODEL_TUFAC1750,
	MODEL_RTAX88U,
	MODEL_GTAX11000,
	MODEL_RTAX92U,
	MODEL_RTAX95Q,
	MODEL_RTAX56_XD4,
	MODEL_RTAX58U,
	MODEL_RTAX56U,
	MODEL_SHAC1300,
	MODEL_RPAC92,
	MODEL_ZENWIFICD6R,
	MODEL_ZENWIFICD6N,
	MODEL_RTAX86U,
	MODEL_RTAX68U,
	MODEL_RT4GAC56,
	MODEL_DSLAX82U,
	MODEL_RTAX55,
	MODEL_GTAXE11000,
	MODEL_RTACRH18,
	MODEL_MAX
};

/* NOTE: Do not insert new entries in the middle of this enum,
 * always add them to the end! */
enum {
	SWITCH_GENERIC = -1,
	SWITCH_UNKNOWN = 0,
	SWITCH_BCM5325,
	SWITCH_BCM53115,
	SWITCH_BCM53125,
	SWITCH_BCM5301x,
	SWITCH_BCM5301x_EX,
	SWITCH_BCM53134,
	SWITCH_RT305x,
	SWITCH_RT3350,
	SWITCH_RT3352,
	SWITCH_RT5350,
	SWITCH_MT7620,
	SWITCH_MT7621,
	SWITCH_MT7628,
	SWITCH_RTL8365MB,
	SWITCH_RTL8367MB,
	SWITCH_RTL8367RB,
	SWITCH_RTL8370M,
};

extern int get_blver(char *bls);
extern int get_fwver(char *buildno, char *extendno);
extern int get_model(void);
extern char *get_modelid(int model);
extern int get_switch(void);

#endif /* __MODEL_H__ */