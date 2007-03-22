typedef union {
    Dword i;
    char *s;
} YYSTYPE;
#define	T_RUN	257
#define	T_STEP	258
#define	T_QUIT	259
#define	T_RESET	260
#define	T_HELP	261
#define	T_DONE	262
#define	T_EOL	263
#define	T_FORMAT	264
#define	T_GREGISTER	265
#define	T_SREGISTER	266
#define	T_NUMBER	267
#define	T_STRING	268
#define	T_ERROR	269
#define	T_PC	270
#define	T_IR	271
#define	T_FLAGS	272
#define	T_LOAD	273


extern YYSTYPE yylval;
